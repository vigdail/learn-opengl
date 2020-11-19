#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 lightPosition;
uniform vec3 viewPos;

out VS_OUT {
    vec3 normal;
    vec3 fragPos;
    vec2 uv;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} vs_out;

void main() {
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    vs_out.normal = normalMatrix * aNormal;
    vs_out.uv = aUV;

    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    vs_out.tangentLightPos = TBN * lightPosition;
    vs_out.tangentViewPos  = TBN * viewPos;
    vs_out.tangentFragPos  = TBN * vs_out.fragPos;

    gl_Position = proj * view * vec4(vs_out.fragPos, 1.0);
}