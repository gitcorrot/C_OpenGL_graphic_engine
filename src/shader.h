#ifndef _SHADER_H_
#define _SHADER_H_

#include "../include/GL/gl3w.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "mathOpengl.h"

typedef struct
{
    GLuint programID;

} Shader;

Shader *shaderCreateFromFile(char *vertexShaderFilename, char *fragmentShaderFilename);

void shaderActivate(Shader *self);
void shaderSetUniformFloat(Shader *self, char *name, float value);
void shaderSetUniformMat4(Shader *self, char *name, mat4f value);
void shaderSetUniformVec3(Shader *self, char *name, vec3f value);
void shaderDestroy(Shader *self);

#endif