#pragma once

#include <stdint.h>

typedef struct Vector {
    uint64_t typeSize;
    uint64_t capacity;
    uint64_t size;
    uint8_t* data;
} Vector;

void initVector(Vector* vector, uint64_t typeSize, uint64_t capacity, uint64_t size);
void destroyVector(Vector* vector);
void* indexVector(Vector* vector, uint64_t index);
void resizeVector(Vector* vector, uint64_t newSize);
void concatVectors(Vector* destination, Vector* source);
uint64_t vectorSize(Vector* vector);
