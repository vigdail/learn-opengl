#version 330 core

float near = 0.1;
float far = 100.0;

out vec4 fragColor;

float linearizeDepth(float depth) {
   float z = depth * 2.0 - 1.0;
   return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
   float depth = linearizeDepth(gl_FragCoord.z) / far;
   fragColor = vec4(vec3(depth), 1.0);
}