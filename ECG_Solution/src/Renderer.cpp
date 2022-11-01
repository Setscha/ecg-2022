#pragma once

#include "Renderer.h"

void Renderer::drawTeapot(const Shader& shader) const {
    shader.activate();
    ::drawTeapot();
}

void Renderer::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::drawCube(float width, float height, float depth) const {
    GLuint vao;
    GLuint vbo;
    GLuint ibo;

    // TODO: maybe cache the positions and don't calculate them every frame

    float wh = width / 2;
    float hh = height / 2;
    float dh = depth / 2;
    const GLfloat cubeVerticesPositions[24] = {
            -wh, -hh, -dh,
            -wh, -hh, dh,
            -wh, hh, -dh,
            -wh, hh, dh,
            wh, -hh, -dh,
            wh, -hh, dh,
            wh, hh, -dh,
            wh, hh, dh,
    };

    const GLuint indices[] = {
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

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), cubeVerticesPositions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
}
