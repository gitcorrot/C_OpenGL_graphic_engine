#include <stdio.h>
#include <stdlib.h>

#include "tile.h"
#include "../include/stb_image.h"
#include "utils.h"

int *tileGenerateIndices(int indicesSize);
TileVertex *tileGenerateVertices(int verticesSize);

Tile *tileCreate(int positionX, int positionY)
{
    Tile *tile = (Tile *)malloc(sizeof(Tile));

    tile->tileID = rand();

    tile->verticesCount = (TILE_MESH_SIZE + 1) * (TILE_MESH_SIZE + 1);
    tile->indicesCount = (TILE_MESH_SIZE * TILE_MESH_SIZE) * 2 * 3;

    tile->vertices = tileGenerateVertices(tile->verticesCount);
    tile->indices = tileGenerateIndices(tile->indicesCount);

    tile->shader = shaderCreateFromFile(
                            "resources/shaders/terrain_vs.glsl",
                            "resources/shaders/terrain_fs.glsl");

    glGenVertexArrays(1, &tile->VAO);
    glGenBuffers(1, &tile->VBO);
    glGenBuffers(1, &tile->EBO);

    glBindVertexArray(tile->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, tile->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TileVertex) * tile->verticesCount, tile->vertices, GL_STATIC_DRAW);
    // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)0);
    // Textures
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    // Normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tile->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * tile->indicesCount, tile->indices, GL_STATIC_DRAW);

    // Texture configuration
    glGenTextures(1, &tile->textureID);
    glBindTexture(GL_TEXTURE_2D, tile->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int width, height, nrChannels;
    // unsigned char *data = stbi_load("resources/textures/grass.jpg",
    //                                 &width, &height, &nrChannels, STBI_rgb);
    unsigned char *data = stbi_load("resources/textures/cat.jpeg",
                                    &width, &height, &nrChannels, STBI_rgb);
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

    for (i = 0; i < TILE_MESH_SIZE + 1; i++)
    {
        for (j = 0; j < TILE_MESH_SIZE + 1; j++)
        {
            TileVertex tmp;
            tmp.position[0] = i * ((float)TILE_SIZE / TILE_MESH_SIZE);
            tmp.position[1] = rand() % 8;
            // tmp.position[1] = 0;
            tmp.position[2] = j * ((float)TILE_SIZE / TILE_MESH_SIZE);

            tmp.texture[0] = (float)j; // textures repeat themselves
            tmp.texture[1] = (float)i; // textures repeat themselves

            tmp.normal[0] = 0.0;
            tmp.normal[1] = 1.0;
            tmp.normal[2] = 0.0;

            vertices[i * (TILE_MESH_SIZE + 1) + j] = tmp;
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
            // First triangle
            indices[index++] = tl;
            indices[index++] = tr;
            indices[index++] = br;
            // Second triangle
            indices[index++] = tl;
            indices[index++] = br;
            indices[index++] = bl;
        }
    }

    return indices;
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

    for (i = 0; i < self->indicesCount; i++)
    {
        printf("Index %d: [%d]\n", i,
               self->indices[i]);
    }
}
