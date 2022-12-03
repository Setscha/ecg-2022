#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main() {
    vec4 vertexPos = modelMatrix * vec4(position, 1.0);
    gl_Position = viewMatrix * vertexPos;

    fragPos = vec3(vertexPos);
    fragNormal = normal;
}