#version 330 core

in VS_OUT {
    vec3 normal;
    vec3 fragPos;
    vec2 uv;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} fs_in;

out vec4 fragColor;

uniform sampler2D u_texture;
uniform sampler2D u_normalMap;


vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
    // diffuse
    vec3 lightDir = normalize(lightPos - fragPos); 
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(fs_in.tangentViewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * lightColor;    
    // simple attenuation
    float distance = length(lightPos - fragPos);
    float attenuation = 2.0 / distance;
    
    diffuse *= attenuation;
    specular *= attenuation;
    
    return diffuse + specular;
}

void main()
{           
    vec3 color = texture(u_texture, fs_in.uv).rgb;
    vec3 lighting = vec3(0.0);
    vec3 normal;
    normal = texture(u_normalMap, fs_in.uv).rgb;
    normal = normalize(2.0 * normal - 1.0);
    lighting = BlinnPhong(normal, fs_in.tangentFragPos, fs_in.tangentLightPos, vec3(1.0, 1.0, 1.0));
    color *= lighting;
    fragColor = vec4(color, 1.0);
}
