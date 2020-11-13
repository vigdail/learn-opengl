#version 330 core

in vec3 Normal;
in vec3 fragPos;
in vec2 uv;

out vec4 fragColor;

uniform sampler2D diffuse;

void main() {
   fragColor = texture(diffuse, uv);
}