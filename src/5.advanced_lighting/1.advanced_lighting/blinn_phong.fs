#version 330 core

in VS_OUT {
   vec3 normal;
   vec3 fragPos;
   vec2 uv;
} fs_in;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;

out vec4 fragColor;

uniform sampler2D u_texture;

void main() {
   vec3 color = texture(u_texture, fs_in.uv).rgb;

   vec3 ambient = 0.05 * color;

   // diffuse
   vec3 lightDir = normalize(lightPos - fs_in.fragPos);
   vec3 normal = normalize(fs_in.normal);
   float diff = max(dot(lightDir, normal), 0.0);
   vec3 diffuse = diff * color;

   // specular
   vec3 viewDir = normalize(viewPos - fs_in.fragPos);
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = 0.0;
   if (blinn) {
      vec3 halfwayDir = normalize(viewDir + lightDir);
      spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
   } else {
      spec = pow(max(dot(normal, reflectDir), 0.0), 8.0);
   }
   vec3 specular = vec3(0.3) * spec;

   fragColor = vec4(ambient + diffuse + specular, 1.0);
}