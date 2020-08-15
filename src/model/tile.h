#ifndef _TILE_H_
#define _TILE_H_

#include "mathOpengl.h"
#include "shader.h"
#include "consts.h"

// 3*4 + 2*4 + 3*4 + 3*4 => 44 bytes
typedef struct
{
    vec3f position;
    vec2f texture;
    vec3f normal;
    vec3f color;

} TileVertex;

typedef struct
{
    int tileID;
    int textureID;
    int positionX;
    int positionY;

    TileVertex *vertices;
    int verticesCount;

    unsigned int VAO, VBO;
    Shader *shader;

} Tile;

Tile *tileCreate(int positionX, int positionY);

void tileRender(Tile *self, mat4f view, mat4f perspective, vec3f lightPosition);
void tilePrint(Tile *self);

#endif