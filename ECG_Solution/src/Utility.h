#pragma once

#include "Utils.h"

static glm::mat4 getCameraTransform(glm::vec3 g = glm::vec3(0, 0, -1), glm::vec3 e = glm::vec3(0, 0, 6), glm::vec3 t = glm::vec3(0, 1, 0)) {
    {
        using namespace glm;
        vec3 w = - normalize(g);
        vec3 u = normalize(cross(t, w));
        vec3 v = cross(w, u);

        return inverse(mat4(vec4(u, 0), vec4(v, 0), vec4(w, 0), vec4(e, 1)));
    }
}

static glm::mat4 getOrbitCameraTransform(double pitch, double yaw, float distance, glm::vec3 targetPosition) {
    {
        using namespace glm;
        pitch = pitch * M_PI / 180.0;
        yaw = yaw * M_PI / 180.0;

        // Calculate necessary angles and positions for the camera transform
        vec3 direction = {cos(pitch) * cos(yaw), sin(pitch), cos(pitch) * sin(yaw)};
        vec3 right = normalize(cross(direction, vec3(0.0, 1.0, 0.0)));
        vec3 up = normalize(cross(right, direction));

        vec3 position = (-direction * distance) + targetPosition;

        return getCameraTransform(direction, position, up);
    }
}