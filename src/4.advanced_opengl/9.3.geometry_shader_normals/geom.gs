#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 norm;
} vs_in[];

const float MAGNITUDE = 0.4;

uniform mat4 u_proj;

void generateLine(int index) {

    gl_Position = u_proj * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = u_proj * (gl_in[index].gl_Position + vec4(vs_in[index].norm, 0.0) * MAGNITUDE);
    EmitVertex();

    EndPrimitive();
}

void main() {
    generateLine(0);
    generateLine(1);
    generateLine(2);
}