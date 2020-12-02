#version 330 core

out vec4 glColor;

in vec3 color;
in vec2 uv;

uniform sampler2D u_texture;

void main() {
   glColor = texture(u_texture, uv);
}