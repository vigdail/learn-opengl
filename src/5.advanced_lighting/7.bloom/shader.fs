#version 330 core
layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

in VS_OUT {
    vec3 Normal;
    vec3 fragPos;
    vec2 uv;
} fs_in;

#define POINT_LIGHTS_COUNT 4

struct PointLight {
   vec3 position;
   vec3 color;
};

uniform vec3 viewPos;
uniform PointLight pointLights[POINT_LIGHTS_COUNT];
uniform sampler2D diffuseTexture;

void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.uv).rgb;
    vec3 normal = normalize(fs_in.Normal);
    // ambient
    vec3 ambient = 0.0 * color;
    // lighting
    vec3 lighting = vec3(0.0);
    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    for(int i = 0; i < 4; i++)
    {
        // diffuse
        vec3 lightDir = normalize(pointLights[i].position - fs_in.fragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 result = pointLights[i].color * diff * color;      
        // attenuation (use quadratic as we have gamma correction)
        float distance = length(fs_in.fragPos - pointLights[i].position);
        result *= 1.0 / (distance * distance);
        lighting += result;
                
    }
    vec3 result = ambient + lighting;
    // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        brightColor = vec4(result, 1.0);
    else
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
    fragColor =  vec4(result, 1.0);
}