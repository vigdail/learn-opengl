#version 330 core

out vec4 fragColor;

in vec2 uv;

uniform sampler2D image;
uniform bool horizontal;
uniform float weights[5] = float[] (0.227027, 0.1945946, 0.1216216,
                                    0.054054, 0.016216);

void main() {
    vec2 tex_offset = 1.0 / textureSize(image, 0);
    vec3 result = texture(image, uv).rgb * weights[0];

    vec2 p;
    if (horizontal) {
        p = vec2(tex_offset.x, 0.0);
    } else {
        p = vec2(0.0, tex_offset.y);
    }

    for (int i = 1; i < 5; ++i) {
        result += texture(image, uv + p * i).rgb * weights[i];
        result += texture(image, uv - p * i).rgb * weights[i];
    }

    fragColor = vec4(result, 1.0);
}