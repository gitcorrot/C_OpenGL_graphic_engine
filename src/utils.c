#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

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
    vertexShaderTextBuffer = (GLchar *)malloc(sizeof(char) * textLength + 1);
    fseek(f, 0, SEEK_SET);
    if (fread(vertexShaderTextBuffer, 1, textLength, f) != textLength)
    {
        fprintf(stderr, "Can't read from '%s'\n", vertexShaderFilename);
    }
    vertexShaderTextBuffer[textLength] = '\0';
    const GLchar *vertexShaderText = vertexShaderTextBuffer;
    fclose(f);

    // Read fragment shader text
    f = fopen(fragmentShaderFilename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Can't open '%s'\n", fragmentShaderFilename);
        exit(EXIT_FAILURE);
    }
    fseek(f, 0, SEEK_END);
    textLength = ftell(f);
    fragmentShaderTextBuffer = (GLchar *)malloc(sizeof(char) * textLength + 1);
    fseek(f, 0, SEEK_SET);
    if (fread(fragmentShaderTextBuffer, 1, textLength, f) != textLength)
    {
        fprintf(stderr, "Can't read from '%s'\n", fragmentShaderFilename);
    }
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