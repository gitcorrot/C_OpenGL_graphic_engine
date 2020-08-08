#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vModel.h"
#include "utils.h"

int countElements(FILE *f, char *element);
void getVertexPositions(FILE *f, vec3f positions[]);

struct modelVTable vModelVTable =
{
    &vModelLoad,
    &vModelSetShader,
    &vModelTranslate,
    &vModelRotate,
    &vModelScale,
    &vModelRender,
    &vModelUpdateProjection,
    &vModelPrint,
    &vModelDestroy
};

Model *vModelCreate()
{
    vModel *vMdl = (vModel *)malloc(sizeof(vModel));
    
    vMdl->super.vtable = &vModelVTable;
    vMdl->super.modelID = rand();

    mat4fIdentity(vMdl->translation);
    mat4fIdentity(vMdl->rotation);
    mat4fIdentity(vMdl->scale);

    glGenVertexArrays(1, &vMdl->VAO);
    glGenBuffers(1, &vMdl->VBO);

    Model *model = (Model *)vMdl; // upcast - first field of 'vtnModel' is base class 'Model'

    return model;
}

void vModelLoad(vModel *self, char *patch)
{
    /*  ALGORITHM (without usage of EBO)

    1. Count vertices                                       [✓]

    4. allocate vertexPosition array                        [✓]

    7. count indicies ('f') * 3                             [✓]
    8. allocate vectices array                              [✓]
    9. go through indicies and add final vertices to array  [✓]
*/

    FILE *f;

    f = fopen(patch, "r");
    if (f == NULL)
    {
        LOG_E("Can't open '%s'", patch);
    }

    int vertexPositionCount = countElements(f, "v ");
    vec3f vertexPositions[vertexPositionCount];
    getVertexPositions(f, vertexPositions);

    int verticesCount = countElements(f, "f ") * 3;
    vVertex *tempVertices = (vVertex *)malloc(sizeof(vVertex) * verticesCount);

    char buffer[255];
    int iter = 0;

    fseek(f, 0, SEEK_SET); // go to start of file

    while (fgets(buffer, 255, f))
    {
        if (strstr(buffer, "f ") != NULL)
        {
            int indices[3];
            int ret = sscanf(buffer, "f %d %d %d\n",
                             &indices[0], &indices[1], &indices[2]);

            if (ret != 3)
            {
                LOG_E("Error during model loading! Only %d/3 attributes loaded.", ret);
                exit(EXIT_FAILURE);
            }
            else
            {
                vVertex verts[3];
                for (int i = 0; i < 3; i++)
                {
                    verts[i].position[0] = vertexPositions[indices[i] - 1][0];
                    verts[i].position[1] = vertexPositions[indices[i] - 1][1];
                    verts[i].position[2] = vertexPositions[indices[i] - 1][2];
                }

                tempVertices[iter] = verts[0];
                tempVertices[iter + 1] = verts[1];
                tempVertices[iter + 2] = verts[2];

                iter += 3;
            }
        }
    }

    self->vertices = tempVertices;
    self->verticesCount = verticesCount;

    glBindVertexArray(self->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, self->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertex)*self->verticesCount, self->vertices, GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
}

void vModelSetShader(vModel *self, Shader *shader)
{
    self->shader = shader;
}

void vModelTranslate(vModel *self, float x, float y, float z)
{
    self->translation[3] += x;
    self->translation[7] += y;
    self->translation[11] += z;
}

void vModelRotate(vModel *self, float x, float y, float z, float theta) 
{
        // this way is ok, but you don't know the object rotation
    mat4f tmp;
    mat4fRotation(tmp, x, y, z, theta);
    mat4fMultiply(self->rotation, tmp);
}

void vModelScale(vModel *self, float x, float y, float z) 
{ 
    self->scale[0] += x;
    self->scale[5] += y;
    self->scale[10] += z;
}

void vModelRender(vModel *self)
{
    // shaderActivate(self->shader);
    glBindVertexArray(self->VAO);

    mat4f tmp;
    mat4fCopy(self->translation, tmp);
    mat4fMultiply(tmp, self->rotation);
    mat4fMultiply(tmp, self->scale);
    shaderSetUniformMat4(self->shader, "model", tmp);
    glDrawArrays(GL_TRIANGLES, 0, self->verticesCount);
    glBindVertexArray(0);
}

void vModelUpdateProjection(vModel *self, mat4f view, mat4f perspective)
{
    shaderActivate(self->shader);
    shaderSetUniformMat4(self->shader, "view", view);
    shaderSetUniformMat4(self->shader, "projection", perspective);
}

void vModelPrint(vModel *self)
{
    // printf("[Model ID: %d]\n\n", self->modelID);
    for (int i = 0; i < self->verticesCount; i++)
    {
        printf("[Vertex %d]\n", i);
        printf("{\n");
        printf("position: [%f, %f, %f]\n",
               self->vertices[i].position[0],
               self->vertices[i].position[1],
               self->vertices[i].position[2]);
        printf("}\n");
    }
}

void vModelDestroy(vModel *self) 
{
    free(self);
}
