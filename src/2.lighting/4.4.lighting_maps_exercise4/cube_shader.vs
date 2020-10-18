#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec3 Normal;
out vec3 fragPos;
out vec2 uv;

void main() {
    fragPos = vec3(u_model * vec4(aPos, 1.0));
    gl_Position = u_proj * u_view * vec4(fragPos, 1.0);
    mat3 normalMatrix = mat3(transpose(inverse(u_model)));
    Normal = normalMatrix * aNormal;
    uv = aUV;
}