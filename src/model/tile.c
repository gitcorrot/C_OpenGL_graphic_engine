#include <stdio.h>
#include <stdlib.h>

#include "tile.h"
#include "../include/stb_image.h"
#include "utils.h"
#include "perlin.h"

int *tileGenerateIndices(int indicesSize);
TileVertex *tileGenerateVertices(int verticesSize);
void tileCalculateNormals(Tile *self);
TileVertex *getTileOnPos(Tile *self, int x, int y);

Tile *tileCreate(int positionX, int positionY)
{
    Tile *tile = (Tile *)malloc(sizeof(Tile));

    tile->tileID = rand();

    tile->verticesCount = (TILE_MESH_SIZE + 1) * (TILE_MESH_SIZE + 1);
    tile->indicesCount = (TILE_MESH_SIZE * TILE_MESH_SIZE) * 2 * 3;

    tile->vertices = tileGenerateVertices(tile->verticesCount);
    tile->indices = tileGenerateIndices(tile->indicesCount);
    tileCalculateNormals(tile);

    tile->shader = shaderCreateFromFile(
        "resources/shaders/terrain_vs.glsl",
        "resources/shaders/terrain_fs.glsl");

    glGenVertexArrays(1, &tile->VAO);
    glGenBuffers(1, &tile->VBO);
    glGenBuffers(1, &tile->EBO);

    glBindVertexArray(tile->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, tile->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TileVertex) * tile->verticesCount, tile->vertices, GL_STATIC_DRAW);
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)0);
    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    // Normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));
    // Color
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(8 * sizeof(GLfloat)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tile->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * tile->indicesCount, tile->indices, GL_STATIC_DRAW);

    // Texture configuration
    glGenTextures(1, &tile->textureID);
    glBindTexture(GL_TEXTURE_2D, tile->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("resources/textures/grass.jpg",
                                    &width, &height, &nrChannels, STBI_rgb);
    // unsigned char *data = stbi_load("resources/textures/cat.jpeg",
    //                                 &width, &height, &nrChannels, STBI_rgb);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        LOG_E("Can't load image!");
    }

    stbi_image_free(data);

    return tile;
}

void tileRender(Tile *self, mat4f view, mat4f perspective, vec3f lightPosition)
{
    mat4f tmp;
    mat4fIdentity(tmp);
    float offsetX = self->positionX * TILE_SIZE;
    float offsetZ = self->positionY * TILE_SIZE;
    mat4fTranslate(tmp, offsetX, 0, offsetZ);

    shaderActivate(self->shader);

    shaderSetUniformMat4(self->shader, "model", tmp);
    shaderSetUniformMat4(self->shader, "view", view);
    shaderSetUniformMat4(self->shader, "projection", perspective);
    shaderSetUniformVec3(self->shader, "lightPosition", lightPosition);

    glBindVertexArray(self->VAO);
    glDrawElements(GL_TRIANGLES, self->indicesCount, GL_UNSIGNED_INT, (void *)0);
    // glDrawElements(GL_LINES, self->indicesCount, GL_UNSIGNED_INT, (void *)0);
    glBindVertexArray(0);
}

