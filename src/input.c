#include <stdlib.h>
#include "input.h"

float lastX, lastY, lastScroll;
float xOff, yOff, scrollOff;

const static float mouseSensitivity = 0.1;
const static float mouseScrollSensitivity = 5.0;

void mouseCallback(GLFWwindow *window, double xPos, double yPos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

InputHandler* inputCreate(GLFWwindow *window, CameraHandler *camera)
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
    glfwSetInputMode(self->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(self->window, mouseCallback);
    glfwSetScrollCallback(self->window, scrollCallback);
}

void mouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    if (lastX == 0 && lastY == 0)
    {
        lastX = (float)xPos;
        lastY = (float)yPos;
        return;
    }
    else
    {
        xOff = (float)xPos - lastX;
        yOff = (float)yPos - lastY;

        lastX = (float)xPos;
        lastY = (float)yPos;
    }
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    scrollOff = (float)yoffset * mouseScrollSensitivity;
}

void inputUpdate(InputHandler *self, float deltaTime)
{
    // Mouse XY
    float tempYaw = self->camera->yaw + xOff * mouseSensitivity;
    float tempPitch = self->camera->pitch - yOff * mouseSensitivity;
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
        const float cameraSpeed = self->camera->cameraSpeed * deltaTime;
        // W
        if (glfwGetKey(self->window, GLFW_KEY_W) == GLFW_PRESS)
        {
            vec3f tmp;
            vec3fMultiplyScalar(tmp, self->camera->cameraFront, cameraSpeed);
            vec3fAdd(self->camera->cameraPosition, tmp);
        }
        // S
        if (glfwGetKey(self->window, GLFW_KEY_S) == GLFW_PRESS)
        {
            vec3f tmp;
            vec3fMultiplyScalar(tmp, self->camera->cameraFront, cameraSpeed);
            vec3fSubtract(self->camera->cameraPosition, tmp);
        }
        // A
        if (glfwGetKey(self->window, GLFW_KEY_A) == GLFW_PRESS)
        {
            vec3f tmp;
            vec3fCrossProduct(tmp, self->camera->cameraFront, self->camera->cameraUp); // right vector
            vec3fNormalize(tmp);
            vec3fMultiplyScalar(tmp, tmp, cameraSpeed);
            vec3fSubtract(self->camera->cameraPosition, tmp);
        }
        // D
        if (glfwGetKey(self->window, GLFW_KEY_D) == GLFW_PRESS)
        {
            vec3f tmp;
            vec3fCrossProduct(tmp, self->camera->cameraFront, self->camera->cameraUp); // right vector
            vec3fNormalize(tmp);
            vec3fMultiplyScalar(tmp, tmp, cameraSpeed);
            vec3fAdd(self->camera->cameraPosition, tmp);
        }
    }
}
