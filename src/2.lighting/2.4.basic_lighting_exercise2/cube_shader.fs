#version 330 core

in vec3 Normal;
in vec3 fragPos;
in vec3 lightPos;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor;

   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - fragPos);
   float diff = max(dot(lightDir, Normal), 0.0);
   vec3 diffuse = diff * lightColor;

   float specularStrength = 0.5;
   vec3 viewDir = normalize(-fragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * lightColor;


   vec3 result = (ambient + diffuse + specular) * objectColor;
   fragColor = vec4(result, 1.0);
}