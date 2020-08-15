#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "mathOpengl.h"

typedef struct
{
    float screenWidth, screenHeight;
    vec3f cameraPosition;
    vec3f cameraFront;
    vec3f cameraUp;
    float pitch;
    float yaw;
    float fov;

} CameraHandler;

CameraHandler *cameraCreate(float screenWidth, float screenHeight);

void cameraUpdate(CameraHandler *self);
void cameraGetViewMatrix(CameraHandler *self, mat4f dest);
void cameraGetPerspectiveMatrix(CameraHandler *self, mat4f dest);

void cameraSetPitch(CameraHandler *self, float pitch);
void cameraSetYaw(CameraHandler *self, float yaw);
void cameraSetFov(CameraHandler *self, float fov);

#endif