#version 330 core

in vec2 o_uv;

uniform sampler2D u_texture;

out vec4 o_color;

void main() {
    o_color = vec4(vec3(1.0 - texture(u_texture, o_uv)), 1.0);
}