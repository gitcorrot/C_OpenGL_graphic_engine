#include <stdlib.h>
#include <stdio.h>

#include "input.h"
#include "consts.h"

double lastX, lastY, lastScroll;
float xOff, yOff, scrollOff;

const static float mouseSensitivity = 0.15;
const static float mouseScrollSensitivity = 5.0;

void mouseCallback(GLFWwindow *window, double xPos, double yPos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

InputHandler *inputCreate(GLFWwindow *window, CameraHandler *camera)
{
    InputHandler *inputHandler = (InputHandler *)malloc(sizeof(InputHandler));
    // TODO: check exception
    inputHandler->window = window;
    inputHandler->camera = camera;

    return inputHandler;
}

void inputInit(InputHandler *self)
{
    glfwSetInputMode(self->window, GLFW_STICKY_KEYS, GL_TRUE);
    // glfwSetInputMode(self->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetCursorPosCallback(self->window, mouseCallback);
    glfwSetMouseButtonCallback(self->window, mouseButtonCallback);
    glfwSetScrollCallback(self->window, scrollCallback);

    GLFWcursor *cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    glfwSetCursor(self->window, cursor);
}

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
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            xOff = (float)xPos - lastX;
            yOff = (float)yPos - lastY;

            lastX = xPos;
            lastY = yPos;
        }
    }
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &lastX, &lastY);
    }
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    scrollOff = (float)yoffset * mouseScrollSensitivity;
}

void inputUpdate(InputHandler *self, float deltaTime)
{
    // Mouse XY
    float tempYaw = self->camera->yaw - xOff * mouseSensitivity;
    float tempPitch = self->camera->pitch + yOff * mouseSensitivity;
    cameraSetYaw(self->camera, tempYaw);
    cameraSetPitch(self->camera, tempPitch);
    xOff = 0;
    yOff = 0;

    // Mouse Scroll
    float tempFov = self->camera->fov - scrollOff;
    cameraSetFov(self->camera, tempFov);
    scrollOff = 0;

    // Q
    if (glfwGetKey(self->window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(self->window, GL_TRUE);
    }
    else
    {
        const float cameraSpeed = CAMERA_SPEED * deltaTime;
        // W
        if (glfwGetKey(self->window, GLFW_KEY_W) == GLFW_PRESS)
        {
            vec3f tmp;
            vec3fMultiplyScalar(tmp, self->camera->cameraFront, cameraSpeed);
            vec3fAdd(self->camera->cameraPosition, self->camera->cameraPosition, tmp);
        }
        // S
        if (glfwGetKey(self->window, GLFW_KEY_S) == GLFW_PRESS)
        {
            vec3f tmp;
            vec3fMultiplyScalar(tmp, self->camera->cameraFront, cameraSpeed);
            vec3fSubtract(self->camera->cameraPosition, self->camera->cameraPosition, tmp);
        }
        // A
        if (glfwGetKey(self->window, GLFW_KEY_A) == GLFW_PRESS)
        {
            vec3f tmp;
            vec3fCrossProduct(tmp, self->camera->cameraFront, self->camera->cameraUp); // right vector
            vec3fNormalize(tmp);
            vec3fMultiplyScalar(tmp, tmp, cameraSpeed);
            vec3fSubtract(self->camera->cameraPosition, self->camera->cameraPosition, tmp);
        }
        // D
        if (glfwGetKey(self->window, GLFW_KEY_D) == GLFW_PRESS)
        {
            vec3f tmp;
            vec3fCrossProduct(tmp, self->camera->cameraFront, self->camera->cameraUp); // right vector
            vec3fNormalize(tmp);
            vec3fMultiplyScalar(tmp, tmp, cameraSpeed);
            vec3fAdd(self->camera->cameraPosition, self->camera->cameraPosition, tmp);
        }
        // Space
        if (glfwGetKey(self->window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            vec3f tmp;
            tmp[1] = 1.0 * cameraSpeed;
            vec3fAdd(self->camera->cameraPosition, self->camera->cameraPosition, tmp);
        }
        // Shift
        if (glfwGetKey(self->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            vec3f tmp;
            tmp[1] = -1.0 * cameraSpeed;
            vec3fAdd(self->camera->cameraPosition, self->camera->cameraPosition, tmp);
        }
    }
}
