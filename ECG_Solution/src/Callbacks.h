#pragma once

#include "Utils.h"

inline double pitch = 0;
inline double yaw = -90;
inline float distance = 6;

inline double pxpos = -1;
inline double pypos = -1;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);