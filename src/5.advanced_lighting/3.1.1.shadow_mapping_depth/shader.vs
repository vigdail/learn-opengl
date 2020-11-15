#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

out VS_OUT {
    vec2 uv;
} vs_out;

void main() {
    vs_out.uv = aUV;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}