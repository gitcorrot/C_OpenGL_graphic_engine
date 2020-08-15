#include <stdio.h>
#include <stdlib.h>

#include "terrain.h"
#include "model/tile.h"


Terrain *terrainCreateRandom()
{
    Terrain *terrain = (Terrain *)malloc(sizeof(Terrain));

    terrain->terrainID = rand();

    // TODO: come up with idea where to decide about position of tiles
    terrain->tiles = tileCreate(0, 0);
    // terrain->tiles = tileCreate(0, 1);

    return terrain;
}

void terrainRender(Terrain *self, mat4f view, mat4f perspective, vec3f lightPosition)
{
    // for each tile -> render
    tileRender(self->tiles, view, perspective, lightPosition);
}
