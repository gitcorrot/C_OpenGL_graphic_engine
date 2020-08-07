#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


int countElements(FILE *f, char *element)
{
    int count = 0;
    char buffer[255];

    fseek(f, 0, SEEK_SET); // go to start of file

    while (fgets(buffer, sizeof(buffer), f))
    {
        if (strstr(buffer, element) != NULL)
        {
            count++;
        }
    }

    return count;
}

void getVertexPositions(FILE *f, vec3f vertexPositions[])
{
    char buffer[255];
    int iter = 0;
    char *element = "v ";
    fseek(f, 0, SEEK_SET); // go to start of file

    while (fgets(buffer, sizeof(buffer), f))
    {
        if (strstr(buffer, element) != NULL)
        {
            sscanf(buffer, "v %f %f %f\n",
                   &vertexPositions[iter][0],
                   &vertexPositions[iter][1],
                   &vertexPositions[iter][2]);
            iter++;
        }
    }
}

void getTextureCoordinates(FILE *f, vec2f textureCoordinates[])
{
    char buffer[255];
    int iter = 0;
    char *element = "vt ";

    fseek(f, 0, SEEK_SET); // go to start of file

    while (fgets(buffer, sizeof(buffer), f))
    {
        if (strstr(buffer, element) != NULL)
        {
            sscanf(buffer, "vt %f %f\n",
                   &textureCoordinates[iter][0],
                   &textureCoordinates[iter][1]);
            iter++;
        }
    }
}

void getNormals(FILE *f, vec3f normals[])
{
    char buffer[255];
    int iter = 0;

    fseek(f, 0, SEEK_SET); // go to start of file

    while (fgets(buffer, 255, f))
    {
        if (strstr(buffer, "vn ") != NULL)
        {
            sscanf(buffer, "vn %f %f %f\n",
                   &normals[iter][0],
                   &normals[iter][1],
                   &normals[iter][2]);
            iter++;
        }
    }
}
