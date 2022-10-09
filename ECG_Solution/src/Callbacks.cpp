#pragma once

#include "Callbacks.h"

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (pxpos >= 0) {
            double deltaX = xpos - pxpos;
            double deltaY = ypos - pypos;
            pitch -= deltaY;
            pitch = glm::min(glm::max(-89.999, pitch), 89.999);
            yaw += deltaX;
            yaw = fmod(yaw, 360.0);
        }
        pxpos = xpos;
        pypos = ypos;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        pxpos = -1;
        pypos = -1;
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    distance -= (float)yoffset;
    distance = glm::max(1.0f, distance);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    // https://www.khronos.org/opengl/wiki/Debug_Output
    if (id == 131185 || id == 131218) return; // ignore performance warnings (buffer uses GPU memory, shader recompilation) from nvidia

    std::string sourceText;
    switch (source) {
        case GL_DEBUG_SOURCE_API:             sourceText = "API"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     sourceText = "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           sourceText = "Other"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceText = "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceText = "Third Party"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceText = "Window System"; break;
    }

    std::string typeText;
    switch (type) {
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeText = "Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_ERROR:               typeText = "Error"; break;
        case GL_DEBUG_TYPE_MARKER:              typeText = "Marker"; break;
        case GL_DEBUG_TYPE_OTHER:               typeText = "Other"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         typeText = "Performance"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           typeText = "Pop Group"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         typeText = "Portability"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          typeText = "Push Group"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeText = "Undefined Behaviour"; break;
    }

    std::string severityText;
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         severityText = "High"; break;
        case GL_DEBUG_SEVERITY_LOW:          severityText = "Low"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       severityText = "Medium"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: severityText = "Notification"; break;
    }

    fprintf(stderr,
            "%s [ID: %u, Source: %s, Type: %s, Severity: %s]\n",
            message, id, sourceText.c_str(), typeText.c_str(), severityText.c_str()
    );
}