// Private functions
TileVertex *tileGenerateVertices(int verticesSize)
{
    TileVertex *vertices = (TileVertex *)malloc(sizeof(TileVertex) * verticesSize);

    int i, j;
    float height = 0.0;

    for (i = 0; i < TILE_MESH_SIZE + 1; i++)
    {
        for (j = 0; j < TILE_MESH_SIZE + 1; j++)
        {
            height = perlinGet2d(i, j, 0.25, 2);

            TileVertex tmp;
            tmp.position[0] = j * ((float)TILE_SIZE / TILE_MESH_SIZE);
            tmp.position[1] = height * TERRAIN_MAX_HEIGHT;
            tmp.position[2] = i * ((float)TILE_SIZE / TILE_MESH_SIZE);

            tmp.texture[0] = (float)j; // textures repeat themselves
            tmp.texture[1] = (float)i; // textures repeat themselves

            // RGB interpolation from green to brown
            // tmp.color[0] = (((92.0-102.0) * height) + 102.0) / 255.0;
            // tmp.color[1] = (((92.0-255.0) * height) + 255.0) / 255.0;
            // tmp.color[2] = (((61.0-102.0) * height) + 102.0) / 255.0;
            if (height < 0.4)
            {
                tmp.color[0] = (102.0) / 255.0;
                tmp.color[1] = (255.0) / 255.0;
                tmp.color[2] = (102.0) / 255.0;
            }
            else if (height > 0.4 && height < 0.6)
            {
                tmp.color[0] = (97.0) / 255.0;
                tmp.color[1] = (81.0) / 255.0;
                tmp.color[2] = (61.0) / 255.0;
            }
            else
            {
                tmp.color[0] = (222.0) / 255.0;
                tmp.color[1] = (222.0) / 255.0;
                tmp.color[2] = (222.0) / 255.0;
            }

            vertices[j + i * (TILE_MESH_SIZE + 1)] = tmp;
        }
    }

    return vertices;
}

int *tileGenerateIndices(int indicesSize)
{
    int *indices = (int *)malloc(sizeof(int) * indicesSize);

    int i, j;
    int index = 0;

    for (i = 0; i < TILE_MESH_SIZE; i++)
    {
        for (j = 0; j < TILE_MESH_SIZE; j++)
        {
            int tl = j + (i * (TILE_MESH_SIZE + 1));
            int tr = tl + 1;
            int bl = tr + TILE_MESH_SIZE;
            int br = bl + 1;

            // Winding in counter clock diration
            // First triangle
            indices[index++] = tl;
            indices[index++] = bl;
            indices[index++] = br;
            // Second triangle
            indices[index++] = tl;
            indices[index++] = br;
            indices[index++] = tr;
        }
    }

    return indices;
}

