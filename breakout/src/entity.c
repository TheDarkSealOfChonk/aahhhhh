#include "entity.h"
#include "vector.h"
#include <cglm/struct/affine-pre.h>
#include <cglm/struct/mat4.h>

Object* firstObject;
Object* lastObject;
uint64_t objectCount;

void initObjectsSystem() {
    firstObject = malloc(sizeof(Object));
    firstObject->next = 0;
    lastObject = firstObject;
    objectCount = 0;
}

void destroyObjectsSystem() {
    Object* currentObject = firstObject;

    while (currentObject->next != 0) {
        Object* oldCurrentObject = currentObject;
        currentObject = currentObject->next;
        free(oldCurrentObject);
    }
}

Object* getObjects() {
    return firstObject;
}

mat4s makeTranslationMatrix(Object* object) {
    mat4s outMatrix = GLMS_MAT4_IDENTITY;
    outMatrix = glms_translate(outMatrix, object->transform.translation);
    return outMatrix;
}

mat4s makeScaleMatrix(Object* object) {
    mat4s outMatrix = GLMS_MAT4_IDENTITY;
    outMatrix = glms_scale(outMatrix, object->transform.scale);
    return outMatrix;
}

mat4s makeRotationMatrix(Object* object) {
    mat4s outMatrix = GLMS_MAT4_IDENTITY;
    outMatrix = glms_rotate_x(outMatrix, object->transform.rotation.x);
    outMatrix = glms_rotate_y(outMatrix, object->transform.rotation.y);
    outMatrix = glms_rotate_z(outMatrix, object->transform.rotation.z);
    return outMatrix;
}

TransformMatrices makeTransformMatrices(Object* object) {
    TransformMatrices outMatrices;
    outMatrices.translation = makeTranslationMatrix(object);
    outMatrices.scale = makeScaleMatrix(object);
    outMatrices.rotation = makeRotationMatrix(object);
    return outMatrices;
}

void makeAllTransformMatrices(Vector* ouputTransformMatrices) {
    resizeVector(ouputTransformMatrices, objectCount);

    Object* currentObject = firstObject;

    uint64_t i = 0;
    while (currentObject->next != 0) {
        *((TransformMatrices*)indexVector(ouputTransformMatrices, i)) = makeTransformMatrices(currentObject->next);
        currentObject = currentObject->next;
        i++;
    }
}

void concatMeshes(Vector* vertices, Vector* indices) {
    resizeVector(vertices, 10);
    resizeVector(indices, 10);

    Object* currentObject = firstObject;

    while (currentObject->next != 0) {
        concatVectors(vertices, &currentObject->next->mesh.vertices);
        concatVectors(indices, &currentObject->next->mesh.indices);
        currentObject = currentObject->next;
    }
}
