/*
* Copyright 2021 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
#pragma once

#include "Utils.h"
#include "Callbacks.h"

/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */

/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */

/* --------------------------------------------- */
// Main
/* --------------------------------------------- */

int main(int argc, char **argv) {
    /* --------------------------------------------- */
    // Load settings.ini
    /* --------------------------------------------- */

    // init reader for ini files
    INIReader reader("assets/settings.ini");

    // load values from ini file
    // first param: section [window], second param: property name, third param: default value
    int window_width = reader.GetInteger("window", "width", 800);
    int window_height = reader.GetInteger("window", "height", 800);
    std::string window_title = reader.Get("window", "title", "ECG 2022");

    /* --------------------------------------------- */
    // Init framework
    /* --------------------------------------------- */

    if (!glfwInit()) {
        EXIT_WITH_ERROR("Failed to init GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #if _DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    #endif

    GLFWwindow *window = glfwCreateWindow(window_width, window_height, window_title.c_str(), nullptr, nullptr);
    if (!window) {
        EXIT_WITH_ERROR("Failed to init window");
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = true;
    if (GLEW_OK != glewInit()) {
        EXIT_WITH_ERROR("Failed to init GLEW");
    }

    glfwSetKeyCallback(window, keyCallback);
    #if _DEBUG
        // Register your callback function.
        glDebugMessageCallback(debugCallback, nullptr);
        // Enable synchronous callback. This ensures that your callback function is called
        // right after an error has occurred.
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    #endif

    if (!initFramework()) {
        EXIT_WITH_ERROR("Failed to init framework");
    }

    /* --------------------------------------------- */
    // Initialize scene and render loop
    /* --------------------------------------------- */
    {
        glClearColor(1, 1, 1, 1);

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            drawTeapot();
            glfwSwapBuffers(window);
            /* Gitlab CI automatic testing */
            if (argc > 1 && std::string(argv[1]) == "--run_headless") {
                saveScreenshot("screenshot", window_width, window_height);
                break;
            }
        }
    }

    /* --------------------------------------------- */
    // Destroy framework
    /* --------------------------------------------- */

    destroyFramework();

    /* --------------------------------------------- */
    // Destroy context and exit
    /* --------------------------------------------- */

    glfwTerminate();

    return EXIT_SUCCESS;
}