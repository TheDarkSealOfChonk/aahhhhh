#pragma once

#include "vector.h"
#include <cglm/struct.h>
#include <cglm/struct/mat4.h>
#include <sys/types.h>

typedef struct TransformMatrices {
    mat4s translation;
    mat4s scale;
    mat4s rotation;
} TransformMatrices;

typedef struct Transform {
    vec3s translation;
    vec3s scale;
    vec3s rotation;
} Transform;

typedef struct Mesh {
    Vector vertices;
    Vector indices;
} Mesh;

typedef struct UvOffset {
    float offsetX, offsetY;
} UvOffset;

typedef struct Object {
    Transform transform;
    Mesh mesh;
    struct Object* next;
    struct Object* last;
} Object;

void initObjectsSystem();
void destroyObjectsSystem();
mat4s makeTranslationMatrix(Object* object);
mat4s makeScaleMatrix(Object* object);
mat4s makeRotationMatrix(Object* object);
TransformMatrices makeTransformMatrices(Object* object);
void makeAllTransformMatrices(Vector* ouputTransformMatrices);
void concatMeshes(Vector* vertices, Vector* indices);
uint64_t spawnObject();
Object* getObjects();
