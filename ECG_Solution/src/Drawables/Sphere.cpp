#include "Sphere.h"

Sphere::Sphere(int longSegments, int latSegments, float radius) {
    generateSphere(longSegments, latSegments, radius);
}

void Sphere::generateSphere(int longSegments, int latSegments, float radius) {
    int vertexCount = (2 + longSegments * (latSegments - 1));
    int indicesCount = (2 * longSegments * (latSegments - 1));
    double latAngleIncrease = M_PI / latSegments;
    double longAngleIncrease = M_PI * 2 / longSegments;

    GLfloat* verticesPositions = new GLfloat[vertexCount * 3];

    verticesPositions[0] = 0;
    verticesPositions[1] = radius;
    verticesPositions[2] = 0;
    verticesPositions[vertexCount * 3 - 3] = 0;
    verticesPositions[vertexCount * 3 - 2] = -radius;
    verticesPositions[vertexCount * 3 - 1] = 0;

    for (int i = 1; i < latSegments; ++i) {
        double latSin = sin(latAngleIncrease * i);
        double latCos = cos(latAngleIncrease * i);
        for (int j = 0; j < longSegments; ++j) {
            int latOffset = longSegments * (i - 1) * 3;
            int longOffset = 3 * j;
            verticesPositions[3 + latOffset + longOffset] = radius * latSin * cos(longAngleIncrease * j);
            verticesPositions[3 + latOffset + longOffset + 1] = radius * latCos;
            verticesPositions[3 + latOffset + longOffset + 2] = radius * latSin * sin(longAngleIncrease * j);
        }
    }

    GLuint* indices = new GLuint[indicesCount * 3];
    for (int j = 0; j < longSegments; ++j) {
        int longOffset = 3 * j;
        int vertAIndex = 1 + ((j + 1) % longSegments);
        int vertBIndex = j + 1;
        // Top cap
        indices[longOffset] = 0;
        indices[longOffset + 1] = vertAIndex;
        indices[longOffset + 2] = vertBIndex;
        // Bottom cap
        indices[indicesCount * 3 - (longOffset + 1)] = vertexCount - 1 - vertBIndex;
        indices[indicesCount * 3 - (longOffset + 2)] = vertexCount - 1 - vertAIndex;
        indices[indicesCount * 3 - (longOffset + 3)] = vertexCount - 1;
    }

    int topCapOffset = longSegments * 3;

    for (int i = 1; i < latSegments - 1; ++i) {
        int latOffset = longSegments * (i - 1) * 6;
        for (int j = 0; j < longSegments; ++j) {
            int longOffset = 6 * j;
            int vertAIndex = longSegments * (i - 1) + 1 + (j + 1) % longSegments;
            int vertBIndex = longSegments * (i - 1) + j + 1;
            int vertCIndex = longSegments * i + 1 + (j + 1) % longSegments;
            int vertDIndex = longSegments * i + j + 1;

            indices[topCapOffset + latOffset + longOffset] = vertBIndex;
            indices[topCapOffset + latOffset + longOffset + 1] = vertAIndex;
            indices[topCapOffset + latOffset + longOffset + 2] = vertCIndex;

            indices[topCapOffset + latOffset + longOffset + 3] = vertBIndex;
            indices[topCapOffset + latOffset + longOffset + 4] = vertCIndex;
            indices[topCapOffset + latOffset + longOffset + 5] = vertDIndex;
        }
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
