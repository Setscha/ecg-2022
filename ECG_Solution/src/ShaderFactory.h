#pragma once

#include "Shader.h"
#include "Camera.h"

class ShaderFactory {
public:
    static Shader createGouraudShader(glm::vec4 color, float ka, float kd, float ks, int alpha);
    static Shader createPhongShader(glm::vec4 color, float ka, float kd, float ks, int alpha);
};