#define GL3W_IMPLEMENTATION
#include "GL/gl3w.h" // https://github.com/gingerBill/gl3w-Single-File

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mathOpengl.h"

const int screenWidth = 800;
const int screenHeight = 800;

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


    GLuint indices[] = {
         0, 1, 2,
         2, 0, 3
    };

    vec3f vertices[] = 
    {
        // position             // color
        {-0.3f,  0.3f, 0.0f},   {0.9f, 0.0f, 0.0f },   // 0
        {-0.3f, -0.3f, 0.0f},   {0.0f, 0.9f, 0.0f },   // 1
        { 0.3f, -0.3f, 0.0f},   {0.0f, 0.0f, 0.9f },   // 2
        { 0.3f,  0.3f, 0.0f},   {1.0f, 1.0f, 1.0f },   // 3
    };

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    // Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Load shaders
    GLuint programID = shaderCreateFromFile("test_vs.glsl", "test_fs.glsl");

    // double time, previousTime = 0;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    int count = 0;
    float angle = 0.0;
    
    mat4f translation, rotation;
    mat4fIdentity(translation);
    mat4fIdentity(rotation);

    while (
        glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.25, 0.25, 0.2, 1);
        glUseProgram(programID);

        glBindVertexArray(VAO);

        //      Model transformation    //
        angle = (count++ % 360) * DEG2RAD;
        mat4fTranslate(translation, cosf(angle) / 2, sinf(angle) / 2, 0.0);
        mat4fRotate(rotation, 0, 0, 1, angle * 2);
        mat4fMultiply(translation, rotation);
        GLint uniTrans = glGetUniformLocation(programID, "model");
        glUniformMatrix4fv(uniTrans, 1, GL_TRUE, translation);
        // end of Model transformation  //

        glDrawElements(GL_TRIANGLES,
                       sizeof(indices) / sizeof(indices[0]),
                       GL_UNSIGNED_INT,
                       (void *)0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Count FPS
        // time = glfwGetTime();
        // if(((int)(time*100000) % 10) == 0 )
        //     fprintf(stdout, "FPS: %.1f\n", 1/(time - previousTime));
        // previousTime = time;
    }

    printf("Exiting...\n");
    glfwDestroyWindow(window);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(programID);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
