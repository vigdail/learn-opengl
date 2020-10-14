#version 330 core

out vec4 glColor;

uniform float uColor;

void main() {
   glColor = vec4(uColor, uColor, uColor, 1.0);
}