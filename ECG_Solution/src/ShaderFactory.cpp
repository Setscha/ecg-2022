#include "ShaderFactory.h"

Shader ShaderFactory::createGouraudShader(glm::vec4 color, float ka, float kd, float ks, int alpha) {
    Shader shader("assets/shaders/gouraud.vsh", "assets/shaders/gouraud.fsh");
    shader.activate();
    shader.setUniform4f("inColor", color.r, color.g, color.b, color.w);
    shader.setUniform1f("ka", ka);
    shader.setUniform1f("kd", kd);
    shader.setUniform1f("ks", ks);
    shader.setUniform1i("alpha", alpha);

    return shader;
}

Shader ShaderFactory::createPhongShader(glm::vec4 color, float ka, float kd, float ks, int alpha) {
    Shader shader("assets/shaders/phong.vsh", "assets/shaders/phong.fsh");
    shader.activate();
    shader.setUniform4f("inColor", color.r, color.g, color.b, color.w);
    shader.setUniform1f("ka", ka);
    shader.setUniform1f("kd", kd);
    shader.setUniform1f("ks", ks);
    shader.setUniform1i("alpha", alpha);

    return shader;
}
