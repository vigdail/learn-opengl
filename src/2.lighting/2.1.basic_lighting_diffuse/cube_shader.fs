#version 330 core

in vec3 Normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPosition;

void main() {
   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor;

   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPosition - fragPos);
   float diff = max(dot(lightDir, norm), 0.0);
   vec3 diffuse = diff * lightColor;

   vec3 result = (ambient + diffuse) * objectColor;
   fragColor = vec4(result, 1.0);
}