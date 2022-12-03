#version 330 core

in vec3 lightIntensity;

out vec4 color;

uniform vec4 inColor;

void main() {
    color = inColor * vec4(lightIntensity, 1.0f);
}