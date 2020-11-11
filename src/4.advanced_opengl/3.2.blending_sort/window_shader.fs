#version 330 core

out vec4 FragColor;
in vec2 uv;
uniform sampler2D ui_texture;

void main()
{
    FragColor = texture(ui_texture, uv);
}
