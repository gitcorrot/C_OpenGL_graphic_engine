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
#include <string.h>

#include "mathOpengl.h"

const float screenWidth = 1200.0;
const float screenHeight = 800.0;

float cameraX = 0.0;
float cameraY = 0.0;
float cameraZ = -5.0;

double lastX, lastY, lastScroll;

double lastMillisTime, deltaTime;

GLuint shaderCreateFromFile(char *vertexShaderFilename, char *fragmentShaderFilename)
{
    FILE *f;
    GLchar *vertexShaderTextBuffer;
    GLchar *fragmentShaderTextBuffer;

    // Read vertex shader text
    f = fopen(vertexShaderFilename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Can't open '%s'", vertexShaderFilename);
        exit(EXIT_FAILURE);
    }
    fseek(f, 0, SEEK_END);
    long textLength = ftell(f);
    vertexShaderTextBuffer = malloc(sizeof(char) * textLength + 1);
    fseek(f, 0, SEEK_SET);
    fread(vertexShaderTextBuffer, 1, textLength, f);
    vertexShaderTextBuffer[textLength] = '\0';
    const GLchar *vertexShaderText = vertexShaderTextBuffer;
    fclose(f);

    // Read fragment shader text
    f = fopen(fragmentShaderFilename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Can't open '%s'", fragmentShaderFilename);
        exit(EXIT_FAILURE);
    }
    fseek(f, 0, SEEK_END);
    textLength = ftell(f);
    fragmentShaderTextBuffer = malloc(sizeof(char) * textLength + 1);
    fseek(f, 0, SEEK_SET);
    fread(fragmentShaderTextBuffer, 1, textLength, f);
    fragmentShaderTextBuffer[textLength] = '\0';
    const GLchar *fragmentShaderText = fragmentShaderTextBuffer;
    fclose(f);

    char infoLogBuffer[512];
    int success;

    // Create vertex shader
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderID, 1, &vertexShaderText, NULL);
    glCompileShader(VertexShaderID);
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(VertexShaderID, 512, NULL, infoLogBuffer);
        fprintf(stderr,
                "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s",
                infoLogBuffer);
    };

    // Create fragment shader
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderID, 1, &fragmentShaderText, NULL);
    glCompileShader(FragmentShaderID);
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(FragmentShaderID, 512, NULL, infoLogBuffer);
        fprintf(stderr,
                "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s",
                infoLogBuffer);
    };

    // Attach and link shaders to program
    GLuint p = glCreateProgram();
    glAttachShader(p, VertexShaderID);
    glAttachShader(p, FragmentShaderID);
    glLinkProgram(p);
    glGetProgramiv(p, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(p, 512, NULL, infoLogBuffer);
        fprintf(stderr,
                "ERROR::ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s",
                infoLogBuffer);
    }

    // Clean up
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    free(vertexShaderTextBuffer);
    free(fragmentShaderTextBuffer);

    return p;
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (lastX == 0 && lastY == 0)
    {
        lastX = xPos;
        lastY = yPos;
        return;
    }

    if (lastX < xPos)
    {
        // mouse moved to the right
    }
    else if (lastX > xPos)
    {
        // mouse moved to the left
    }

    if (lastY < yPos)
    {
        // mouse moved to the top
    }
    else if (lastY > yPos)
    {
        // mouse moved to the bottom
    }

    lastX = xPos;
    lastY = yPos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (lastScroll < yoffset)
    {
        cameraZ += 0.5;
    }
    else if (lastScroll > yoffset)
    {
        cameraZ -= 0.5;
    }   
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Q
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // W
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        cameraY -= 0.05;
    }
    // S
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        cameraY += 0.05;
    }
    // A
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        cameraX += 0.05;
    }
    // D
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        cameraX -= 0.05;
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
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);

    GLuint indices[] = {
        // front
        2, 1, 0,
        0, 3, 2,
        // back
        6, 5, 4,
        4, 7, 6,
        // left 
        1, 5, 4,
        4, 0, 1,
        // right
        6, 2, 3,
        3, 7, 6,
        // top
        3, 0, 4,
        4, 7, 3, 
        // bottom
        2, 1, 5,
        5, 6, 2
    };

    vec3f vertices[] = 
    {
        // position             // color
        {-0.5f,  0.5f, 0.5f},   {0.9f, 0.0f, 0.0f },   // 0
        {-0.5f, -0.5f, 0.5f},   {0.0f, 0.9f, 0.0f },   // 1
        { 0.5f, -0.5f, 0.5f},   {0.0f, 1.0f, 0.9f },   // 2
        { 0.5f,  0.5f, 0.5f},   {1.0f, 1.0f, 1.0f },   // 3
        
        {-0.5f,  0.5f, -0.5f},   {0.9f, 0.5f, 0.0f },   // 4
        {-0.5f, -0.5f, -0.5f},   {0.0f, 0.9f, 1.0f },   // 5
        { 0.5f, -0.5f, -0.5f},   {0.7f, 0.0f, 0.9f },   // 6
        { 0.5f,  0.5f, -0.5f},   {1.0f, 1.0f, 1.0f },   // 7
    };

    // TODO: implement vec3i (integer)
    // vec3f indices[] = 
    // {
    //     {0, 1, 2},
    //     {2, 0, 3},
    //     {0, 3, 4}
    // };

    GLuint VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Positions (first 3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    // Colors (second 3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Load shaders
    GLuint programID = shaderCreateFromFile("test_vs.glsl", "test_fs.glsl");

    vec3f modelsPositions[4] = 
    {
        {-1.5,  1.5, -2.0},
        { 1.5, -1.5, -4.0},
        { 0.0,  0.0, -6.0},
        { 2.5,  2.5, -8.0}
    };

    // Model matrix
    mat4f translation, rotation;

    // View matrix
    mat4f view;

    // Projection matrix
    mat4f perspective;
    mat4fPerspective(perspective, 45.0, screenWidth / screenHeight, 0.1, 100.0);

    // Setup time
    lastMillisTime = glfwGetTime() * 1000.0;
    deltaTime = lastMillisTime;

    while (glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.15, 1);

        glUseProgram(programID);

        uint8_t modelsCount = sizeof(modelsPositions)/sizeof(modelsPositions[0]);
        for (int i = 0; i < modelsCount; i++) 
        {

            mat4fTranslate(translation,
                           modelsPositions[i][0],
                           modelsPositions[i][1],
                           modelsPositions[i][2]);
            if (i % 2 == 0)
                mat4fRotate(rotation, 1, 0, 0, i * 50.0 + (lastMillisTime * DEG2RAD / 5.0));
            else
                mat4fRotate(rotation, 0, 1, 0, i * 50.0 + (lastMillisTime * DEG2RAD / 5.0));

            mat4fMultiply(translation, rotation);

            GLint uniModel = glGetUniformLocation(programID, "model");
            glUniformMatrix4fv(uniModel, 1, GL_TRUE, translation);

            glDrawElements(GL_TRIANGLES,
                           sizeof(indices) / sizeof(indices[0]),
                           GL_UNSIGNED_INT,
                           (void *)0);
        }

        mat4fTranslate(view, cameraX, cameraY, cameraZ);
        GLint uniView = glGetUniformLocation(programID, "view");
        glUniformMatrix4fv(uniView, 1, GL_TRUE, view);

        GLint uniProj = glGetUniformLocation(programID, "projection");
        glUniformMatrix4fv(uniProj, 1, GL_TRUE, perspective);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Update delta time
        double tempTime = glfwGetTime() * 1000.0;
        deltaTime = tempTime - lastMillisTime;
        lastMillisTime = tempTime;
        printf("dt = %f\n", deltaTime);
    }

    printf("Exiting...\n");
    glfwDestroyWindow(window);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(programID);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
