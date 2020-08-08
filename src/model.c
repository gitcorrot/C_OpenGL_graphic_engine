#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mathOpengl.h"
#include "model.h"
#include "utils.h"


Model *modelCreate()
{
    Model *model = (Model *)malloc(sizeof(Model));
    model->modelID = rand();

    return model;
}

void modelLoad(Model *self, char *path)
{
    self->vtable->modelLoad(self, path);
}

void modelSetShader(Model *self, Shader *shader)
{
    self->vtable->modelSetShader(self, shader);
}

void modelTranslate(Model *self, float x, float y, float z)
{
    self->vtable->modelTranslate(self, x, y, z);
}

void modelRotate(Model *self, float x, float y, float z, float theta)
{
    self->vtable->modelRotate(self, x, y, z, theta);
}

void modelScale(Model *self, float x, float y, float z)
{
    self->vtable->modelScale(self, x, y, z);
}

void modelRender(Model *self)
{
    self->vtable->modelRender(self);
}

void modelUpdateProjection(Model *self, mat4f view, mat4f perspective)
{
    self->vtable->modelUpdateProjection(self, view, perspective);
}

void modelPrint(Model *self)
{
    printf("[Model ID: %d]\n\n", self->modelID);
    self->vtable->modelPrint(self);
}

void modelDestroy(Model *self) 
{
    self->vtable->modelDestroy(self);
}
