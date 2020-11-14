#version 330 core

in vec3 uv;

uniform samplerCube skybox;

out vec4 o_color;

void main() {
    o_color = texture(skybox, uv);
}