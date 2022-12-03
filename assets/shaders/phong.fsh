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

in vec3 fragPos;
in vec3 fragNormal;

out vec4 color;

uniform vec4 inColor;
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
    // Ambient light
    vec3 lightIntensity = vec3(1, 1, 1) * ka;

    // PointLight
    // Directional vector to light source
    vec3 L = normalize(pointLight.position - vec3(fragPos));
    // Reflection
    vec3 R = 2 * dot(L, fragNormal) * fragNormal - L;
    // Directional vector to viewer
    vec3 V = normalize(eyePos - vec3(fragPos));
    float attenuationFactor = pointLight.attenuation.x + pointLight.attenuation.y * distance(pointLight.position, vec3(fragPos)) + pointLight.attenuation.z * pow(distance(pointLight.position, vec3(fragPos)), 2);
    lightIntensity += pointLight.intensity / attenuationFactor * kd * max(0, dot(L, fragNormal)) + pointLight.intensity / attenuationFactor * ks * pow(max(0, dot(R, V)), alpha);

    // DirectionalLight
    L = normalize(-directionalLight.direction);
    R = 2 * dot(L, fragNormal) * fragNormal - L;
    lightIntensity += directionalLight.intensity * kd * max(0, dot(L, fragNormal)) + directionalLight.intensity * ks * pow(max(0, dot(R, V)), alpha);

    color = inColor * vec4(lightIntensity, 1.0f);
}