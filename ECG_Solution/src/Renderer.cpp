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
    GLfloat* sphereVerticesPositions = new GLfloat[(2 + longSegments * (latSegments - 1)) * 3];
    double latAngleIncrease = M_PI / latSegments;
    double longAngleIncrease = M_PI * 2 / longSegments;
    /** segment offset for index array, i.e. where the lower circle positions begin, excluding center*/
    //int sIOffset = segments + 1;
    /** segment offset in position array, 3 * sIOffset */
    //int sOffset = sIOffset * 3;

    sphereVerticesPositions[0] = 0;
    sphereVerticesPositions[1] = radius;
    sphereVerticesPositions[2] = 0;
    sphereVerticesPositions[(2 + longSegments * (latSegments - 1)) * 3 - 3] = 0;
    sphereVerticesPositions[(2 + longSegments * (latSegments - 1)) * 3 - 2] = -radius;
    sphereVerticesPositions[(2 + longSegments * (latSegments - 1)) * 3 - 1] = 0;

    for (int i = 1; i < latSegments; ++i) {
        double latSin = sin(latAngleIncrease * i);
        double latCos = cos(latAngleIncrease * i);
        for (int j = 0; j < longSegments; ++j) {
            // Top circle
            sphereVerticesPositions[3 + longSegments * (i - 1) * 3 + 3 * j] = radius * latSin * cos(longAngleIncrease * j);
            sphereVerticesPositions[3 + longSegments * (i - 1) * 3 + 3 * j + 1] = radius * latCos;
            sphereVerticesPositions[3 + longSegments * (i - 1) * 3 + 3 * j + 2] = radius * latSin * sin(longAngleIncrease * j);
        }
    }

    GLuint* indices = new GLuint[(2 * longSegments * (latSegments - 1)) * 3];
    for (int j = 0; j < longSegments; ++j) {
        // Top cap
        indices[3 * j] = 0;
        indices[3 * j + 1] = 1 + ((j + 1) % longSegments);
        indices[3 * j + 2] = j + 1;
        // Bottom cap
        indices[(2 * longSegments * (latSegments - 1)) * 3 - (3 * j + 1)] =
                2 + longSegments * (latSegments - 1) - 1 - (j + 1);
        indices[(2 * longSegments * (latSegments - 1)) * 3 - (3 * j + 2)] =
                2 + longSegments * (latSegments - 1) - 1 - (1 + ((j + 1) % longSegments));
        indices[(2 * longSegments * (latSegments - 1)) * 3 - (3 * j + 3)] =
                2 + longSegments * (latSegments - 1) - 1;
    }

    for (int i = 1; i < latSegments - 1; ++i) {
        for (int j = 0; j < longSegments; ++j) {
            // TODO: longSegments * i * 6 should be longSegments * i * 3 for the top cap, because its only tris
            indices[longSegments * 3 + longSegments * (i - 1) * 6 + 6 * j] = longSegments * i + j + 1;
            indices[longSegments * 3 + longSegments * (i - 1) * 6 + 6 * j + 1] = longSegments * (i - 1) + 1 + (j + 1) % longSegments;
            indices[longSegments * 3 + longSegments * (i - 1) * 6 + 6 * j + 2] = longSegments * i + 1 + (j + 1) % longSegments;
            indices[longSegments * 3 + longSegments * (i - 1) * 6 + 6 * j + 3] = longSegments * i + j + 1;
            indices[longSegments * 3 + longSegments * (i - 1) * 6 + 6 * j + 4] = longSegments * (i - 1) + j + 1;
            indices[longSegments * 3 + longSegments * (i - 1) * 6 + 6 * j + 5] = longSegments * (i - 1) + 1 + (j + 1) % longSegments;
        }
    }

    drawBuffers(2 + longSegments * (latSegments - 1), 2 * longSegments * (latSegments - 1), sphereVerticesPositions, indices);

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
