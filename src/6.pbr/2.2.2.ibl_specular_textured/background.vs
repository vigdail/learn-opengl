#version 330 core

layout (location = 0) in vec3 aPosition;

uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;

void main() {
    fragPos = aPosition;

    mat4 rotView = mat4(mat3(view));
    vec4 clipPos = projection * rotView * vec4(aPosition, 1.0);

    gl_Position = clipPos.xyww;
}