#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"
#include "cube.h"

static const int indices[] =
{
    // front
    2, 1, 0,
    0, 3, 2,
    // back
    6, 5, 4,
    4, 7, 6,
    // left
    1, 5, 4,
    4, 0, 1,
    // right
    6, 2, 3,
    3, 7, 6,
    // top
    3, 0, 4,
    4, 7, 3,
    // bottom
    2, 1, 5,
    5, 6, 2
};

static const vec3f vertices[] = 
{
    // position             // color
    {-0.5,  0.5, 0.5},   {0.0, 0.0, 0.9 },   // 0
    {-0.5, -0.5, 0.5},   {0.0, 0.0, 0.9 },   // 1
    { 0.5, -0.5, 0.5},   {0.0, 0.0, 0.9 },   // 2
    { 0.5,  0.5, 0.5},   {0.0, 0.0, 0.9 },   // 3
        
    {-0.5,  0.5, -0.5},  {0.8, 0.8, 0.0 },   // 4
    {-0.5, -0.5, -0.5},  {0.8, 0.8, 0.0 },   // 5
    { 0.5, -0.5, -0.5},  {0.8, 0.8, 0.0 },   // 6
    { 0.5,  0.5, -0.5},  {0.8, 0.8, 0.0 },   // 7
};

static GLuint programID;
static GLuint VAO, VBO, EBO;

Cube *cubeCreate()
{
    Cube *cube = (Cube *)malloc(sizeof(Cube));

    cube->objectID = (GLuint)rand();
    cube->programID = programID;

    mat4fIdentity(cube->translation);
    mat4fIdentity(cube->rotation);
    mat4fIdentity(cube->scale);

    return cube;
}

void cubeInit()
{
    programID = shaderCreateFromFile("test_vs.glsl", "test_fs.glsl");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Positions (first 3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    // Colors (second 3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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

void cubeRender(Cube *self)
{
    glUseProgram(self->programID);
    glBindVertexArray(VAO);

    mat4f tmp;
    mat4fCopy(self->translation, tmp);
    mat4fMultiply(tmp, self->rotation);
    mat4fMultiply(tmp, self->scale);

    GLint uniModel = glGetUniformLocation(self->programID, "model");
    glUniformMatrix4fv(uniModel, 1, GL_TRUE, tmp);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *)0);
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
    printf("programiD: %d\n", self->programID);
    printf("\n");
}