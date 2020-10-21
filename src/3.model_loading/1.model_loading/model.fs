#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 fragPos;

struct PointLight {
   vec3 position;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float constant;
   float linear;
   float quadratic;
};

uniform PointLight light;

uniform vec3 cameraPosition;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

out vec4 glColor;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
   vec3 viewDir = normalize(cameraPosition - fragPos);
   vec3 result = vec3(0.0);
   result += calcPointLight(light, normalize(normal), fragPos, viewDir);

   glColor = vec4(result, 1.0);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
   vec3 lightDir = normalize(light.position - fragPos);
   
   float diff = max(dot(normal, lightDir), 0.0);
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

   float distance = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + distance * light.linear + distance * distance * light.quadratic);

   vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, uv));
   vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, uv));
   vec3 specular = light.specular * spec * vec3(texture(texture_specular1, uv));

   ambient *= attenuation;
   diffuse *= attenuation;
   specular *= attenuation;

   return ambient + diffuse + specular;
}