#pragma once

#include "Shader.h"
#include "Drawables/Drawable.h"

class Renderer {
public:
    void clear() const;
    void renderDrawable(const Shader& shader, Drawable& drawable);
    void drawTeapot(const Shader& shader) const;
};