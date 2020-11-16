#version 330 core

in VS_OUT {
   vec3 normal;
   vec3 fragPos;
   vec2 uv;
   vec4 fragPosLightSpace;
} fs_in;

uniform vec3 lightPosition;
uniform vec3 viewPos;

out vec4 fragColor;

uniform sampler2D u_texture;
uniform sampler2D u_shadowMap;

float calculateShadow(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = 0.5 * projCoords + 0.5;
    if (projCoords.z > 1.0) {
        return 0.0;
    }
    float currentDepth = projCoords.z;
    vec3 normal = normalize(fs_in.normal);
    vec3 lightDir = normalize(lightPosition);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    vec2 texelSize = 1.0 / textureSize(u_shadowMap, 0);
    float shadow = 0.0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
           float pcfDepth = texture(u_shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
           shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }

    return shadow / 9.0;
}

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
    // diffuse
    vec3 lightDir = normalize(lightPos); 
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * lightColor;    
    // simple attenuation
    float max_distance = 1.5;
    float distance = min(length(lightPos - fragPos), max_distance);
    float attenuation = 1.0 / (distance * distance);
    
    diffuse *= attenuation;
    specular *= attenuation;
    
    return diffuse + specular;
}

void main()
{           
    vec3 color = texture(u_texture, fs_in.uv).rgb;
    vec3 lighting = vec3(0.0);
    lighting += BlinnPhong(normalize(fs_in.normal), fs_in.fragPos, lightPosition, vec3(1.0, 1.0, 1.0));
    lighting = lighting * (1.0 - calculateShadow(fs_in.fragPosLightSpace));
    color *= lighting;
    color = pow(color, vec3(1.0/2.2));
    fragColor = vec4(color, 1.0);
}
