#version 330 core

in vec2 uv;

out vec4 fragColor;

uniform sampler2D u_texture;
uniform float exposure;

void main()
{           
    float gamma = 2.2;
    vec3 hdrColor = texture(u_texture, uv).rgb;

    vec3 color = vec3(1.0) - exp(-hdrColor * exposure);

    color = pow(color, vec3(1.0) / gamma);

    fragColor = vec4(color, 1.0);
}
