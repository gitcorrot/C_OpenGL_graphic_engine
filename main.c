#define GL3W_IMPLEMENTATION
#include "GL/gl3w.h" // https://github.com/gingerBill/gl3w-Single-File

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mathOpengl.h"

const int screen_width = 800;
const int screen_height = 600;

GLuint shaderCreateFromFile(char *vertex_shader_filename, char *fragment_shader_filename)
{
    FILE *f;
    GLchar *vertex_shader_text_buffer;
    GLchar *fragment_shader_text_buffer;

    // Read vertex shader text
    f = fopen(vertex_shader_filename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Can't open '%s'", vertex_shader_filename);
        exit(EXIT_FAILURE);
    }
    fseek(f, 0, SEEK_END);
    long text_length = ftell(f);
    vertex_shader_text_buffer = malloc(sizeof(char) * text_length + 1);
    fseek(f, 0, SEEK_SET);
    fread(vertex_shader_text_buffer, 1, text_length, f);
    vertex_shader_text_buffer[text_length] = '\0';
    const GLchar *vertex_shader_text = vertex_shader_text_buffer;
    fclose(f);

    // Read fragment shader text
    f = fopen(fragment_shader_filename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Can't open '%s'", fragment_shader_filename);
        exit(EXIT_FAILURE);
    }
    fseek(f, 0, SEEK_END);
    text_length = ftell(f);
    fragment_shader_text_buffer = malloc(sizeof(char) * text_length + 1);
    fseek(f, 0, SEEK_SET);
    fread(fragment_shader_text_buffer, 1, text_length, f);
    fragment_shader_text_buffer[text_length] = '\0';
    const GLchar *fragment_shader_text = fragment_shader_text_buffer;
    fclose(f);

    char info_log_buffer[512];
    int success;

    // Create vertex shader
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderID, 1, &vertex_shader_text, NULL);
    glCompileShader(VertexShaderID);
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(VertexShaderID, 512, NULL, info_log_buffer);
        fprintf(stderr,
                "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s",
                info_log_buffer);
    };

    // Create fragment shader
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderID, 1, &fragment_shader_text, NULL);
    glCompileShader(FragmentShaderID);
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(FragmentShaderID, 512, NULL, info_log_buffer);
        fprintf(stderr,
                "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s",
                info_log_buffer);
    };

    // Attach and link shaders to program
    GLuint p = glCreateProgram();
    glAttachShader(p, VertexShaderID);
    glAttachShader(p, FragmentShaderID);
    glLinkProgram(p);
    glGetProgramiv(p, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(p, 512, NULL, info_log_buffer);
        fprintf(stderr,
                "ERROR::ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s",
                info_log_buffer);
    }

    // Clean up
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    free(vertex_shader_text_buffer);
    free(fragment_shader_text_buffer);

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

    GLFWwindow *window = glfwCreateWindow(screen_width, screen_height, "OpenGL test", NULL, NULL);
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

    GLfloat vertices[] = {
        // position             // color
        -0.5f,  0.5f, 0.0f,     0.9f, 0.0f, 0.0f,   // 0
        -0.5f, -0.5f, 0.0f,     0.0f, 0.9f, 0.0f,   // 1
         0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 0.9f,   // 2
         0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 0.1f,   // 3
         0.0f,  0.9f, 0.0f,     1.0f, 1.0f, 1.0f,   // 4
    };

    GLuint indices[] = {
         0, 1, 2,
         2, 0, 3,
         0, 3, 4
    };

    GLuint VBO, VAO, EBO; // EBO - element array buffer

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

    double time, previousTime = 0;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    while (
        glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.25, 0.25, 0.2, 1);
        glUseProgram(programID);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,
                       sizeof(indices) / sizeof(indices[0]),
                       GL_UNSIGNED_INT,
                       (void *)0); 

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Count FPS
        time = glfwGetTime();
        if(((int)(time*10000) % 10) == 0 )
            fprintf(stdout, "\nFPS: %f", 1/(time - previousTime));
        previousTime = time;
    }


    printf("Exiting...\n");
    glfwDestroyWindow(window);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(programID);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
