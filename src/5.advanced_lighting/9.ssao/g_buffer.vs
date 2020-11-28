#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 fragPos;
    vec2 uv;
    vec3 normal;
} vs_out;

void main() {
    vs_out.uv = aUV;
    vec4 viewPos = view * model * vec4(aPos, 1.0);
    vs_out.fragPos = viewPos.xyz; 
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    vs_out.normal = normalMatrix * aNormal;
    gl_Position =  projection * viewPos;
}