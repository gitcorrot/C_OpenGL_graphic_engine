#ifndef _MODEL_H_
#define _MODEL_H_

#include "shader.h"
#include "mathOpengl.h"

typedef struct
{
    vec3f position;
    vec2f texture;
    vec3f normal;

} Vertex;

typedef struct
{
    int modelID;
    int textureID;

    Shader *shader; 

    unsigned int VAO, VBO;
    Vertex *vertices;   
    int verticesCount;

    mat4f translation;
    mat4f rotation;
    mat4f scale;

} Model;


Model *modelCreate();

void modelLoad(Model *self, char *path);
void modelSetShader(Model *self, Shader *shader);
void modelTranslate(Model *self, float x, float y, float z);
void modelRotate(Model *self, float x, float y, float z, float theta);
void modelScale(Model *self, float x, float y, float z);
void modelRender(Model *self);
void modelUpdateProjection(Model *self, mat4f view, mat4f perspective);
void modelPrint(Model *self);

#endif
