#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/stb_image.h"
#include "vtnModel.h"
#include "utils.h"

int countElements(FILE *f, char *element);
void getVertexPositions(FILE *f, vec3f positions[]);
void getTextureCoordinates(FILE *f, vec2f textureCoordinates[]);
void getNormals(FILE *f, vec3f normals[]);

struct modelVTable vtnModelVTable =
{
    &vtnModelLoad,
    &vtnModelSetShader,
    &vtnModelTranslate,
    &vtnModelRotate,
    &vtnModelScale,
    &vtnModelRender,
    &vtnModelUpdateProjection,
    &vtnModelPrint
};

vtnModel *vtnModelCreate()
{
    vtnModel *model = (vtnModel *)malloc(sizeof(vtnModel));
    
    model->super.vtable = &vtnModelVTable;
    model->super.modelID = rand();

    mat4fIdentity(model->translation);
    mat4fIdentity(model->rotation);
    mat4fIdentity(model->scale);

    glGenVertexArrays(1, &model->VAO);
    glGenBuffers(1, &model->VBO);

    return model;
}

void vtnModelLoad(vtnModel *self, char *patch)
{
    /*  ALGORITHM (without usage of EBO)

    1. Count vertices                                       [✓]
    2. Count textureCoordinates                             [✓]
    3. Count normals                                        [✓]

    4. allocate vertexPosition array                        [✓]
    5. allocate textureCoordinates array                    [✓]
    6. allocate normals array                               [✓]

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

    int textureCoordinateCount = countElements(f, "vt ");
    vec2f textureCoordinates[textureCoordinateCount];
    getTextureCoordinates(f, textureCoordinates);

    int normalCount = countElements(f, "vn ");
    vec3f normals[normalCount];
    getNormals(f, normals);

    int verticesCount = countElements(f, "f ") * 3;
    Vertex *tempVertices = (Vertex *)malloc(sizeof(Vertex) * verticesCount);

    char buffer[255];
    int iter = 0;

    fseek(f, 0, SEEK_SET); // go to start of file

    while (fgets(buffer, 255, f))
    {
        if (strstr(buffer, "f ") != NULL)
        {
            int indices[9];
            int ret = sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
                             &indices[0], &indices[1], &indices[2],
                             &indices[3], &indices[4], &indices[5],
                             &indices[6], &indices[7], &indices[8]);

            if (ret != 9)
            {
                LOG_E("Error during model loading! Only %d/9 attributes loaded.", ret);
                exit(EXIT_FAILURE);
            }
            else
            {
                Vertex verts[3];
                for (int i = 0; i < 3; i++)
                {
                    verts[i].position[0] = vertexPositions[indices[3 * i] - 1][0];
                    verts[i].position[1] = vertexPositions[indices[3 * i] - 1][1];
                    verts[i].position[2] = vertexPositions[indices[3 * i] - 1][2];

                    verts[i].texture[0] = textureCoordinates[indices[3 * i + 1] - 1][0];
                    verts[i].texture[1] = textureCoordinates[indices[3 * i + 1] - 1][1];

                    verts[i].normal[0] = normals[indices[3 * i + 2] - 1][0];
                    verts[i].normal[1] = normals[indices[3 * i + 2] - 1][1];
                    verts[i].normal[2] = normals[indices[3 * i + 2] - 1][2];
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*self->verticesCount, self->vertices, GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)0);
    // Textures coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    // Normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));

    // Textures configuration
    glGenTextures(1, &self->textureID);
    glBindTexture(GL_TEXTURE_2D, self->textureID);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("resources/textures/stone.png",
                                    &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Can't load image!");
    }

    stbi_image_free(data);
}

void vtnModelSetShader(vtnModel *self, Shader *shader)
{
    self->shader = shader;
}

void vtnModelTranslate(vtnModel *self, float x, float y, float z)
{
        self->translation[3] += x;
    self->translation[7] += y;
    self->translation[11] += z;
}

void vtnModelRotate(vtnModel *self, float x, float y, float z, float theta) 
{
        // this way is ok, but you don't know the object rotation
    mat4f tmp;
    mat4fRotation(tmp, x, y, z, theta);
    mat4fMultiply(self->rotation, tmp);
}

void vtnModelScale(vtnModel *self, float x, float y, float z) 
{ 
    self->scale[0] += x;
    self->scale[5] += y;
    self->scale[10] += z;
}

void vtnModelRender(vtnModel *self)
{
    // shaderActivate(self->shader);
    glBindTexture(GL_TEXTURE_2D, self->textureID);
    glBindVertexArray(self->VAO);

    mat4f tmp;
    mat4fCopy(self->translation, tmp);
    mat4fMultiply(tmp, self->rotation);
    mat4fMultiply(tmp, self->scale);
    shaderSetUniformMat4(self->shader, "model", tmp);
    glDrawArrays(GL_TRIANGLES, 0, self->verticesCount);
    glBindVertexArray(0);
}

void vtnModelUpdateProjection(vtnModel *self, mat4f view, mat4f perspective)
{
    shaderActivate(self->shader);
    shaderSetUniformMat4(self->shader, "view", view);
    shaderSetUniformMat4(self->shader, "projection", perspective);
}

void vtnModelPrint(vtnModel *self)
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
        printf("textureCoordinate: [%f, %f]\n",
               self->vertices[i].texture[0],
               self->vertices[i].texture[1]);
        printf("normal: [%f, %f, %f]\n",
               self->vertices[i].normal[0],
               self->vertices[i].normal[1],
               self->vertices[i].normal[2]);
        printf("}\n");
    }
}
