#ifndef _CUBE_H_
#define _CUBE_H_

#include "../include/GL/gl3w.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "mathOpengl.h"

typedef struct Cube
{
    GLuint objectID;
    GLuint programID;
    // GLuint VAO, VBO, EBO;
    mat4f translation;
    mat4f rotation;
    mat4f scale;

} Cube;

Cube *cubeCreate();

void cubeInit();
void cubeSetPosition(Cube *self, vec3f position);
void cubeTranslate(Cube *self, float x, float y, float z);
void cubeRotate(Cube *self, float x, float y, float z, float theta);
void cubeScale(Cube *self, float x, float y, float z);
void cubeRender(Cube *self);
void cubeDestroy(Cube *self);
void cubePrint(Cube *self);

#endif