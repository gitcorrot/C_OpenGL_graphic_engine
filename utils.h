#ifndef _UTILS_H_
#define _UTILS_H_

#include "GL/gl3w.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

GLuint shaderCreateFromFile(char *vertexShaderFilename, char *fragmentShaderFilename);

#endif
 