#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

// uniform bool inverseNormals;

out VS_OUT {
    vec3 Normal;
    vec3 fragPos;
    vec2 uv;
} vs_out;

void main() {
    vs_out.fragPos = vec3(u_model * vec4(aPos, 1.0));
    mat3 normalMatrix = mat3(transpose(inverse(u_model)));
    vs_out.Normal = normalize(normalMatrix * aNormal);
    vs_out.uv = aUV;

    gl_Position = u_proj * u_view * vec4(vs_out.fragPos, 1.0);
}