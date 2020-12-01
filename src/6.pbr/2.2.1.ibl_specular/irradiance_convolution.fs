#version 330 core

in vec3 fragPos;

uniform samplerCube environmentMap;

out vec4 fragColor;

const float PI = 3.14159265359;

void main() {

    vec3 normal = normalize(fragPos);

    vec3 irradiance = vec3(0.0);    

    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    float sampleDelta = 0.025;
    float nSamples = 0.0f;

    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta) {
        for (float theta = 0.0; theta < PI / 2.0; theta += sampleDelta) {
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

            irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);

            nSamples++;
        }
    }

    irradiance = PI * irradiance / nSamples;

    fragColor = vec4(irradiance, 1.0);
}