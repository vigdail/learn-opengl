#version 330 core

out float fragColor;

uniform sampler2D ssaoTexture;

in VS_OUT {
    vec2 uv;
} fs_in;

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(ssaoTexture, 0));

    float result = 0.0;

    for (int x = -2; x < 2; x++) {
        for (int y = -2; y < 2; y++) {
            vec2 uv = fs_in.uv + vec2(float(x), float(y)) * texelSize;
            result += texture(ssaoTexture, uv).r;
        }
    }

    fragColor = result / 4.0 / 4.0;
}
