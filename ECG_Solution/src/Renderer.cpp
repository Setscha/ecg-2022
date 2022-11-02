#pragma once

#include "Renderer.h"

void Renderer::drawTeapot(const Shader& shader) const {
    shader.activate();
    ::drawTeapot();
}

void Renderer::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::drawCube(float width, float height, float depth) {
    // TODO: maybe cache the positions and don't calculate them every frame

    float wh = width / 2;
    float hh = height / 2;
    float dh = depth / 2;
    GLfloat cubeVerticesPositions[24] = {
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

    drawBuffers(24, 12, cubeVerticesPositions, indices);

    cleanupBuffers();
}

void Renderer::drawCylinder(int segments, float height, float radius) {
    GLfloat* cylinderVerticesPositions = new GLfloat[(2 * segments + 2) * 3];
    double angleIncrease = M_PI * 2 / segments;
    /** segment offset for index array, i.e. where the lower circle positions begin, excluding center*/
    int sIOffset = segments + 1;
    /** segment offset in position array, 3 * sIOffset */
    int sOffset = sIOffset * 3;

    cylinderVerticesPositions[0] = 0;
    cylinderVerticesPositions[1] = height / 2;
    cylinderVerticesPositions[2] = 0;
    cylinderVerticesPositions[sOffset + 0] = 0;
    cylinderVerticesPositions[sOffset + 1] = -height / 2;
    cylinderVerticesPositions[sOffset + 2] = 0;

    for (int i = 1; i <= segments; ++i) {
        // Top circle
        cylinderVerticesPositions[3 * i] = radius * sin(angleIncrease * i);
        cylinderVerticesPositions[3 * i + 1] = height / 2;
        cylinderVerticesPositions[3 * i + 2] = radius * cos(angleIncrease * i);
        // Bottom circle
        cylinderVerticesPositions[sOffset + 3 * i] = cylinderVerticesPositions[i * 3];
        cylinderVerticesPositions[sOffset + 3 * i + 1] = -height / 2;
        cylinderVerticesPositions[sOffset + 3 * i + 2] = cylinderVerticesPositions[i * 3 + 2];
    }

    GLuint* indices = new GLuint[segments * 4 * 3];
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

    drawBuffers((2 * segments + 2), segments * 4, cylinderVerticesPositions, indices);

    cleanupBuffers();
    free(cylinderVerticesPositions);
    free(indices);
}

void Renderer::drawSphere(int longSegments, int latSegments, float radius) {
    int vertexCount = (2 + longSegments * (latSegments - 1));
    int indicesCount = (2 * longSegments * (latSegments - 1));
    double latAngleIncrease = M_PI / latSegments;
    double longAngleIncrease = M_PI * 2 / longSegments;

    GLfloat* sphereVerticesPositions = new GLfloat[vertexCount * 3];

    sphereVerticesPositions[0] = 0;
    sphereVerticesPositions[1] = radius;
    sphereVerticesPositions[2] = 0;
    sphereVerticesPositions[vertexCount * 3 - 3] = 0;
    sphereVerticesPositions[vertexCount * 3 - 2] = -radius;
    sphereVerticesPositions[vertexCount * 3 - 1] = 0;

    for (int i = 1; i < latSegments; ++i) {
        double latSin = sin(latAngleIncrease * i);
        double latCos = cos(latAngleIncrease * i);
        for (int j = 0; j < longSegments; ++j) {
            int latOffset = longSegments * (i - 1) * 3;
            int longOffset = 3 * j;
            sphereVerticesPositions[3 + latOffset + longOffset] = radius * latSin * cos(longAngleIncrease * j);
            sphereVerticesPositions[3 + latOffset + longOffset + 1] = radius * latCos;
            sphereVerticesPositions[3 + latOffset + longOffset + 2] = radius * latSin * sin(longAngleIncrease * j);
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

    drawBuffers(vertexCount, indicesCount, sphereVerticesPositions, indices);

    cleanupBuffers();
    free(sphereVerticesPositions);
    free(indices);
}

void Renderer::drawBuffers(int vertexBufferSize, int indexBufferSize, GLfloat* positions, GLuint* indices) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize * 3 * sizeof(GLfloat), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, indexBufferSize * 3, GL_UNSIGNED_INT, nullptr);
}

void Renderer::cleanupBuffers() const {
    glDisableVertexAttribArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}
