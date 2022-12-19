#version 450 core

const int MAX_LIGHTS = 16;

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
uniform PointLight pointLight[MAX_LIGHTS];
uniform int amountOfPointLights;
uniform DirectionalLight directionalLight[MAX_LIGHTS];
uniform int amountOfDirectionalLights;

vec3 calculateDiffuseFromDirectionalLight(DirectionalLight directionalLight);
vec3 calculateSpecularFromDirectionalLight(DirectionalLight directionalLight);
vec3 calculateDiffuseFromPointLight(PointLight pointLight);
vec3 calculateSpecularFromPointLight(PointLight pointLight);

vec3 normal;

void main() {
    normal = normalize(fragNormal);
    // Ambient light
    vec3 lightIntensity = vec3(1, 1, 1) * ka;
    vec3 specularIntensity = {0, 0, 0};

    // Point lights
    for (int i = 0; i < amountOfPointLights; i++) {
        lightIntensity += calculateDiffuseFromPointLight(pointLight[i]);
        specularIntensity += calculateSpecularFromPointLight(pointLight[i]);
    }

    // Directional lights
    for (int i = 0; i < amountOfDirectionalLights; i++) {
        lightIntensity += calculateDiffuseFromDirectionalLight(directionalLight[i]);
        specularIntensity += calculateSpecularFromDirectionalLight(directionalLight[i]);
    }

    color = inColor * vec4(lightIntensity, 1.0f) + vec4(specularIntensity, 1.0f);
}

vec3 calculateDiffuseFromDirectionalLight(DirectionalLight directionalLight) {
    // Directional vector to light source
    vec3 L = normalize(-directionalLight.direction);
    // Reflection
    vec3 R = 2 * dot(L, normal) * normal - L;
    // Directional vector to viewer
    vec3 V = normalize(eyePos - vec3(fragPos));
    return directionalLight.intensity * kd * max(0, dot(L, normal));
}

vec3 calculateSpecularFromDirectionalLight(DirectionalLight directionalLight) {
    // Directional vector to light source
    vec3 L = normalize(-directionalLight.direction);
    // Reflection
    vec3 R = 2 * dot(L, normal) * normal - L;
    // Directional vector to viewer
    vec3 V = normalize(eyePos - vec3(fragPos));
    return directionalLight.intensity * ks * pow(max(0, dot(R, V)), alpha);
}

vec3 calculateDiffuseFromPointLight(PointLight pointLight) {
    // Directional vector to light source
    vec3 L = normalize(pointLight.position - vec3(fragPos));
    // Reflection
    vec3 R = 2 * dot(L, normal) * normal - L;
    // Directional vector to viewer
    vec3 V = normalize(eyePos - vec3(fragPos));
    float attenuationFactor = pointLight.attenuation.x + pointLight.attenuation.y * distance(pointLight.position, vec3(fragPos)) + pointLight.attenuation.z * pow(distance(pointLight.position, vec3(fragPos)), 2);
    return pointLight.intensity / attenuationFactor * kd * max(0, dot(L, normal));
}

vec3 calculateSpecularFromPointLight(PointLight pointLight) {
    vec3 L = normalize(pointLight.position - vec3(fragPos));
    // Reflection
    vec3 R = 2 * dot(L, normal) * normal - L;
    // Directional vector to viewer
    vec3 V = normalize(eyePos - vec3(fragPos));
    float attenuationFactor = pointLight.attenuation.x + pointLight.attenuation.y * distance(pointLight.position, vec3(fragPos)) + pointLight.attenuation.z * pow(distance(pointLight.position, vec3(fragPos)), 2);
    return pointLight.intensity / attenuationFactor * ks * pow(max(0, dot(R, V)), alpha);
}