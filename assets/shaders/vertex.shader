#version 450 core

uniform mat4 transformMatrix;
in vec3 position;
out vec4 gl_Position;

void main() {
    gl_Position = transformMatrix * vec4(position, 1.0);
}