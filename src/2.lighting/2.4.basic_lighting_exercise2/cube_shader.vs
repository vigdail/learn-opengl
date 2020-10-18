#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform vec3 lightPosition;

out vec3 Normal;
out vec3 fragPos;
out vec3 lightPos;

void main() {
    fragPos = vec3(u_view * u_model * vec4(aPos, 1.0));
    gl_Position = u_proj * vec4(fragPos, 1.0);
    mat3 normalMatrix = mat3(transpose(inverse(u_view * u_model)));
    Normal = normalMatrix * aNormal;
    lightPos = vec3(u_view * vec4(lightPosition, 1.0));
}