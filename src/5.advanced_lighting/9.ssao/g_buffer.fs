#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColorSpec;


in VS_OUT {
    vec3 fragPos;
    vec2 uv;
    vec3 normal;
} fs_in;

void main() {
    gPosition = fs_in.fragPos;
    gNormal = normalize(fs_in.normal);
    gColorSpec.rgb = vec3(0.95f);
}
