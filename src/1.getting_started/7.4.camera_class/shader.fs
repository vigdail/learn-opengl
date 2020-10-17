#version 330 core

out vec4 glColor;

in vec2 uv;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

void main() {
   glColor = mix(texture(u_texture1, uv), texture(u_texture2, uv), 0.2);
}