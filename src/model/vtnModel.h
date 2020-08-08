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

// vtn-type model -> vertices, textures and normals
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

} vtnModel;

Model *vtnModelCreate();

void vtnModelLoad(vtnModel *self, char *path);
void vtnModelSetShader(vtnModel *self, Shader *shader);
void vtnModelTranslate(vtnModel *self, float x, float y, float z);
void vtnModelRotate(vtnModel *self, float x, float y, float z, float theta);
void vtnModelScale(vtnModel *self, float x, float y, float z);
void vtnModelRender(vtnModel *self);
void vtnModelUpdateProjection(vtnModel *self, mat4f view, mat4f perspective);
void vtnModelPrint(vtnModel *self);
void vtnModelDestroy(vtnModel *self);


#endif
