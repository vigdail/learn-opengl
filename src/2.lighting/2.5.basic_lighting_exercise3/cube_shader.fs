#version 330 core

in vec3 o_lightColor;

out vec4 fragColor;

uniform vec3 objectColor;

void main() {
   fragColor = vec4(o_lightColor * objectColor, 1.0);
}