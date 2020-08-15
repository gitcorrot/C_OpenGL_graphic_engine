#include <stdio.h>
#include <stdlib.h>

#include "tile.h"
#include "../include/stb_image.h"
#include "utils.h"
#include "perlin.h"

TileVertex *tileGenerateVertices(int verticesSize);
// TileVertex *getTileOnPos(Tile *self, int x, int y);
void calculateNormal(vec3f result, TileVertex tv1, TileVertex tv2, TileVertex tv3);
void applyColor(TileVertex *tv );
void copyTileVertex(TileVertex from, TileVertex *to);

Tile *tileCreate(int positionX, int positionY)
{
    Tile *tile = (Tile *)malloc(sizeof(Tile));

    tile->tileID = rand();
    tile->shader = shaderCreateFromFile(
        "resources/shaders/terrain_vs.glsl",
        "resources/shaders/terrain_fs.glsl");

    tile->verticesCount = TILE_MESH_SIZE * TILE_MESH_SIZE * 2 * 3;
    tile->vertices = tileGenerateVertices(tile->verticesCount);

    glGenVertexArrays(1, &tile->VAO);
    glGenBuffers(1, &tile->VBO);

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
    glDrawArrays(GL_TRIANGLES, 0, self->verticesCount);
    // glDrawArrays(GL_LINES, 0, self->verticesCount);
    glBindVertexArray(0);
}

// Private functions
TileVertex *tileGenerateVertices(int verticesSize)
{
    TileVertex *vertices = (TileVertex *)malloc(sizeof(TileVertex) * verticesSize);

    TileVertex lt, rt, lb, rb;
    int index = 0;
    int i, j;
    float height = 0.0;

    for (i = 0; i < TILE_MESH_SIZE; i++)
    {
        for (j = 0; j < TILE_MESH_SIZE; j++)
        {
            height = perlinGet2d(i, j, 0.25, 2);

            lt.position[0] = j * ((float)TILE_SIZE / TILE_MESH_SIZE);
            rt.position[0] = (j + 1) * ((float)TILE_SIZE / TILE_MESH_SIZE);
            lb.position[0] = j * ((float)TILE_SIZE / TILE_MESH_SIZE);
            rb.position[0] = (j + 1) * ((float)TILE_SIZE / TILE_MESH_SIZE);

            lt.position[1] = perlinGet2d(i, j, 0.125, 2) * TERRAIN_MAX_HEIGHT;
            rt.position[1] = perlinGet2d(i, j + 1, 0.125, 2) * TERRAIN_MAX_HEIGHT;
            lb.position[1] = perlinGet2d(i + 1, j, 0.125, 2) * TERRAIN_MAX_HEIGHT;
            rb.position[1] = perlinGet2d(i + 1, j + 1, 0.125, 2) * TERRAIN_MAX_HEIGHT;

            lt.position[2] = i * ((float)TILE_SIZE / TILE_MESH_SIZE);
            rt.position[2] = i * ((float)TILE_SIZE / TILE_MESH_SIZE);
            lb.position[2] = (i + 1) * ((float)TILE_SIZE / TILE_MESH_SIZE);
            rb.position[2] = (i + 1) * ((float)TILE_SIZE / TILE_MESH_SIZE);

            lt.texture[0] = (float)j; // textures repeat themselves
            rt.texture[0] = (float)j+1; // textures repeat themselves
            lb.texture[0] = (float)j; // textures repeat themselves
            rb.texture[0] = (float)j+1; // textures repeat themselves

            lt.texture[1] = (float)i; // textures repeat themselves
            rt.texture[1] = (float)i; // textures repeat themselves
            lb.texture[1] = (float)i+1; // textures repeat themselves
            rb.texture[1] = (float)i+1; // textures repeat themselves

            vec3f n1, n2;
            calculateNormal(n1, lt, lb, rb);
            calculateNormal(n2, lt, rb, rt);

            applyColor(&lt);
            applyColor(&rt);
            applyColor(&lb);
            applyColor(&rb);

            // Triangle 1
            copyTileVertex(lt, &vertices[index++]);
            vec3fCopy(n1, vertices[index-1].normal);
            copyTileVertex(lb, &vertices[index++]);
            vec3fCopy(n1, vertices[index-1].normal);
            copyTileVertex(rb, &vertices[index++]);
            vec3fCopy(n1, vertices[index-1].normal);
            // Triangle 2
            copyTileVertex(lt, &vertices[index++]);
            vec3fCopy(n2, vertices[index-1].normal);
            copyTileVertex(rb, &vertices[index++]);
            vec3fCopy(n2, vertices[index-1].normal);
            copyTileVertex(rt, &vertices[index++]);
            vec3fCopy(n2, vertices[index-1].normal);
        }
    }

    return vertices;
}

void copyTileVertex(TileVertex from, TileVertex *to)
{
    to->position[0] = from.position[0];
    to->position[1] = from.position[1];
    to->position[2] = from.position[2];

    to->texture[0] = from.texture[0];
    to->texture[1] = from.texture[1];
    
    to->normal[0] = from.normal[0];
    to->normal[1] = from.normal[1];
    to->normal[2] = from.normal[2];

    to->color[0] = from.color[0];
    to->color[1] = from.color[1];
    to->color[2] = from.color[2];
}

void applyColor(TileVertex *tv)
{
    vec3f color;
    float height = tv->position[1] / TERRAIN_MAX_HEIGHT;

    // RGB interpolation from green to brown
    // tmp.color[0] = (((92.0-102.0) * height) + 102.0) / 255.0;
    // tmp.color[1] = (((92.0-255.0) * height) + 255.0) / 255.0;
    // tmp.color[2] = (((61.0-102.0) * height) + 102.0) / 255.0;

    if (height < 0.3)
    {
        color[0] = (102.0) / 255.0;
        color[1] = (255.0) / 255.0;
        color[2] = (102.0) / 255.0;
    }
    else if (height > 0.3 && height < 0.6)
    {
        color[0] = (97.0) / 255.0;
        color[1] = (81.0) / 255.0;
        color[2] = (61.0) / 255.0;
    }
    else
    {
        color[0] = (222.0) / 255.0;
        color[1] = (222.0) / 255.0;
        color[2] = (222.0) / 255.0;
    }

    tv->color[0] = color[0];
    tv->color[1] = color[1];
    tv->color[2] = color[2];
}

void calculateNormal(vec3f result, TileVertex tv1, TileVertex tv2, TileVertex tv3)
{
    vec3f v1, v2;
    vec3fSubtract(v1, tv2.position, tv1.position);
    vec3fSubtract(v2, tv3.position, tv1.position);

    vec3f n;
    vec3fCrossProduct(n, v1, v2);
    vec3fNormalize(n);

    result[0] = n[0];
    result[1] = n[1];
    result[2] = n[2];
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
}
