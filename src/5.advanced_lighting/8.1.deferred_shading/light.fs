#version 330 core

out vec4 fragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;

struct Light {
    vec3 position;
    vec3 color;
    float linear;
    float quadratic;
};

const int N_LIGHTS = 32;
uniform Light lights[N_LIGHTS];
uniform vec3 viewPos;

in VS_OUT {
    vec2 uv;
} fs_in;

void main() {
    vec3 Position = texture(gPosition,fs_in.uv).rgb;
    vec3 Normal = texture(gNormal,fs_in.uv).rgb;
    vec3 Albedo = texture(gColorSpec,fs_in.uv).rgb;
    float Specular = texture(gColorSpec,fs_in.uv).a;

    vec3 lighting = vec3(0.0); // Albedo * 0.05;
    vec3 viewDir = normalize(viewPos - Position);
    for (int i = 0; i < N_LIGHTS; i++) {
        vec3 lightDir = normalize(lights[i].position - Position);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lights[i].color;

        vec3 halfWay = normalize(lightDir + viewDir);
        float spec = pow(max(dot(Normal, halfWay), 0.0), 16.0);
        vec3 specular = spec * Albedo * lights[i].color;

        float distance = length(lights[i].position - Position);
        float attenuation = 1.0 / (1.0 + lights[i].linear * distance + lights[i].quadratic * distance * distance);

        diffuse *= attenuation;
        specular *= attenuation;

        lighting += diffuse + specular;
    }

    fragColor = vec4(lighting, 1.0);
}
