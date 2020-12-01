#version 330 core

in vec3 fragPos;

uniform samplerCube cubemap;

out vec4 fragColor;

void main(){
    vec3 color = texture(cubemap, fragPos).rgb;

    // tonemap
    color = color / (1.0 + color);
    color = pow(color, vec3(1.0) / 2.2);

    fragColor = vec4(color, 1.0);
}