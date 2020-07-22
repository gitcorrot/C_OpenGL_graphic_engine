#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mathOpengl.h"

/*      vec3f        */
void vec3fAdd(vec3f this, vec3f v)
{
    for (int i = 0; i < 3; i++)
        this[i] += v[i];
}

void vec3fSubtract(vec3f this, vec3f v)
{
    for (int i = 0; i < 3; i++)
        this[i] -= v[i];
}

float vec3fDotProduct(vec3f this, vec3f v) // dot product
{
    float sum = 0.0;

    for (int i = 0; i < 3; i++)
        sum += this[i] * v[i];

    return sum;
}

void vec3fCrossProduct(vec3f result, vec3f v1, vec3f v2)
{
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
    result[1] = v1[2] * v2[0] - v1[0] * v2[2];
    result[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

float vec3fLength(vec3f this)
{
    return sqrtf(this[0]*this[0] + this[1]*this[1] + this[2]*this[2]);
}

void vec3fCopy(vec3f this, vec3f v) // from "this" to "v"
{
    for (int i = 0; i < 3; i++)
        v[i] = this[i];
}

void vec3fNormalize(vec3f this)
{
    float length = vec3fLength(this);
    this[0] = this[0] / length;
    this[1] = this[1] / length;
    this[2] = this[2] / length;
}


/*      vec4f        */
void vec4fCopy(vec4f this, vec4f v)
{
    for (int i = 0; i < 4; i++)
        v[i] = this[i];
}

void vec4fAdd(vec4f this, vec4f v)
{
    for (int i = 0; i < 4; i++)
        this[i] += v[i];
}

void vec4fSubtract(vec4f this, vec4f v)
{
    for (int i = 0; i < 4; i++)
        this[i] -= v[i];
}

float vec4fDotProduct(vec4f this, vec4f v)
{
    float sum = 0.0;

    for (int i = 0; i < 4; i++)
        sum += this[i] * v[i];

    return sum;
}

/*      mat3f       */
void mat3fCopy(mat3f this, mat3f m)
{
    for (int i = 0; i < 9; i++)
        m[i] = this[i];
}

void mat3fAdd(mat3f this, mat3f m)
{
    for (int i = 0; i < 9; i++)
        this[i] += m[i];
}

void mat3fSubtract(mat3f this, mat3f m)
{
    for (int i = 0; i < 9; i++)
        this[i] -= m[i];
}

void mat3fMultiply(mat3f this, mat3f m)
{
    mat3f tmp;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                tmp[i * 3 + j] += this[i * 3 + k] * m[k * 3 + j];
            }
        }
    }

    mat3fCopy(tmp, this);
}

/*      mat4f       */
void mat4fCopy(mat4f this, mat4f m)
{
    for (int i = 0; i < 16; i++)
        m[i] = this[i];
}

void mat4fIdentity(mat4f this)
{
    this[0] = 1.0;    this[1] = 0.0;    this[2] = 0.0;    this[3] = 0.0;
    this[4] = 0.0;    this[5] = 1.0;    this[6] = 0.0;    this[7] = 0.0;
    this[8] = 0.0;    this[9] = 0.0;    this[10] = 1.0;   this[11] = 0.0;
    this[12] = 0.0;   this[13] = 0.0;   this[14] = 0.0;   this[15] = 1.0;

    // mat4f i = {
    //     1.0,0.0,0.0,
    //     0.0,1.0,0.0,
    //     0.0,0.0,1.0
    // };

    // memcpy(this, i, sizeof(i));
}

void mat4fAdd(mat4f this, mat4f m)
{
    for (int i = 0; i < 16; i++)
        this[i] += m[i];
}

void mat4fSubtract(mat4f this, mat4f m)
{
    for (int i = 0; i < 16; i++)
        this[i] -= m[i];
}

void mat4fMultiply(mat4f this, mat4f m)
{
    mat4f tmp = 
    {
        0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0
    };
    
    int i,j,k;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            for (k = 0; k < 4; k++)
            {
                tmp[i * 4 + j] += this[i * 4 + k] * m[k * 4 + j];
            }
        }
    }
    
    mat4fCopy(tmp, this);
}

