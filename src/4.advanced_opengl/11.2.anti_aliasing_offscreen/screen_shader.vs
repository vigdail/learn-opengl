#version 330 core

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_view;

out vec2 o_uv;

void main() {
    gl_Position = vec4(a_pos.x, a_pos.y, 0.0, 1.0);
    o_uv = a_uv;
}