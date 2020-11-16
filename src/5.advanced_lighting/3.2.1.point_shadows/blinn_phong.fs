#version 330 core

in VS_OUT {
   vec3 normal;
   vec3 fragPos;
   vec2 uv;
} fs_in;

uniform vec3 lightPosition;
uniform vec3 viewPos;
uniform float lightFarPlane;

out vec4 fragColor;

uniform sampler2D u_texture;
uniform samplerCube u_shadowMap;

float calculateShadow(vec3 fragPos) {
    vec3 fragToLight = fragPos - lightPosition;
    float closestDepth = texture(u_shadowMap, fragToLight).r;
    closestDepth *= lightFarPlane;
    float currentDepth = length(fragToLight);
    float bias = 0.05;
    return currentDepth - bias > closestDepth ? 1.0 : 0.0;
}

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
    // diffuse
    vec3 lightDir = normalize(lightPos - fragPos); 
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
    float distance = length(lightPos - fragPos);
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
    lighting = lighting * (1.0 - calculateShadow(fs_in.fragPos));
    color *= lighting;
    color = pow(color, vec3(1.0/2.2));
    fragColor = vec4(color, 1.0);
}
