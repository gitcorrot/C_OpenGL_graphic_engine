#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/stb_image.h"

#include "cube.h"
#include "shader.h"
#include "utils.h"

static const float vertices[] =
{
    -1.0,   1.0,    1.0,    0.0, 0.0,    // Front-top-left
    1.0,    1.0,    1.0,    1.0, 0.0,    // Front-top-right
    -1.0,   -1.0,   1.0,    0.0, 1.0,    // Front-bottom-left
    1.0,    -1.0,   1.0,    1.0, 1.0,    // Front-bottom-right
    1.0,    -1.0,   -1.0,   1.0, 0.0,    // Back-bottom-right
    1.0,    1.0,    1.0,    0.0, 1.0,    // Front-top-right
    1.0,    1.0,    -1.0,   0.0, 0.0,    // Back-top-right
    -1.0,   1.0,    1.0,    1.0, 1.0,    // Front-top-left
    -1.0,   1.0,    -1.0,   1.0, 0.0,    // Back-top-left
    -1.0,   -1.0,   1.0,    0.0, 1.0,    // Front-bottom-left
    -1.0,   -1.0,   -1.0,   0.0, 0.0,    // Back-bottom-left
    1.0,    -1.0,   -1.0,   1.0, 0.0,    // Back-bottom-right
    -1.0,   1.0,    -1.0,   0.0, 1.0,    // Back-top-left
    1.0,    1.0,    -1.0,   1.0, 1.0     // Back-top-right
};

static Shader *shader;;
static GLuint textureID;
static GLuint VAO, VBO;

Cube *cubeCreate()
{
    Cube *cube = (Cube *)malloc(sizeof(Cube));

    cube->objectID = (GLuint)rand();

    mat4fIdentity(cube->translation);
    mat4fIdentity(cube->rotation);
    mat4fIdentity(cube->scale);

    return cube;
}

void cubeInit()
{
    shader = shaderCreateFromFile("resources/shaders/cube_vs.glsl", "resources/shaders/cube_fs.glsl");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
    // Textures coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

    // Textures configuration
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("resources/textures/cat2.jpg", &width, &height, &nrChannels, 3);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Can't load image!");
    }

    stbi_image_free(data);
}

void cubeSetPosition(Cube *self, vec3f position)
{
    self->translation[3] = position[0];
    self->translation[7] = position[1];
    self->translation[11] = position[2];
}

void cubeTranslate(Cube *self, float x, float y, float z)
{
    self->translation[3] += x;
    self->translation[7] += y;
    self->translation[11] += z;
}

void cubeRotate(Cube *self, float x, float y, float z, float theta)
{
    // this way is ok, but you don't know the object rotation
    mat4f tmp;
    mat4fRotation(tmp, x, y, z, theta);
    mat4fMultiply(self->rotation, tmp);
}   

void cubeScale(Cube *self, float x, float y, float z)
{
    self->scale[0] += x;
    self->scale[5] += y;
    self->scale[10] += z;
}

void cubeUpdateProjection(mat4f view, mat4f perspective)
{
    shaderSetUniformMat4(shader, "view", view);
    shaderSetUniformMat4(shader, "projection", perspective);
}

void cubeRender(Cube *self)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    shaderActivate(shader);
    glBindVertexArray(VAO);

    mat4f tmp;
    mat4fCopy(self->translation, tmp);
    mat4fMultiply(tmp, self->rotation);
    mat4fMultiply(tmp, self->scale);

    shaderSetUniformMat4(shader, "model", tmp);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
    glBindVertexArray(0);
}

void cubeDestroy(Cube *self)
{
    free(self);
}

void cubePrint(Cube *self)
{
    printf("\n[Cube object]\n");
    printf("objectID: %d\n", self->objectID);
    printf("programiD: %d\n", shader->programID);
    printf("\n");
}
