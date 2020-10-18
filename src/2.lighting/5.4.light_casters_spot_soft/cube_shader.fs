#version 330 core

in vec3 Normal;
in vec3 fragPos;
in vec2 uv;

out vec4 fragColor;

uniform vec3 cameraPosition;

struct Material {
   sampler2D diffuse;
   sampler2D specular;
   float shininess;
};

struct Light {
   vec3 position;
   vec3 direction;
   float cutOff;
   float outerCutOff;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float constant;
   float linear;
   float quadratic;
};

uniform Material material;
uniform Light light;

void main() {

   vec3 lightDir = normalize(light.position - fragPos);

   float theta = dot(lightDir, normalize(-light.direction)); 
   float epsilon = light.cutOff - light.outerCutOff;
   float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

   vec3 ambient = light.ambient * texture(material.diffuse, uv).rgb;

   vec3 norm = normalize(Normal);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * texture(material.diffuse, uv).rgb;

   vec3 viewDir = normalize(cameraPosition - fragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * spec * texture(material.specular, uv).rgb;

   float distance = length(light.position-fragPos);
   float attenuation = 1.0 / (light.constant + distance * light.linear + distance * distance * light.quadratic);

   vec3 result = (ambient + (diffuse + specular) * intensity) * attenuation;

   fragColor = vec4(result, 1.0);
}