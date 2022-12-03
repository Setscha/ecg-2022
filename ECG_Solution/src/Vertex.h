#pragma once

#include "Utils.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    inline bool operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal;
    }
};

namespace std {
    template<>
    struct hash<Vertex> {
        inline long operator()(const Vertex& x) const {
            // https://stackoverflow.com/questions/5928725/hashing-2d-3d-and-nd-vectors
            return ((long)(x.position.x * 1000) * 73856093 ^ (long)(x.position.y * 1000) * 83492791 ^ (long)(x.position.z * 1000) * 10619863) +
                    ((long)(x.normal.x * 1000) * 73856093 ^ (long)(x.normal.y * 1000) * 10619863 ^ (long)(x.normal.z * 1000) * 83492791);
        }
    };
}