void tileCalculateNormals(Tile *self)
{
    int i, j;

    for (i = 0; i < TILE_MESH_SIZE + 1; i++)
    {
        for (j = 0; j < TILE_MESH_SIZE + 1; j++)
        {
            vec3f pCenter, pLeft, pRight, pTop, pBottom;
            vec3fCopy(getTileOnPos(self, j, i)->position, pCenter);
            vec3fCopy(getTileOnPos(self, j-1, i)->position, pLeft); // TODO: free when obj is new allocated TileVector
            vec3fCopy(getTileOnPos(self, j+1, i)->position, pRight);
            vec3fCopy(getTileOnPos(self, j, i+1)->position, pTop);
            vec3fCopy(getTileOnPos(self, j, i-1)->position, pBottom);

            vec3f cl, cr, ct, cb;
            // vec3fSubtract(cl, pCenter, pLeft);
            // vec3fSubtract(cr, pCenter, pRight);
            // vec3fSubtract(ct, pCenter, pTop);
            // vec3fSubtract(cb, pCenter, pBottom);
            vec3fSubtract(cl, pCenter, pLeft);
            vec3fSubtract(cr, pRight, pCenter);
            vec3fSubtract(ct, pCenter, pTop);
            vec3fSubtract(cb, pBottom, pCenter);

            vec3fNormalize(cl);
            vec3fNormalize(cr);
            vec3fNormalize(ct);
            vec3fNormalize(cb);

            vec3f n1, n2, n3, n4;
            vec3fCrossProduct(n1, cl, ct);
            vec3fCrossProduct(n2, ct, cr);
            vec3fCrossProduct(n3, cr, cb);
            vec3fCrossProduct(n4, cb, cl);

            // vec3fPrint(n1);
            // vec3fPrint(n2);
            // vec3fPrint(n3);
            // vec3fPrint(n4);

            vec3f normal;
            vec3fAdd(normal, normal, n1);
            // vec3fAdd(normal, normal, n2);
            vec3fAdd(normal, normal, n3);
            // vec3fAdd(normal, normal, n4);

            vec3fNormalize(normal);

            // printf("Normal: %f, %f, %f\n\n", normal[0], normal[1], normal[2]);

            // if(normal[1] < 0) normal[1] = -normal[1];

            self->vertices[i * (TILE_MESH_SIZE + 1) + j].normal[0] = normal[0];
            self->vertices[i * (TILE_MESH_SIZE + 1) + j].normal[1] = normal[1];
            self->vertices[i * (TILE_MESH_SIZE + 1) + j].normal[2] = normal[2];
        }
    }

    // for (i = 0; i < self->indicesCount; i += 3)
    // {
    //     vec3f lt, rt, lb, rb;
    //     vec3fCopy(self->vertices[self->indices[i]].position, lt);
    //     vec3fCopy(self->vertices[self->indices[i + 1]].position, rt);
    //     vec3fCopy(self->vertices[self->indices[i + 2]].position, rb);

    //     // printf("%d\n", self->indices[i]);
    //     // printf("%d\n", self->indices[i+1]);
    //     // printf("%d\n", self->indices[i+2]);
    //     // printf("%d\n\n", self->indices[i+5]);

    //     vec3f v_lt_rt, v_lt_rb, v_lt_lb;
    //     vec3fSubtract(v_lt_rt, rt, lt);
    //     vec3fSubtract(v_lt_rb, rb, lt);
    //     vec3fSubtract(v_lt_lb, lb, lt);

    //     vec3fNormalize(v_lt_rt);
    //     vec3fNormalize(v_lt_rb);
    //     vec3fNormalize(v_lt_lb);

    //     vec3f n1, n2;
    //     vec3fCrossProduct(n1, v_lt_rb, v_lt_rt);
    //     vec3fCrossProduct(n2, v_lt_rb, v_lt_lb);

    //     vec3fNormalize(n1);
    //     vec3fNormalize(n2);

    //     //  printf("n1: %f, %f, %f\n", n1[0], n1[1], n1[2]);
    //     //  printf("n2: %f, %f, %f\n\n", n2[0], n2[1], n2[2]);

    //     self->vertices[self->indices[i]].normal[0] = (n2[0]);
    //     self->vertices[self->indices[i]].normal[1] = (n2[1]);
    //     self->vertices[self->indices[i]].normal[2] = (n2[2]);
    //     // self->vertices[self->indices[i]].normal[0] = (n1[0] + n2[0]);
    //     // self->vertices[self->indices[i]].normal[1] = (n1[1] + n2[1]);
    //     // self->vertices[self->indices[i]].normal[2] = (n1[2] + n2[2]);
    // }
}

TileVertex *getTileOnPos(Tile *self, int x, int y)
{
    if (x > TILE_MESH_SIZE || y > TILE_MESH_SIZE || x < 0 || y < 0)
    {
        // LOG_E("Position given in argument is larger that tile mesh size or less than 0!\n");

        TileVertex *tmp = (TileVertex *)malloc(sizeof(TileVertex));
        return tmp;
        // return NULL;
    }
    else
    {
        return &self->vertices[x + y * (TILE_MESH_SIZE + 1)];
    }
}

void tilePrint(Tile *self)
{
    printf("[Tile ID: %d]\n\n", self->tileID);

    int i;

    for (i = 0; i < self->verticesCount; i++)
    {
        printf("Vertex %d: [%f, %f, %f], [%f, %f]\n", i,
               self->vertices[i].position[0],
               self->vertices[i].position[1],
               self->vertices[i].position[2],
               self->vertices[i].texture[0],
               self->vertices[i].texture[1]);
    }

    for (i = 0; i < self->indicesCount; i += 3)
    {
        printf("Triangle %d: [%d, %d, %d]\n", i / 3,
               self->indices[i],
               self->indices[i + 1],
               self->indices[i + 2]);
    }
}
