#define GL3W_IMPLEMENTATION
#include "../include/GL/gl3w.h" // https://github.com/gingerBill/gl3w-Single-File

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"
#include "mathOpengl.h"
#include "camera.h"
#include "input.h"

#include "model.h"
#include "model/defaultModel.h"

const float screenWidth = 1200.0;
const float screenHeight = 800.0;

float lastMillisTime, deltaTime;


int main(void)
{
    if (!glfwInit())
    {
        fprintf(stderr, "Can't initialize glfw");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    if (gl3w_init() != 0)
    {
        fprintf(stderr, "failed to initialize OpenGL\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(stdout, "OpenGL initialize succeeded\n");
    }

    if (gl3w_is_supported(3, 3) != 1)
    {
        fprintf(stderr, "OpenGL 3.3 not supported\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(stdout, "OpenGL 3.3 supported\n");
    }

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);

    CameraHandler *cameraHandler = cameraCreate(screenWidth, screenHeight);
    InputHandler *inputHandler = inputCreate(window, cameraHandler);
    inputInit(inputHandler);

    lastMillisTime = glfwGetTime() * 1000.0;
    deltaTime = lastMillisTime;

    defaultModel *dm = defaultModelCreate();
    Model *m = (Model *)dm; // upcast - first field of 'defaultModel' is base class 'Model'

    modelSetShader(m, shaderCreateFromFile(
                          "resources/shaders/model_vs.glsl",
                          "resources/shaders/model_fs.glsl"));

    modelLoad(m, "resources/models/stone.obj");
    modelScale(m, 2.0, 2.0, 2.0);

    while (glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2, 0.2, 0.3, 1);

        inputUpdate(inputHandler, deltaTime);

        mat4f view, perspective;
        cameraUpdate(cameraHandler);
        cameraGetViewMatrix(cameraHandler, view);
        cameraGetPerspectiveMatrix(cameraHandler, perspective);

        modelUpdateProjection(m, view, perspective);
        modelRotate(m, 0, 1, 0 , 1*DEG2RAD);
        modelRender(m);  

        glfwSwapBuffers(window);
        glfwPollEvents();

        GLenum err = glGetError();
        if(err != GL_NO_ERROR) {
            fprintf(stderr, "OpenGL error: %d", err);
        }

        double tempTime = (float)glfwGetTime() * 1000.0;
        deltaTime = tempTime - lastMillisTime;
        lastMillisTime = tempTime;
    }

    printf("Exiting...\n");

    // TODO: destroy every object
    
    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
