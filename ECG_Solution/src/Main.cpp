/*
* Copyright 2021 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include "Utils.h"
#include "Callbacks.h"
#include "Renderer.h"
#include "Shader.h"
#include "Transform.h"
#include "Camera.h"
#include "Drawables/Sphere.h"
#include "Drawables/Cylinder.h"
#include "Drawables/Cube.h"
#include "Drawables/Torus.h"
#include "Vertex.h"
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"

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

    double camera_fov = reader.GetReal("camera", "fov", 60) * M_PI / 180.0;
    double camera_near = reader.GetReal("camera", "near", 0.1);
    double camera_far = reader.GetReal("camera", "far", 100);

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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

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
    // Initialize scene and draw loop
    /* --------------------------------------------- */
    {
        glClearColor(1, 1, 1, 1);

        double lastTime = glfwGetTime();
        int nbFrames = 0;

        Camera camera(window, camera_fov, (double)window_width / (double)window_height, camera_near, camera_far);

        Renderer renderer;
        glm::mat4 viewMatrix;

        Cube cube(1.5f, 1.5f, 1.5f);
        Sphere sphere1(32, 16, 1.0f);
        Sphere sphere2(32, 16, 1.0f);
        Cylinder cylinder(16, 1.5f, 1.0f);

        PointLight pointLight1({0, 0, 0}, {1, 1, 1}, {1.0f, 0.4f, 0.1f});
        DirectionalLight directionalLight1({0, -1, -1}, {0.8f, 0.8f, 0.8f});

        Shader cubeShader("assets/shaders/gouraud.vsh", "assets/shaders/gouraud.fsh");
        Shader sphere1Shader("assets/shaders/phong.vsh", "assets/shaders/phong.fsh");
        Shader sphere2Shader("assets/shaders/gouraud.vsh", "assets/shaders/gouraud.fsh");
        Shader cylinderShader("assets/shaders/phong.vsh", "assets/shaders/phong.fsh");

        Transform cubeTransform;
        cubeTransform.translate(-1.2f, -1.5f, 0);
        Transform sphere1Transform;
        sphere1Transform.translate(-1.2f, 1.0f, 0);
        Transform sphere2Transform;
        sphere2Transform.translate(1.2f, 1.0f, 0);
        Transform cylinderTransform;
        cylinderTransform.translate(1.2f, -1.5f, 0);

        while (!glfwWindowShouldClose(window)) {
            renderer.clear();
            glfwPollEvents();

            #if _DEBUG
                double currentTime = glfwGetTime();
                nbFrames++;
                if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
                    // printf and reset timer
                    printf("%f ms/frame, %d frames\n", 1000.0/double(nbFrames), nbFrames);
                    nbFrames = 0;
                    lastTime += 1.0;
                }
            #endif

            viewMatrix = camera.getTransformMatrix();

            cubeShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, viewMatrix);
            cubeShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, cubeTransform.getMatrix());
            cubeShader.setUniform4f("inColor", 1.0f, 0.0f, 0.0f, 1.0f);
            cubeShader.setUniform1f("ka", 0.05f);
            cubeShader.setUniform1f("kd", 0.8f);
            cubeShader.setUniform1f("ks", 0.5f);
            cubeShader.setUniform1i("alpha", 5);
            cubeShader.setUniform3f("eyePos", camera.pos.x, camera.pos.y, camera.pos.z);
            cubeShader.setUniformPointLight("pointLight", pointLight1);
            cubeShader.setUniformDirectionalLight("directionalLight", directionalLight1);
            renderer.renderDrawable(cubeShader, cube);

            sphere1Shader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, viewMatrix);
            sphere1Shader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, sphere1Transform.getMatrix());
            sphere1Shader.setUniform4f("inColor", 0.0f, 1.0f, 0.0f, 1.0f);
            sphere1Shader.setUniform1f("ka", 0.1f);
            sphere1Shader.setUniform1f("kd", 0.9f);
            sphere1Shader.setUniform1f("ks", 0.3f);
            sphere1Shader.setUniform1i("alpha", 10);
            sphere1Shader.setUniform3f("eyePos", camera.pos.x, camera.pos.y, camera.pos.z);
            sphere1Shader.setUniformPointLight("pointLight", pointLight1);
            sphere1Shader.setUniformDirectionalLight("directionalLight", directionalLight1);
            renderer.renderDrawable(sphere1Shader, sphere1);

            sphere2Shader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, viewMatrix);
            sphere2Shader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, sphere2Transform.getMatrix());
            sphere2Shader.setUniform4f("inColor", 1.0f, 0.0f, 0.0f, 1.0f);
            sphere2Shader.setUniform1f("ka", 0.1f);
            sphere2Shader.setUniform1f("kd", 0.9f);
            sphere2Shader.setUniform1f("ks", 0.3f);
            sphere2Shader.setUniform1i("alpha", 10);
            sphere2Shader.setUniform3f("eyePos", camera.pos.x, camera.pos.y, camera.pos.z);
            sphere2Shader.setUniformPointLight("pointLight", pointLight1);
            sphere2Shader.setUniformDirectionalLight("directionalLight", directionalLight1);
            renderer.renderDrawable(sphere2Shader, sphere2);

            cylinderShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, viewMatrix);
            cylinderShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, cylinderTransform.getMatrix());
            cylinderShader.setUniform4f("inColor", 0.0f, 1.0f, 0.0f, 1.0f);
            cylinderShader.setUniform1f("ka", 0.05f);
            cylinderShader.setUniform1f("kd", 0.8f);
            cylinderShader.setUniform1f("ks", 0.5f);
            cylinderShader.setUniform1i("alpha", 5);
            cylinderShader.setUniform3f("eyePos", camera.pos.x, camera.pos.y, camera.pos.z);
            cylinderShader.setUniformPointLight("pointLight", pointLight1);
            cylinderShader.setUniformDirectionalLight("directionalLight", directionalLight1);
            renderer.renderDrawable(cylinderShader, cylinder);

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