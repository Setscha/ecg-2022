#include "Cube.h"

Cube::Cube(float width, float height, float depth) {
    generateCube(width, height, depth);
}

void Cube::generateCube(float width, float height, float depth) {
    int vertexCount = 8;
    int indicesCount = 12;
    float wh = width / 2;
    float hh = height / 2;
    float dh = depth / 2;

    GLfloat verticesPositions[24] = {
            -wh, -hh, -dh,
            -wh, -hh, dh,
            -wh, hh, -dh,
            -wh, hh, dh,
            wh, -hh, -dh,
            wh, -hh, dh,
            wh, hh, -dh,
            wh, hh, dh,
    };

    GLuint indices[] = {
            0, 1, 3,
            3, 2, 0,
            1, 5, 7,
            7, 3, 1,
            5, 4, 6,
            6, 7, 5,
            4, 0, 2,
            2, 6, 4,
            3, 7, 6,
            6, 2, 3,
            0, 4, 5,
            5, 1, 0
    };

    iboSize = indicesCount * 3;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), verticesPositions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glDisableVertexAttribArray(0);
}
