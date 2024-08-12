#include "vector.h"
#include <string.h>
#include <stdlib.h>

void initVector(Vector* vector, uint64_t typeSize, uint64_t capacity, uint64_t size) {
    if (capacity < size) capacity = size;

    vector->typeSize = typeSize;
    vector->capacity = capacity * typeSize;
    vector->size = size * typeSize;

    vector->data = malloc(vector->capacity);
}

void destroyVector(Vector* vector) {
    free(vector->data);
}

void* indexVector(Vector* vector, uint64_t index) {
    if (index > vector->size) resizeVector(vector, index);
    index *= vector->typeSize;
    return vector->data + index;
}

void resizeVector(Vector* vector, uint64_t newSize) {
    vector->size = newSize * vector->typeSize;

    if (vector->capacity >= vector->size) {
        if (vector->capacity > vector->size + 10) {
            vector->capacity -= 10;
            vector->data = realloc(vector->data, vector->capacity);
        }

        return;
    }

    vector->capacity = vector->size + 10;
    vector->data = realloc(vector->data, vector->capacity);
}

void concatVectors(Vector* destination, Vector* source) {
    resizeVector(destination, vectorSize(destination) + vectorSize(source));
    memcpy(indexVector(destination, vectorSize(destination) + 1), indexVector(source, 0), source->size);
}

uint64_t vectorSize(Vector* vector) {
    return vector->size / vector->typeSize;
}
