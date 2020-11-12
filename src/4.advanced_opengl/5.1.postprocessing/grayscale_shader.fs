#version 330 core

in vec2 o_uv;

uniform sampler2D u_texture;

out vec4 o_color;

void main() {
    vec4 color = texture(u_texture, o_uv);
    float gray = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    o_color = vec4(gray, gray, gray, 1.0);
}