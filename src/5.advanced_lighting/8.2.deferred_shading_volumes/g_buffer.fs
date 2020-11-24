#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColorSpec;

uniform sampler2D texture_diffure1;
uniform sampler2D texture_specular1;

in VS_OUT {
    vec3 fragPos;
    vec2 uv;
    vec3 normal;
} fs_in;

void main() {
    gPosition = fs_in.fragPos;
    gNormal = normalize(fs_in.normal);
    gColorSpec.rgb = texture(texture_diffure1, fs_in.uv).rgb;
    gColorSpec.a = texture(texture_specular1, fs_in.uv).r;
}
