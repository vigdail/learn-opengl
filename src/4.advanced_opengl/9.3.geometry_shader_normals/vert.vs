#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;


out VS_OUT {
    vec3 norm;
} vs_out;

uniform mat4 u_view;
uniform mat4 u_model;

void main() {
    gl_Position = u_view * u_model * vec4(aPos, 1.0);
    mat3 normalMat = mat3(transpose(inverse(u_view * u_model)));
    vs_out.norm = normalize(vec3(vec4(normalMat * aNormal, 0.0)));
}