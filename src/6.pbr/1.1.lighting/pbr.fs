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

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

out vec4 glColor;

vec3 fresnelShlick(float cosTheta, vec3 F0) {
   return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness) {
   float a = roughness * roughness;
   float a2 = a * a;

   float NdotH = max(dot(N, H), 0.0);
   float NdotH2 = NdotH * NdotH;

   float num = a2;
   float denom = (NdotH2 * (a2 - 1.0) + 1.0);
   denom = PI * denom * denom;

   return num / max(denom, 0.001);
}

float geometryShlickGGX(float NdotV, float roughness) {
   float r = roughness + 1.0;
   float k = r * r / 8.0;

   float num = NdotV;
   float denom = NdotV * (1.0 - k) + k;

   return num / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
   float NdotV = max(dot(N, V), 0.0);
   float NdotL = max(dot(N, L), 0.0);

   float ggx2 = geometryShlickGGX(NdotV, roughness);
   float ggx1 = geometryShlickGGX(NdotL, roughness);

   return ggx1 * ggx2;
}

void main() {
   vec3 N = normalize(fs_in.normal);
   vec3 V = normalize(cameraPosition - fs_in.fragPos);

   vec3 F0 = vec3(0.04);
   F0 = mix(F0, albedo, metallic);

   vec3 Lo = vec3(0.0);

   for (int i = 0; i < LIGHTS_COUNT; i++) {
      PointLight light = lights[i];
      vec3 L = normalize(light.position - fs_in.fragPos);
      vec3 H = normalize(V + L);

      float distance = length(light.position - fs_in.fragPos);
      float attenuation = 1.0 / (distance * distance);
      vec3 radiance = light.color * attenuation;

      float NDF = distributionGGX(N, H, roughness);
      float G = geometrySmith(N, V, L, roughness);
      vec3 F = fresnelShlick(clamp(dot(H, V), 0.0, 1.0), F0);

      vec3 nominator = NDF * G * F;
      float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
      vec3 specular = nominator / max(denominator, 0.001);

      vec3 kS = F;
      vec3 kD = vec3(1.0) - kS;
      kD *= 1.0 - metallic;

      float NdotL = max(dot(N, L), 0.0);
      Lo += (kD * albedo / PI + specular) * radiance * NdotL;
   }

   vec3 ambient = vec3(0.03) * albedo * ao;
   vec3 color = ambient + Lo;

   // HDR
   color = color / (vec3(1.0) + color);
   color = pow(color, vec3(1.0) / 2.2);

   glColor = vec4(color, 1.0);
}
