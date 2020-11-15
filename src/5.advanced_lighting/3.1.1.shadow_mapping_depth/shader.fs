#version 330 core

in VS_OUT {
   vec2 uv;
} fs_in;

out vec4 fragColor;

uniform sampler2D depthmap;

void main()
{           
    float color = texture(depthmap, fs_in.uv).r;
    fragColor = vec4(color, color, color, 1.0);
}
