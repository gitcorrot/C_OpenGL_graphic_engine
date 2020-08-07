#ifndef _V_MODEL_H_
#define _V_MODEL_H_

#include "../model.h"
#include "../mathOpengl.h"

typedef struct
{
    vec3f position;

} vVertex;

// V-type model -> only vertices (without textures and normals)
typedef struct
{
    Model super; // important to stay as first field

    Shader *shader;
    unsigned int VAO, VBO;
    vVertex *vertices;
    int verticesCount;
    mat4f translation;
    mat4f rotation;
    mat4f scale;

} vModel;

vModel *vModelCreate();

void vModelLoad(vModel *self, char *path);
void vModelSetShader(vModel *self, Shader *shader);
void vModelTranslate(vModel *self, float x, float y, float z);
void vModelRotate(vModel *self, float x, float y, float z, float theta);
void vModelScale(vModel *self, float x, float y, float z);
void vModelRender(vModel *self);
void vModelUpdateProjection(vModel *self, mat4f view, mat4f perspective);
void vModelPrint(vModel *self);


#endif
