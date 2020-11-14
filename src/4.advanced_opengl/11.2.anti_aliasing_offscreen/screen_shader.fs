#version 330 core

in vec2 o_uv;

uniform sampler2D u_texture;

out vec4 o_color;

void main() {
    vec4 color = texture(u_texture, o_uv);
    float grey = (color.r + color.g  + color.b) / 3.0;
    o_color = vec4(grey, grey, grey, 1.0);
}