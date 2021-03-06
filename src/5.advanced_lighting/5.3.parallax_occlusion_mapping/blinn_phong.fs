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
uniform sampler2D u_dispMap;

uniform float heightScale;

vec2 ParallaxMapping(vec2 uv, vec3 viewDir) {
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaUV = P / numLayers;

    vec2 currentUV = uv;
    float currentDepthMapValue = texture(u_dispMap, currentUV).r;

    while (currentLayerDepth < currentDepthMapValue) {
        currentUV -= deltaUV;
        currentDepthMapValue = texture(u_dispMap, currentUV).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevUV = currentUV + deltaUV;
    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(u_dispMap, prevUV).r - currentLayerDepth + layerDepth;

    float weigth = afterDepth / (afterDepth - beforeDepth);
    vec2 finalUV = prevUV * weigth + currentUV * (1.0 - weigth);

    return finalUV;
}

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
    vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
    vec2 uv = ParallaxMapping(fs_in.uv, viewDir);

    if ((uv.x < 0.0) || (uv.x > 1.0) || (uv.y < 0.0) || (uv.y > 1.0)) {
        discard;
    }

    vec3 color = texture(u_texture, uv).rgb;
    vec3 lighting = vec3(0.0);
    vec3 normal = texture(u_normalMap, uv).rgb;
    normal = normalize(2.0 * normal - 1.0);
    lighting = BlinnPhong(normal, fs_in.tangentFragPos, fs_in.tangentLightPos, vec3(1.0, 1.0, 1.0));
    color *= lighting;
    fragColor = vec4(color, 1.0);
}
