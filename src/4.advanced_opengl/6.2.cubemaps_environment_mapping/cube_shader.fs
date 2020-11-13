#version 330 core

in vec3 Normal;
in vec3 fragPos;
in vec2 uv;

out vec4 fragColor;

uniform vec3 u_camera;
uniform samplerCube skybox;

void main() {
   vec3 I = normalize(fragPos - u_camera);
   vec3 R = reflect(I, normalize(Normal));
   fragColor = vec4(texture(skybox, R).rgb, 1.0);
}