#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
   vec3 fragPos;
   vec3 normal;
   vec2 uv;
} vs_out;

void main() {
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    vs_out.normal = normalMatrix * aNorm;
    vs_out.uv = aUV;

    gl_Position = projection * view * vec4(vs_out.fragPos, 1.0);
}