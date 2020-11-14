#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec3 color;

void main() {
    gl_Position = u_proj * u_view * u_model * vec4(aPos * gl_InstanceID / 100.0 + aOffset, 0.0, 1.0);
    color = aColor;
}