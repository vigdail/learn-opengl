#version 330 core

layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices {
    mat4 u_proj;
    mat4 u_view;
};
uniform mat4 u_model;

void main() {
    gl_Position = u_proj * u_view * u_model * vec4(aPos, 1.0);
}