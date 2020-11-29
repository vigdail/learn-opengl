#version 330 core

in vec3 fragPos;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 sampleSphericalMap(vec3 v) {
    vec2 uv = vec2(atan(v.z,v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;

    return uv;
}

out vec4 fragColor;

void main() {
    vec2 uv = sampleSphericalMap(normalize(fragPos));
    vec3 color = texture(equirectangularMap, uv).rgb;

    fragColor = vec4(color, 1.0);
}