#version 330 core

out vec4 fragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;
uniform sampler2D ssaoMap;

struct Light {
    vec3 position;
    vec3 color;
    float linear;
    float quadratic;
};

uniform Light light;
// uniform vec3 viewPos;

in VS_OUT {
    vec2 uv;
} fs_in;

void main() {
    vec3 Position = texture(gPosition,fs_in.uv).rgb;
    vec3 Normal = texture(gNormal,fs_in.uv).rgb;
    vec3 Albedo = texture(gColorSpec,fs_in.uv).rgb;
    float AmbientOcclusion = texture(ssaoMap, fs_in.uv).r;

    vec3 ambient = vec3(0.3 * Albedo * AmbientOcclusion);
    vec3 lighting = ambient;
    vec3 lightDir = normalize(light.position - Position);
    vec3 viewDir = normalize(-Position);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * light.color;
    vec3 halfWay = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfWay), 0.0), 16.0);
    vec3 specular = spec * Albedo * light.color;

    float distance = length(light.position - Position);
    float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * distance * distance);

    diffuse *= attenuation;
    specular *= attenuation;

    lighting += diffuse + specular;

    fragColor = vec4(lighting, 1.0);
}
