#pragma once

#include "Shader.h"
#include "Camera.h"

class ShaderManager {
private:
    const int MAX_LIGHTS = 16;
    std::vector<PointLight> pointLights = {};
    std::vector<DirectionalLight> directionalLights = {};
    std::vector<Shader*> shaders = {};
public:
    void updateCameraValues(Camera& camera);
    bool addPointLight(PointLight pointLight);
    bool addDirectionalLight(DirectionalLight directionalLight);
    Shader* createGouraudShader(glm::vec4 color, float ka, float kd, float ks, int alpha);
    Shader* createPhongShader(glm::vec4 color, float ka, float kd, float ks, int alpha);
};