#include <stdlib.h>
#include <math.h>

#include "camera.h"

CameraHandler *cameraCreate(float screenWidth, float screenHeight)
{
    CameraHandler *cameraHandler = (CameraHandler *)malloc(sizeof(CameraHandler));

    cameraHandler->screenWidth = screenWidth;
    cameraHandler->screenHeight = screenHeight;

    cameraHandler->cameraPosition[0] = 7.0;
    cameraHandler->cameraPosition[1] = 3.0;
    cameraHandler->cameraPosition[2] = 15.0;

    cameraHandler->cameraFront[0] = 0.0;
    cameraHandler->cameraFront[1] = 0.0;
    cameraHandler->cameraFront[2] = -1.0;

    cameraHandler->cameraUp[0] = 0.0;
    cameraHandler->cameraUp[1] = 1.0;
    cameraHandler->cameraUp[2] = 0.0;

    cameraHandler->pitch = 0.0;
    cameraHandler->yaw = -119.0;
    cameraHandler->fov = 45.0;

    cameraHandler->cameraSpeed = 0.01;

    return cameraHandler;
}

void cameraUpdate(CameraHandler *self)
{
    // Camera
    vec3f direction =
    {
        cosf(self->yaw * DEG2RAD) * cosf(self->pitch * DEG2RAD),
        sinf(self->pitch * DEG2RAD),
        sinf(self->yaw * DEG2RAD) * cosf(self->pitch * DEG2RAD)
    };

    vec3fNormalize(direction);
    vec3fCopy(direction, self->cameraFront);
}

void cameraGetViewMatrix(CameraHandler *self, mat4f dest)
{
    vec3f cameraTarget; // camera position + camera front
    vec3fCopy(self->cameraPosition, cameraTarget);
    vec3fAdd(cameraTarget, self->cameraFront);
    mat4fLookAt(dest, self->cameraPosition, cameraTarget);
}

void cameraGetPerspectiveMatrix(CameraHandler *self, mat4f dest)
{
    mat4fPerspective(dest, self->fov, self->screenWidth / self->screenHeight, 0.1, 100.0);
}

void cameraSetPitch(CameraHandler *self, float pitch)
{
    if (pitch >= 90.0)
        pitch = 89.0;
    else if (pitch <= -90.0)
        pitch = -89.0;

    self->pitch = pitch;
}

void cameraSetYaw(CameraHandler *self, float yaw)
{
    self->yaw = yaw;
}

void cameraSetFov(CameraHandler *self, float fov)
{
    if (fov < 1.0)
        fov = 1.0;
    else if (fov > 45.0)
        fov = 45.0;

    self->fov = fov;
}
