#ifndef _INPUT_H_
#define _INPUT_H_

#include "../include/GL/gl3w.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "camera.h"

typedef struct
{
    GLFWwindow *window;
    CameraHandler *camera;

} InputHandler;


InputHandler* inputCreate(GLFWwindow *window, CameraHandler *camera);

void inputInit(InputHandler *self);
void inputUpdate(InputHandler *self, float deltaTime);

#endif
