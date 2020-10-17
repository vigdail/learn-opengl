#version 330 core

in vec3 Normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;

void main() {
   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor;

   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPosition - fragPos);
   float diff = max(dot(lightDir, Normal), 0.0);
   vec3 diffuse = diff * lightColor;

   float specularStrength = 0.5;
   vec3 viewDir = normalize(cameraPosition - fragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * lightColor;


   vec3 result = (ambient + diffuse + specular) * objectColor;
   fragColor = vec4(result, 1.0);
}