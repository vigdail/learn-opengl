#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in mat4 aModel;

uniform mat4 u_view;
uniform mat4 u_proj;

out vec2 uv;

void main() {
    gl_Position = u_proj * u_view * aModel * vec4(aPos, 1.0);
    uv = aUV;
}