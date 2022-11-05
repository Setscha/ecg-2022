#include "Cylinder.h"

Cylinder::Cylinder(int segments, float height, float radius) {
    generateCylinder(segments, height, radius);
}

void Cylinder::generateCylinder(int segments, float height, float radius) {
    int indicesCount = segments * 4;
    int vertexCount = (2 * segments + 2);
    GLfloat* verticesPositions = new GLfloat[vertexCount * 3];
    double angleIncrease = M_PI * 2 / segments;
    /** segment offset for index array, i.e. where the lower circle positions begin, excluding center*/
    int sIOffset = segments + 1;
    /** segment offset in position array, 3 * sIOffset */
    int sOffset = sIOffset * 3;

    verticesPositions[0] = 0;
    verticesPositions[1] = height / 2;
    verticesPositions[2] = 0;
    verticesPositions[sOffset + 0] = 0;
    verticesPositions[sOffset + 1] = -height / 2;
    verticesPositions[sOffset + 2] = 0;

    for (int i = 1; i <= segments; ++i) {
        // Top circle
        verticesPositions[3 * i] = radius * sin(angleIncrease * i);
        verticesPositions[3 * i + 1] = height / 2;
        verticesPositions[3 * i + 2] = radius * cos(angleIncrease * i);
        // Bottom circle
        verticesPositions[sOffset + 3 * i] = verticesPositions[i * 3];
        verticesPositions[sOffset + 3 * i + 1] = -height / 2;
        verticesPositions[sOffset + 3 * i + 2] = verticesPositions[i * 3 + 2];
    }

    GLuint* indices = new GLuint[indicesCount * 3];
    for (int i = 1, j = 0; i <= segments; ++i, ++j) {
        // Top triangle
        indices[12 * j] = i;
        indices[12 * j + 1] = 1 + (i % segments);
        indices[12 * j + 2] = 0;
        // Side quad
        indices[12 * j + 3] = 1 + (i % segments);
        indices[12 * j + 4] = i;
        indices[12 * j + 5] = sIOffset + i;

        indices[12 * j + 6] = sIOffset + i;
        indices[12 * j + 7] = sIOffset + 1 + (i % segments);
        indices[12 * j + 8] = 1 + (i % segments);
        // Bottom triangle
        indices[12 * j + 9] = sIOffset;
        indices[12 * j + 10] = sIOffset + 1 + (i % segments);
        indices[12 * j + 11] = sIOffset + i;
    }

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

    free(verticesPositions);
    free(indices);
}
