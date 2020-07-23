#ifndef _MATHOPENGL_H_
#define _MATHOPENGL_H_

// TODO:
// - comparison
// - invert
// - generate projection matrix
// - generate look at matrix

#define PI      3.1415926
#define DEG2RAD 0.0174532
#define RAD2DEG 57.295779

typedef float vec3f[3];
typedef float vec4f[4];
typedef float mat3f[3 * 3];
typedef float mat4f[4 * 4];

void    vec3fAdd(vec3f this, vec3f v);
void    vec3fSubtract(vec3f this, vec3f v);
float   vec3fDotProduct(vec3f this, vec3f v);
void    vec3fCrossProduct(vec3f result, vec3f v1, vec3f v2);
void    vec3fMultiplyScalar(vec3f result, vec3f m, float s);
float   vec3fLength(vec3f this);
void    vec3fNormalize(vec3f this);
void    vec3fCopy(vec3f this, vec3f v); // from "this" to "v"
void    vec3fPrint(vec3f this);

void    vec4fAdd(vec4f this, vec4f v);
void    vec4fSubtract(vec4f this, vec4f v);
float   vec4fDotProduct(vec4f this, vec4f v);

void mat3fCopy(mat3f this, mat3f m); // from "this" to "m"
void mat3fAdd(mat3f this, mat3f m);
void mat3fSubtract(mat3f this, mat3f m);
void mat3fMultiply(mat3f this, mat3f m);

void mat4fCopy(mat4f this, mat4f m); // from "this" to "m"
void mat4fIdentity(mat4f this);
void mat4fAdd(mat4f this, mat4f m);
void mat4fSubtract(mat4f this, mat4f m);
void mat4fMultiply(mat4f this, mat4f m);
void mat4fMultiplyVec4f(vec4f dest, mat4f this, vec4f v);

// returns translation matrix 4x4
void mat4fTranslate(mat4f dest, float x, float y, float z); 

// returns scale matrix 4x4
void mat4fScale(mat4f dest, float x, float y, float z); 

// returns rotation matrix 4x4
void mat4fRotate(mat4f dest, float x, float y, float z, float theta); 

// returns perspective matrix 4x4 (fov in deegres)
void mat4fPerspective(mat4f dest, float fov, float aspectRatio, float near, float far);

void mat4fLookAt(mat4f dest, vec3f cameraPosition, vec3f cameraTarget);

void mat4fPrint(mat4f this);

#endif