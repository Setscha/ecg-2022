#include "Renderer.h"

void Renderer::drawTeapot(const Shader& shader) const {
    shader.activate();
    ::drawTeapot();
}

void Renderer::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderDrawable(const Shader &shader, Drawable& drawable) {
    shader.activate();
    drawable.draw();
}
