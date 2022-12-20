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

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 intensity;
    vec3 attenuation;
    float angle;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec4 fragCol;

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
uniform PointLight pointLight[MAX_LIGHTS];
uniform int amountOfPointLights;
uniform DirectionalLight directionalLight[MAX_LIGHTS];
uniform int amountOfDirectionalLights;
uniform SpotLight spotLight[MAX_LIGHTS];
uniform int amountOfSpotLights;
uniform vec4 inColor;

vec3 calculateDiffuseFromDirectionalLight(DirectionalLight directionalLight);
vec3 calculateSpecularFromDirectionalLight(DirectionalLight directionalLight);
vec3 calculateDiffuseFromPointLight(PointLight pointLight);
vec3 calculateSpecularFromPointLight(PointLight pointLight);
vec3 calculateDiffuseFromSpotLight(SpotLight spotLight);
vec3 calculateSpecularFromSpotLight(SpotLight spotLight);

vec4 vertexPos;

void main() {
    vertexPos = modelMatrix * vec4(position, 1.0);
    gl_Position = viewMatrix * vertexPos;

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

    // Spot lights
    for (int i = 0; i < amountOfSpotLights; i++) {
        lightIntensity += calculateDiffuseFromSpotLight(spotLight[i]);
        specularIntensity += calculateSpecularFromSpotLight(spotLight[i]);
    }

    fragCol = inColor * vec4(lightIntensity, 1.0f) + vec4(specularIntensity, 1.0f);
}

vec3 calculateDiffuseFromDirectionalLight(DirectionalLight directionalLight) {
    // Directional vector to light source
    vec3 L = normalize(-directionalLight.direction);
    return directionalLight.intensity * kd * max(0, dot(L, normal));
}

vec3 calculateSpecularFromDirectionalLight(DirectionalLight directionalLight) {
    // Directional vector to light source
    vec3 L = normalize(-directionalLight.direction);
    // Reflection
    vec3 R = 2 * dot(L, normal) * normal - L;
    // Directional vector to viewer
    vec3 V = normalize(eyePos - vec3(vertexPos));
    return directionalLight.intensity * ks * pow(max(0, dot(R, V)), alpha);
}

vec3 calculateDiffuseFromPointLight(PointLight pointLight) {
    // Directional vector to light source
    vec3 L = normalize(pointLight.position - vec3(vertexPos));
    float attenuationFactor = pointLight.attenuation.x + pointLight.attenuation.y * distance(pointLight.position, vec3(vertexPos)) + pointLight.attenuation.z * pow(distance(pointLight.position, vec3(vertexPos)), 2);
    return pointLight.intensity / attenuationFactor * kd * max(0, dot(L, normal));
}

vec3 calculateSpecularFromPointLight(PointLight pointLight) {
    vec3 L = normalize(pointLight.position - vec3(vertexPos));
    // Reflection
    vec3 R = 2 * dot(L, normal) * normal - L;
    // Directional vector to viewer
    vec3 V = normalize(eyePos - vec3(vertexPos));
    float attenuationFactor = pointLight.attenuation.x + pointLight.attenuation.y * distance(pointLight.position, vec3(vertexPos)) + pointLight.attenuation.z * pow(distance(pointLight.position, vec3(vertexPos)), 2);
    return pointLight.intensity / attenuationFactor * ks * pow(max(0, dot(R, V)), alpha);
}

vec3 calculateDiffuseFromSpotLight(SpotLight spotLight) {
    // Directional vector to light source
    vec3 L = normalize(spotLight.position - vec3(vertexPos));
    float attenuationFactor = spotLight.attenuation.x + spotLight.attenuation.y * distance(spotLight.position, vec3(vertexPos)) + spotLight.attenuation.z * pow(distance(spotLight.position, vec3(vertexPos)), 2);

    vec3 spotDir = normalize(-spotLight.direction);
    float intensity = smoothstep(0.0f, 0.02f, max(dot(L, spotDir) - cos(spotLight.angle), 0.0f));

    return intensity * spotLight.intensity / attenuationFactor * kd * max(0, dot(L, normal));
}

vec3 calculateSpecularFromSpotLight(SpotLight spotLight) {
    vec3 L = normalize(spotLight.position - vec3(vertexPos));
    // Reflection
    vec3 R = 2 * dot(L, normal) * normal - L;
    // Directional vector to viewer
    vec3 V = normalize(eyePos - vec3(vertexPos));
    float attenuationFactor = spotLight.attenuation.x + spotLight.attenuation.y * distance(spotLight.position, vec3(vertexPos)) + spotLight.attenuation.z * pow(distance(spotLight.position, vec3(vertexPos)), 2);

    vec3 spotDir = normalize(-spotLight.direction);
    float intensity = smoothstep(0.0f, 0.02f, max(dot(L, spotDir) - cos(spotLight.angle), 0.0f));

    return intensity * spotLight.intensity / attenuationFactor * ks * pow(max(0, dot(R, V)), alpha);
}