#ifndef _DEFAULT_MODEL_H_
#define _DEFAULT_MODEL_H_

#include "../model.h"
#include "../mathOpengl.h"

typedef struct
{
    vec3f position;
    vec2f texture;
    vec3f normal;

} Vertex;

// VTS-type model -> vertices, textures and normals
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

} vtsModel;

vtsModel *vtsModelCreate();

void vtsModelLoad(vtsModel *self, char *path);
void vtsModelSetShader(vtsModel *self, Shader *shader);
void vtsModelTranslate(vtsModel *self, float x, float y, float z);
void vtsModelRotate(vtsModel *self, float x, float y, float z, float theta);
void vtsModelScale(vtsModel *self, float x, float y, float z);
void vtsModelRender(vtsModel *self);
void vtsModelUpdateProjection(vtsModel *self, mat4f view, mat4f perspective);
void vtsModelPrint(vtsModel *self);


#endif
