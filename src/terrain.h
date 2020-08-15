#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "model/tile.h"
#include "mathOpengl.h"

typedef struct
{
    int terrainID;
    Tile *tiles;

} Terrain;


Terrain *terrainCreateRandom();

void terrainRender(Terrain *self, mat4f view, mat4f perspective, vec3f lightPosition);


#endif