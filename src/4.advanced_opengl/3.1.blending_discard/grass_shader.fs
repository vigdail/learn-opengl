#version 330 core

// in vec3 i_1;
// in vec3 i_2;
in vec2 uv;

uniform sampler2D u_texture;

out vec4 fragColor;

void main() {
   vec4 tex_color = texture(u_texture, uv);

   if (tex_color.a < 0.1){
      discard;
   }

   fragColor = tex_color;
}