void mat4fMultiplyVec4f(vec4f dest, mat4f this, vec4f v)
{
    for (int i = 0; i < 4; i++)
    {
        dest[i] = 0;
        for (int j = 0; j < 4; j++)
        {
            dest[i] += this[i * 4 + j] * v[j];
        }
    }
}

void mat4fTranslate(mat4f dest, float x, float y, float z)
{
    mat4fIdentity(dest);
    dest[3] = x;
    dest[7] = y;
    dest[11] = z;
}

void mat4fScale(mat4f dest, float x, float y, float z)
{
    mat4fIdentity(dest);
    dest[3] *= x;
    dest[7] *= y;
    dest[11] *= z;
}

void mat4fRotate(mat4f dest, float x, float y, float z, float theta)
{
    // TODO: tmp is not neccesary?
    mat4f tmp;
    float c = cosf(theta);
    float s = sinf(theta);

    // https://en.wikipedia.org/wiki/Rotation_matrix
    tmp[0] = c + x * x * (1 - c);
    tmp[1] = x * y * (1 - c) - z * s;
    tmp[2] = x * z * (1 - c) + y * s;
    tmp[3] = 0;
    tmp[4] = y * x * (1 - c) + z * s;
    tmp[5] = c + y * y * (1 - c);
    tmp[6] = y * z * (1 - c) - x * s;
    tmp[7] = 0;
    tmp[8] = z * x * (1 - c) - y * s;
    tmp[9] = z * y * (1 - c) + x * s;
    tmp[10] = c + z * z * (1 - c);
    tmp[11] = 0;
    tmp[12] = 0;
    tmp[13] = 0;
    tmp[14] = 0;
    tmp[15] = 1;

    mat4fCopy(tmp, dest);
}

void mat4fPerspective(mat4f dest, float fov, float aspectRatio, float near, float far)
{
    float d = 1 / tanf((fov*DEG2RAD)/2);

    // https://en.wikibooks.org/wiki/GLSL_Programming/Vertex_Transformations
    dest[0] = d / aspectRatio;
    dest[1] = 0;
    dest[2] = 0;
    dest[3] = 0;
    dest[4] = 0;
    dest[5] = d;
    dest[6] = 0;
    dest[7] = 0;
    dest[8] = 0;
    dest[9] = 0;
    dest[10] = (near + far) / (near - far);
    dest[11] = (2 * near * far) / (near - far);
    dest[12] = 0;
    dest[13] = 0;
    dest[14] = -1;
    dest[15] = 0;
}

void mat4fLookAt(mat4f dest, vec3f cameraPosition, vec3f cameraTarget)
{
    vec3f cameraDirection = {0.0, 0.0, 0.0};
    vec3fCopy(cameraPosition, cameraDirection);
    vec3fSubtract(cameraDirection, cameraTarget);
    vec3fNormalize(cameraDirection);

    vec3f cameraRight = {0.0, 0.0, 0.0};
    vec3f up = { 0.0, 1.0, 0.0 };
    vec3fCrossProduct(cameraRight, up, cameraDirection);
    vec3fNormalize(cameraRight);

    vec3f cameraUp = {0.0, 0.0, 0.0};
    vec3fCrossProduct(cameraUp, cameraDirection, cameraRight); // tutaj chyba jest błąd

    mat4f tempTranslate;
    mat4fIdentity(dest);
    mat4fIdentity(tempTranslate);

    dest[0] = cameraRight[0];
    dest[1] = cameraRight[1];
    dest[2] = cameraRight[2];
    dest[4] = cameraUp[0];
    dest[5] = cameraUp[1];
    dest[6] = cameraUp[2];
    dest[8] = cameraDirection[0];
    dest[9] = cameraDirection[1];
    dest[10] = cameraDirection[2];

    tempTranslate[3] = -cameraPosition[0];
    tempTranslate[7] = -cameraPosition[1];
    tempTranslate[11] = -cameraPosition[2];

    mat4fMultiply(dest, tempTranslate);
}

void mat4fPrint(mat4f this)
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            printf("%.3f\t", this[i * 4 + j]);
        }
        printf("\n");
    }
}
