#define GL3W_IMPLEMENTATION
#include "../include/GL/gl3w.h" // https://github.com/gingerBill/gl3w-Single-File

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "consts.h"
#include "utils.h"
#include "mathOpengl.h"
#include "camera.h"
#include "input.h"
#include "terrain.h"

#include "model.h"
#include "model/vtnModel.h"
#include "model/vModel.h"

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

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME, NULL, NULL);
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

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); 

    CameraHandler *cameraHandler = cameraCreate(SCREEN_WIDTH, SCREEN_HEIGHT);
    InputHandler *inputHandler = inputCreate(window, cameraHandler);
    inputInit(inputHandler);

    float lastMillisTime = glfwGetTime() * 1000.0;
    float deltaTime = lastMillisTime;

    vec3f sunPosition = { 30.0, 80.0, 30.0 };

    Model *models[1];

    Model *sunModel = vModelCreate();
    models[0] = sunModel;
    modelSetShader(sunModel, shaderCreateFromFile(
                          "resources/shaders/light_vs.glsl",
                          "resources/shaders/light_fs.glsl"));
    modelLoad(sunModel, "resources/models/sun.obj");
    modelTranslate(sunModel, sunPosition[0], sunPosition[1], sunPosition[2]);
    modelScale(sunModel, 5.0, 5.0, 5.0);

    // Shader *rockShader = shaderCreateFromFile(
    //                         "resources/shaders/model_vs.glsl",
    //                         "resources/shaders/model_fs.glsl");
    // for (int i = 1; i < sizeof(models)/sizeof(models[0]); i++) {
    //     Model *rockModel = vtnModelCreate();
    //     models[i] = rockModel;
    //     modelSetShader(rockModel, rockShader);
    //     modelLoad(rockModel, "resources/models/stone.obj");
    //     modelTranslate(rockModel, rand()%15-5, rand()%15-5, rand()%15-5);
    //     // modelTranslate(rockModel, 0, 15, -15);
    //     modelRotate(rockModel, 1,0,0, rand()%180);
    //     modelScale(rockModel, -(float)(rand()%5)/10.0, -(rand()%5)/10.0, -(rand()%5)/10.0);
    // } 



    Terrain *terrain = terrainCreateRandom();
    // tilePrint(terrain->tiles);
    


    while (glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2, 0.2, 0.3, 1);

        inputUpdate(inputHandler, deltaTime);

        mat4f view, perspective;
        cameraUpdate(cameraHandler);
        cameraGetViewMatrix(cameraHandler, view);
        cameraGetPerspectiveMatrix(cameraHandler, perspective);


        terrainRender(terrain, view, perspective, sunPosition);

        for (int i = 0; i < sizeof(models)/sizeof(models[0]); i++) {
            modelRender(models[i], view, perspective, sunPosition);  
        }

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

    // Destroy every object
        for (int i = 0; i < sizeof(models)/sizeof(models[0]); i++) {
            modelDestroy(models[i]);  
        }
    
    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
