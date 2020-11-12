#version 330 core

in vec2 o_uv;

uniform sampler2D u_texture;

out vec4 o_color;

void main() {
    o_color = texture(u_texture, o_uv);
}