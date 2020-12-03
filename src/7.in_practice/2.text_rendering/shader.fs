#version 330 core

in vec2 uv;

uniform vec3 color;
uniform sampler2D bitmap;

out vec4 fragColor;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(bitmap, uv).r);
    fragColor = vec4(color, 1.0) * sampled;
}