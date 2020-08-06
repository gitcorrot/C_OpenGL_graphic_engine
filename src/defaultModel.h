#ifndef _DEFAULT_MODEL_H_
#define _DEFAULT_MODEL_H_

#include "model.h"
#include "mathOpengl.h"

typedef struct
{
    vec3f position;
    vec2f texture;
    vec3f normal;

} Vertex;

typedef struct
{
    Model super; // important to stay as first field

    int textureID;
    Shader *shader;
    unsigned int VAO, VBO;
    Vertex *vertices;
    int verticesCount;
    mat4f translation;
    mat4f rotation;
    mat4f scale;

} defaultModel;

defaultModel *defaultModelCreate();

void defaultModelLoad(defaultModel *self, char *path);
void defaultModelSetShader(defaultModel *self, Shader *shader);
void defaultModelTranslate(defaultModel *self, float x, float y, float z);
void defaultModelRotate(defaultModel *self, float x, float y, float z, float theta);
void defaultModelScale(defaultModel *self, float x, float y, float z);
void defaultModelRender(defaultModel *self);
void defaultModelUpdateProjection(defaultModel *self, mat4f view, mat4f perspective);
void defaultModelPrint(defaultModel *self);


#endif
