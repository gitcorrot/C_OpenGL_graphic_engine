    // TODO:
    // create input library:
    //  - in keyCallback() function only sets flags
    //  - update() function that checks flags and do what is needed (or updateMouseInput(), updateKeyboardInput())
    // or:
    //  - make class (OOB in C) InputManager and place the functions there
    // or:
    //  - only use glfwGetKey(window,key)

    // crate Camera class/library
    //  - s


#define GL3W_IMPLEMENTATION
#include "GL/gl3w.h" // https://github.com/gingerBill/gl3w-Single-File

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"
#include "mathOpengl.h"
#include "cube.h"

const float screenWidth = 1200.0;
const float screenHeight = 800.0;

vec3f cameraPosition =  { 7.0,  3.0,  15.0 };
vec3f cameraFront =     { 0.0,  0.0, -1.0 };
vec3f cameraUp =        { 0.0,  1.0,  0.0 };
float pitch = 0.0;
float yaw = -119.0;
float fov = 45.0;

double lastX = 0.0;
double lastY = 0.0;
double lastScroll;

double lastMillisTime, deltaTime;

void mouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    if (lastX == 0 && lastY == 0)
    {
        lastX = xPos;
        lastY = yPos;
        return;
    }
    else
    {
        // printf("yaw: %f, pitch: %f\n", yaw, pitch);
        const float mouseSensitivity = 0.1;

        yaw += (xPos - lastX) * mouseSensitivity;
        pitch -= (yPos - lastY) * mouseSensitivity;

        lastX = xPos;
        lastY = yPos;

        if (pitch >= 90.0)
            pitch = 89.0;
        else if (pitch <= -90.0)
            pitch = -89.0;
    }
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    fov -= yoffset;

    if (fov < 1.0)
        fov = 1.0;
    else if (fov > 45.0)
        fov = 45.0;
}

void processKeyboardInput(GLFWwindow *window)
{
    // Q
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else
    {
        const float cameraSpeed = 0.01 * deltaTime;
        // W
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            vec3f tmp;
            vec3fMultiplyScalar(tmp, cameraFront, cameraSpeed);
            vec3fAdd(cameraPosition, tmp);
        }
        // S
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            vec3f tmp;
            vec3fMultiplyScalar(tmp, cameraFront, cameraSpeed);
            vec3fSubtract(cameraPosition, tmp);
        }
        // A
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            vec3f tmp;
            vec3fCrossProduct(tmp, cameraFront, cameraUp); // right vector
            vec3fNormalize(tmp);
            vec3fMultiplyScalar(tmp, tmp, cameraSpeed);
            vec3fSubtract(cameraPosition, tmp);
        }
        // D
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            vec3f tmp;
            vec3fCrossProduct(tmp, cameraFront, cameraUp); // right vector
            vec3fNormalize(tmp);
            vec3fMultiplyScalar(tmp, tmp, cameraSpeed);
            vec3fAdd(cameraPosition, tmp);
        }
    }
}

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

    // Input configuration
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    // glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);

    vec3f axisVertices[] = 
    {
        // axis
        { 0.0f,  0.0f,  0.0f },  {0.7f, 0.7f, 0.7f },  // 8
        { 0.2f,  0.2f,  0.0f },  {0.7f, 0.7f, 0.7f },  // 9
        { 10.0f, 0.0f,  0.0f },  {1.0f, 0.0f, 0.0f },  // x axis // 10
        { 0.0f,  10.0f, 0.0f },  {0.0f, 1.0f, 0.0f },  // y axis // 11
        { 0.0f,  0.0f,  10.0f},  {0.0f, 0.0f, 1.0f },  // z axis // 12
    };

    GLuint axisIndices[] = 
    {
        0, 2, 1, // axis x
        0, 3, 1, // axis y
        0, 4, 1  // axis z
    };

    GLuint axisVAO;
    GLuint axisVBO;
    GLuint axisEBO;

    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);
    glGenBuffers(1, &axisEBO);

    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);
    // Positions (first 3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    // Colors (second 3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axisEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(axisIndices), axisIndices, GL_STATIC_DRAW);

    // Load shaders
    GLuint programID = shaderCreateFromFile("test_vs.glsl", "test_fs.glsl");

    // View matrix
    mat4f view;

    // Projection matrix
    mat4f perspective;

    // Setup time
    lastMillisTime = glfwGetTime() * 1000.0;
    deltaTime = lastMillisTime;

    // Generate cubes
    vec3f cubePositions[] = 
    {
        {-1.5,  3.5, -2.0},
        { 1.5,  2.5, -4.0},
        { 0.0,  1.0, -6.0},
        { 2.5,  2.0, -8.0}
    };

    Cube *cubes[4];
    cubeInit();
    for (int i = 0; i < 4; i++)
    {
        cubes[i] = cubeCreate();
        cubeSetPosition(cubes[i], cubePositions[i]);
        cubePrint(cubes[i]);
    }

    while (glfwWindowShouldClose(window) == 0)
    {
        processKeyboardInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.15, 1);

        // draw every cube
        for (int i = 0; i < 4; i++) 
        {
            cubeTranslate(cubes[i], 0, 0, 0.025);
            cubeRotate(cubes[i], 0, 1, 0, 5*DEG2RAD);
            // cubeScale(cubes[i], 0.01, 0.01, 0.01);
            cubeRender(cubes[i]);
        }

        // Render axis
        glBindVertexArray(axisVAO);
        mat4f axisTranslation;
        mat4fIdentity(axisTranslation);
        GLint uniModel = glGetUniformLocation(programID, "model");
        glUniformMatrix4fv(uniModel, 1, GL_TRUE, axisTranslation);

        glDrawElements(GL_TRIANGLES,
                       sizeof(axisIndices) / sizeof(axisIndices[0]),
                       GL_UNSIGNED_INT,
                       (void *)0);
                       

        // Camera
        vec3f direction = 
        {
            cosf(yaw*DEG2RAD) * cosf(pitch*DEG2RAD),
            sinf(pitch*DEG2RAD),
            sinf(yaw*DEG2RAD) * cosf(pitch*DEG2RAD)
        };
        vec3fNormalize(direction);
        vec3fCopy(direction, cameraFront);

        vec3f cameraTarget; // camera position + camera front
        vec3fCopy(cameraPosition, cameraTarget);
        vec3fAdd(cameraTarget, cameraFront); 
        mat4fLookAt(view, cameraPosition, cameraTarget);
        GLint uniView = glGetUniformLocation(programID, "view");
        glUniformMatrix4fv(uniView, 1, GL_TRUE, view);

        mat4fPerspective(perspective, fov, screenWidth / screenHeight, 0.1, 100.0);
        GLint uniProj = glGetUniformLocation(programID, "projection");
        glUniformMatrix4fv(uniProj, 1, GL_TRUE, perspective);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Update delta time
        double tempTime = glfwGetTime() * 1000.0;
        deltaTime = tempTime - lastMillisTime;
        lastMillisTime = tempTime;
        // printf("dt = %f\n", deltaTime);
    }

    printf("Exiting...\n");
    glfwDestroyWindow(window);
    glDeleteVertexArrays(1, &axisVAO);
    glDeleteBuffers(1, &axisVBO);
    glDeleteBuffers(1, &axisEBO);
    glDeleteProgram(programID);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
