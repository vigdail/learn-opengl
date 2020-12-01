#version 330 core

in VS_OUT {
   vec3 fragPos;
   vec3 normal;
   vec2 uv;
} fs_in;

struct PointLight {
   vec3 position;
   vec3 color;
};

const int LIGHTS_COUNT = 4;
const float PI = 3.14159265359;

uniform PointLight lights[LIGHTS_COUNT];
uniform vec3 cameraPosition;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

out vec4 fragColor;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, fs_in.uv).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_in.fragPos);
    vec3 Q2  = dFdy(fs_in.fragPos);
    vec2 st1 = dFdx(fs_in.uv);
    vec2 st2 = dFdy(fs_in.uv);

    vec3 N   = normalize(fs_in.normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
   return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
   return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}


float distributionGGX(vec3 N, vec3 H, float roughness) {
   float a = roughness*roughness;
   float a2 = a*a;
   float NdotH = max(dot(N, H), 0.0);
   float NdotH2 = NdotH*NdotH;

   float nom   = a2;
   float denom = (NdotH2 * (a2 - 1.0) + 1.0);
   denom = PI * denom * denom;

   return nom / denom;
}

float geometryShlickGGX(float NdotV, float roughness) {
   float r = (roughness + 1.0);
   float k = (r*r) / 8.0;

   float nom   = NdotV;
   float denom = NdotV * (1.0 - k) + k;

   return nom / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
   float NdotV = max(dot(N, V), 0.0);
   float NdotL = max(dot(N, L), 0.0);

   float ggx2 = geometryShlickGGX(NdotV, roughness);
   float ggx1 = geometryShlickGGX(NdotL, roughness);

   return ggx1 * ggx2;
}

void main() {
   vec3 N = getNormalFromMap();
   vec3 V = normalize(cameraPosition - fs_in.fragPos);
   vec3 R = reflect(-V, N);

   vec3 albedo = texture(albedoMap, fs_in.uv).rgb;
   float metallic = texture(metallicMap, fs_in.uv).r;
   float roughness = texture(roughnessMap, fs_in.uv).r;
   float ao = texture(aoMap, fs_in.uv).r;

   // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
   // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
   vec3 F0 = vec3(0.04); 
   F0 = mix(F0, albedo, metallic);

   // reflectance equation
   vec3 Lo = vec3(0.0);
   for(int i = 0; i < 4; ++i) 
   {
       // calculate per-light radiance
       vec3 L = normalize(lights[i].position - fs_in.fragPos);
       vec3 H = normalize(V + L);
       float distance = length(lights[i].position - fs_in.fragPos);
       float attenuation = 1.0 / (distance * distance);
       vec3 radiance = lights[i].color * attenuation;

       // Cook-Torrance BRDF
       float NDF = distributionGGX(N, H, roughness);   
       float G   = geometrySmith(N, V, L, roughness);    
       vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
       
       vec3 nominator    = NDF * G * F;
       float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
       vec3 specular = nominator / denominator;
       
        // kS is equal to Fresnel
       vec3 kS = F;
       // for energy conservation, the diffuse and specular light can't
       // be above 1.0 (unless the surface emits light); to preserve this
       // relationship the diffuse component (kD) should equal 1.0 - kS.
       vec3 kD = vec3(1.0) - kS;
       // multiply kD by the inverse metalness such that only non-metals 
       // have diffuse lighting, or a linear blend if partly metal (pure metals
       // have no diffuse light).
       kD *= 1.0 - metallic;	                
           
       // scale light by NdotL
       float NdotL = max(dot(N, L), 0.0);        

       // add to outgoing radiance Lo
       Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
   }   
   
   // ambient lighting (we now use IBL as the ambient term)
   vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
   vec3 kS = F;
   vec3 kD = 1.0 - kS;
   kD *= 1.0 - metallic;	  
   
   vec3 irradiance = texture(irradianceMap, N).rgb;
   vec3 diffuse      = irradiance * albedo;
   
   // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
   const float MAX_REFLECTION_LOD = 4.0;
   vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
   vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
   vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

   vec3 ambient = (kD * diffuse + specular) * ao;
   
   vec3 color = ambient + Lo;

   // HDR tonemapping
   color = color / (color + vec3(1.0));
   // gamma correct
   color = pow(color, vec3(1.0/2.2)); 

   fragColor = vec4(color, 1.0);
}
