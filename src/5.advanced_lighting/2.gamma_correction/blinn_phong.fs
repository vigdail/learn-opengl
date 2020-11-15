#version 330 core

in VS_OUT {
   vec3 normal;
   vec3 fragPos;
   vec2 uv;
} fs_in;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 viewPos;
uniform bool gamma;

out vec4 fragColor;

uniform sampler2D u_texture;


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
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // simple attenuation
    float max_distance = 1.5;
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (gamma ? distance * distance : distance);
    
    diffuse *= attenuation;
    specular *= attenuation;
    
    return diffuse + specular;
}

void main()
{           
    vec3 color = texture(u_texture, fs_in.uv).rgb;
    vec3 lighting = vec3(0.0);
    for(int i = 0; i < 4; ++i) {
        lighting += BlinnPhong(normalize(fs_in.normal), fs_in.fragPos, lightPositions[i], lightColors[i]);
    }
    color *= lighting;
    if(gamma) {
        color = pow(color, vec3(1.0/2.2));
    }
    fragColor = vec4(color, 1.0);
}
