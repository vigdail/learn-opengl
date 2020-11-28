#version 330 core

out float fragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D ssaoNoise;

const int kernelSize = 64;
uniform vec3 ssaoKernel[kernelSize];
uniform mat4 projection;

const float radius = 0.5;
const float bias = 0.025;

const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0);

in VS_OUT {
    vec2 uv;
} fs_in;

void main() {
    vec3 fragPos = texture(gPosition,fs_in.uv).xyz;
    vec3 normal = texture(gNormal,fs_in.uv).rgb;
    vec3 randomVec = texture(ssaoNoise, fs_in.uv * noiseScale).xyz;

    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; i++) {
        vec3 sample = TBN * ssaoKernel[i];
        sample = fragPos + sample * radius;

        vec4 offset = vec4(sample, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = texture(gPosition, offset.xy).z;

        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;
    }

    fragColor = 1.0 - occlusion / kernelSize;
}
