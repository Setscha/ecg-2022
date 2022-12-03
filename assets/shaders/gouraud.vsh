#version 330 core

struct PointLight {
    vec3 position;
    vec3 intensity;
    vec3 attenuation;
};

struct DirectionalLight {
    vec3 direction;
    vec3 intensity;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 lightIntensity;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
// ambient lighting factor
uniform float ka;
// diffuse lighting factor
uniform float kd;
// specular lighting factor
uniform float ks;
// specular factor
uniform int alpha;
uniform vec3 eyePos;
uniform PointLight pointLight;
uniform DirectionalLight directionalLight;

void main() {
    vec4 vertexPos = modelMatrix * vec4(position, 1.0);
    gl_Position = viewMatrix * vertexPos;

    // Ambient light
    lightIntensity = vec3(1, 1, 1) * ka;

    // PointLight
    // Directional vector to light source
    vec3 L = normalize(pointLight.position - vec3(vertexPos));
    // Reflection
    vec3 R = 2 * dot(L, normal) * normal - L;
    // Directional vector to viewer
    vec3 V = normalize(eyePos - vec3(vertexPos));
    float attenuationFactor = pointLight.attenuation.x + pointLight.attenuation.y * distance(pointLight.position, vec3(vertexPos)) + pointLight.attenuation.z * pow(distance(pointLight.position, vec3(vertexPos)), 2);
    lightIntensity += pointLight.intensity / attenuationFactor * kd * max(0, dot(L, normal)) + pointLight.intensity / attenuationFactor * ks * pow(max(0, dot(R, V)), alpha);

    // DirectionalLight
    L = normalize(-directionalLight.direction);
    R = 2 * dot(L, normal) * normal - L;
    lightIntensity += directionalLight.intensity * kd * max(0, dot(L, normal)) + directionalLight.intensity * ks * pow(max(0, dot(R, V)), alpha);
}