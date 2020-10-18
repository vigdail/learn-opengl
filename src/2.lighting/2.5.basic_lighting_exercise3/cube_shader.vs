#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec3 objectColor;
uniform vec3 lightColor;

out vec3 o_lightColor;

void main() {
    vec3 position = vec3(u_model * vec4(aPos, 1.0));
    mat3 normalMatrix = mat3(transpose(inverse(u_model)));
    vec3 Normal = normalMatrix * aNormal;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
 
    float specularStrength = 0.5;
    vec3 viewDir = normalize(cameraPosition-position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
 
    o_lightColor = (ambient + diffuse + specular);

    gl_Position = u_proj * u_view * vec4(position, 1.0);
}