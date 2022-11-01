#pragma  once

#include "Utils.h"
#include "Shader.h"

class Renderer {
private:
public:
    void clear() const;
    void drawTeapot(const Shader& shader) const;
    void drawCube(float width, float height, float depth) const;
};