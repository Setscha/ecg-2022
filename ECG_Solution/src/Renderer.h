#pragma  once

#define _USE_MATH_DEFINES
#include "Utils.h"
#include "Shader.h"
#include <cmath>

class Renderer {
private:
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
public:
    void clear() const;
    void drawTeapot(const Shader& shader) const;
    void drawCube(float width, float height, float depth);
    void drawCylinder(int segments, float height, float radius);
    void drawBuffers(int vertexBufferSize, int indexBufferSize, GLfloat* positions, GLuint* indices);
    void cleanupBuffers() const;
};