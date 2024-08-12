#pragma once

#include <cglm/types-struct.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "resources.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <cglm/struct.h>
#include "vector.h"
#include "entity.h"

typedef struct Vertex {
    float x, y, z;
    float u, v;
} Vertex;

void initRenderer();
void drawFrame();
void endRenderer();
GLFWwindow* getWindow();
