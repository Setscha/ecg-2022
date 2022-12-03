#include "Cube.h"
#include "unordered_map"
#include "../Vertex.h"
#include "glm/gtc/type_ptr.hpp"

Cube::Cube(float width, float height, float depth) {
    generateCube(width, height, depth);
}

void Cube::generateCube(float width, float height, float depth) {
    int vertexCount = 8 * 3;
    int indicesCount = 12;
    float wh = width / 2;
    float hh = height / 2;
    float dh = depth / 2;

    Vertex auxVertex[8];

    auxVertex[0] = {{-wh, -hh, -dh}};
    auxVertex[1] = {{-wh, -hh, dh}};
    auxVertex[2] = {{-wh, hh, -dh}};
    auxVertex[3] = {{-wh, hh, dh}};
    auxVertex[4] = {{wh, -hh, -dh}};
    auxVertex[5] = {{wh, -hh, dh}};
    auxVertex[6] = {{wh, hh, -dh}};
    auxVertex[7] = {{wh, hh, dh}};

    GLuint auxIndices[] = {
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

    std::unordered_map<Vertex, int> verticesIndices;
    GLuint* indices = new GLuint[indicesCount * 3];
    Vertex* vertices = new Vertex[vertexCount];

    int index = 0;
    for (int i = 0; i < indicesCount; ++i) {
        GLuint vertA = auxIndices[i * 3];
        GLuint vertB = auxIndices[i * 3 + 1];
        GLuint vertC = auxIndices[i * 3 + 2];

        glm::vec3 normal = glm::normalize(glm::cross(auxVertex[vertA].position - auxVertex[vertB].position, auxVertex[vertB].position - auxVertex[vertC].position));
        Vertex vertANorm({auxVertex[vertA].position, normal});
        Vertex vertBNorm({auxVertex[vertB].position, normal});
        Vertex vertCNorm({auxVertex[vertC].position, normal});

        if (verticesIndices.find(vertANorm) != verticesIndices.end()) {
            indices[i * 3] = verticesIndices[vertANorm];
        } else {
            indices[i * 3] = index;
            vertices[index] = vertANorm;
            verticesIndices[vertANorm] = index++;
        }
        if (verticesIndices.find(vertBNorm) != verticesIndices.end()) {
            indices[i * 3 + 1] = verticesIndices[vertBNorm];
        } else {
            indices[i * 3 + 1] = index;
            vertices[index] = vertBNorm;
            verticesIndices[vertBNorm] = index++;
        }
        if (verticesIndices.find(vertCNorm) != verticesIndices.end()) {
            indices[i * 3 + 2] = verticesIndices[vertCNorm];
        } else {
            indices[i * 3 + 2] = index;
            vertices[index] = vertCNorm;
            verticesIndices[vertCNorm] = index++;
        }
        //glm::vec3 normal = auxVertex[vertA].position - auxVertex[vertB].position
    }

    iboSize = indicesCount * 3;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glDisableVertexAttribArray(0);

    free(vertices);
    free(indices);
}
