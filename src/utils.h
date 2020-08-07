#ifndef _UTILS_H_
#define _UTILS_H_

#include "mathOpengl.h"

#include <stdio.h>

#define DEBUG 1

#define LOG_D(format, args...)                                        \
    if (DEBUG)                                                        \
    {                                                                 \
        printf("%s:%d " format "\n", __FUNCTION__, __LINE__, ##args); \
    }

#define LOG_E(format, args...)                                                        \
    if (DEBUG)                                                                        \
    {                                                                                 \
        fprintf(stderr, "ERROR: %s:%d " format "\n", __FUNCTION__, __LINE__, ##args); \
    }


int countElements(FILE *f, char *element);
void getVertexPositions(FILE *f, vec3f vertexPositions[]);
void getTextureCoordinates(FILE *f, vec2f textureCoordinates[]);
void getNormals(FILE *f, vec3f normals[]);

#endif
