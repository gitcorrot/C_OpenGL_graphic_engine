#ifndef _MATHOPENGL_H_
#define _MATHOPENGL_H_

// TODO:
// - comparison
// - invert

#define PI 3.1415926
#define DEG2RAD 0.0174532
#define RAD2DEG 57.295779

typedef float vec2f[2];
typedef float vec3f[3];
typedef float vec4f[4];
typedef float mat3f[3 * 3];
typedef float mat4f[4 * 4];


/*      vec3f        */
void vec3fAdd(vec3f result, vec3f v1, vec3f v2);
void vec3fSubtract(vec3f result, vec3f v1, vec3f v2);
float vec3fDotProduct(vec3f this, vec3f v);
void vec3fCrossProduct(vec3f result, vec3f v1, vec3f v2);
void vec3fMultiplyScalar(vec3f result, vec3f m, float s);
float vec3fLength(vec3f this);
void vec3fNormalize(vec3f this);
void vec3fEmpty(vec3f this);
void vec3fCopy(vec3f this, vec3f v); // from "this" to "v"
void vec3fPrint(vec3f this);

/*      vec4f        */
void vec4fAdd(vec4f this, vec4f v);
void vec4fSubtract(vec4f this, vec4f v);
float vec4fDotProduct(vec4f this, vec4f v);

/*      mat3f       */
void mat3fCopy(mat3f this, mat3f m); // from "this" to "m"
void mat3fAdd(mat3f this, mat3f m);
void mat3fSubtract(mat3f this, mat3f m);
void mat3fMultiply(mat3f this, mat3f m);

/*      mat4f       */
void mat4fCopy(mat4f this, mat4f m); // from "this" to "m"
void mat4fIdentity(mat4f this);
void mat4fAdd(mat4f this, mat4f m);
void mat4fSubtract(mat4f this, mat4f m);
void mat4fMultiply(mat4f this, mat4f m);
void mat4fMultiplyVec4f(vec4f dest, mat4f this, vec4f v);
void mat4fTranslate(mat4f dest, float x, float y, float z);
void mat4fScale(mat4f dest, float x, float y, float z);
void mat4fRotation(mat4f dest, float x, float y, float z, float theta);
// void mat4fRotationFromEuler(mat4f dest, float yaw, float pitch, float roll); // TODO
void mat4fPerspective(mat4f dest, float fov, float aspectRatio, float near, float far);
void mat4fLookAt(mat4f dest, vec3f cameraPosition, vec3f cameraTarget);
void mat4fPrint(mat4f this);

#endif