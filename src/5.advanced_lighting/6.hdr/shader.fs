#version 330 core

in vec3 Normal;
in vec3 fragPos;
in vec2 uv;

out vec4 fragColor;

#define POINT_LIGHTS_COUNT 4

uniform sampler2D diffuse;

struct PointLight {
   vec3 position;
   vec3 color;
};

uniform vec3 cameraPosition;

uniform PointLight pointLights[POINT_LIGHTS_COUNT];

void main()
{           
    vec3 color = texture(diffuse, uv).rgb;
    vec3 normal = normalize(Normal);
    // ambient
    vec3 ambient = 0.0 * color;
    // lighting
    vec3 lighting = vec3(0.0);
    for(int i = 0; i < POINT_LIGHTS_COUNT; i++)
    {
        // diffuse
        vec3 lightDir = normalize(pointLights[i].position - fragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = pointLights[i].color * diff * color;      
        vec3 result = diffuse;        
        // attenuation (use quadratic as we have gamma correction)
        float distance = length(fragPos - pointLights[i].position);
        result *= 1.0 / (distance * distance);
        lighting += result;
                
    }
    fragColor = vec4(ambient + lighting, 1.0);
}