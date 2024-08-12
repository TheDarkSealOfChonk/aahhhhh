#include "render.h"
#include "entity.h"
#include "resources.h"
#include "vector.h"
#include <stdint.h>

// Vertex Shader source code
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTex;\n"
    "out vec2 texCoord;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   texCoord = aTex;\n"
    "}\0";

// Fragment Shader source code
const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D tex0;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(tex0, texCoord);\n"
    "}\n\0";

GLFWwindow *window;

GLuint shaderProgram;
GLuint vao, vbo, ebo;
GLuint texture;
GLuint tex0Uniform;

Vector queueVertices;
Vector queueIndices;

Vector allTransformMatrices;

// Define the vertices of the rectangle
Vertex vertices[] = {
    {-0.1f, -0.05f, 0.0f, 0.0f, 1.0f},  // Bottom left
    { 0.1f, -0.05f, 0.0f, 0.2f, 1.0f},  // Bottom right
    { 0.1f,  0.05f, 0.0f, 0.2f, 0.0f},  // Top right
    {-0.1f,  0.05f, 0.0f, 0.0f, 0.0f}   // Top left
};

void initRenderer() {
    initVector(&queueVertices, sizeof(Vertex), 10, 0);
    initVector(&queueIndices, sizeof(GLuint), 10, 0);

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(400, 400, "Breakout", 0, 0);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, 400, 400);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define the indices for drawing the rectangle using two triangles
    GLuint indices[] = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    // GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gimp_image.width, gimp_image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, gimp_image.pixel_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    tex0Uniform = glGetUniformLocation(shaderProgram, "tex0");
    glUseProgram(shaderProgram);
    glUniform1i(tex0Uniform, 0);
}

void drawFrame() {
    //vertices[0].x += 0.01f;
    //vertices[1].x += 0.01f;
    //vertices[2].x += 0.01f;
    //vertices[3].x += 0.01f;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(shaderProgram);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void endRenderer() {
    glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteTextures(1, &texture);
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
}

void addPaddle(float x) {
    Vector verts;
    initVector(&verts, sizeof(Vertex), 20, 20);
    verts.data = (uint8_t*)vertices;

    ((Vertex*)indexVector(&verts, 0))->x += x;
    ((Vertex*)indexVector(&verts, 1))->x += x;
    ((Vertex*)indexVector(&verts, 2))->x += x;
    ((Vertex*)indexVector(&verts, 3))->x += x;

    ((Vertex*)indexVector(&verts, 0))->y -= 0.9;
    ((Vertex*)indexVector(&verts, 1))->y -= 0.9;
    ((Vertex*)indexVector(&verts, 2))->y -= 0.9;
    ((Vertex*)indexVector(&verts, 3))->y -= 0.9;

    concatVectors(&verts, &queueVertices);

    Vector inds;
    initVector(&inds, sizeof(GLuint), 6, 6);
    //()indexVector(&inds, 0)
}

GLFWwindow* getWindow() {
    return window;
}
