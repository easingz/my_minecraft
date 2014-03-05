// -*- Mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

GLFWwindow* create_window() {
    // Know nothing about 4x antialiasing...
    glfwWindowHint(GLFW_SAMPLES, 4);
    // What's the purpose of this...hint glfw we need opengl 3.3 or higher?
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // What's this....
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_WIDTH, "First Window", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        exit(1);
    }
    return window;
}

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f,  -1.0f, 0.0f,
    0.0f,  1.0f,  0.0f,
};

int main(int argc, char **argv) {
    // create window
    if (!glfwInit()) {
        return -1;
    }
    glfwSetErrorCallback(error_callback);
    GLFWwindow* window = create_window();
    // Set 'window' as the opengl context
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    // init glew for platform opengl support check.
    if (glewInit() != GLEW_OK) {
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Following we only use this vertex array object to handle vertices.
    // If we don't create one, what happens?
    // The following gl command won't work?
    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    // first we should generate one buffer for our vertex data to load in.
    GLuint vertexBuffer;
    // only one buffer, the second para should be a GLuint* which could store many generated buffers.
    glGenBuffers(1, &vertexBuffer);
    // this buffer is for 'vertex' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // finally load the vertex data into buffer. What a lot of work...
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");
    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(programID);

        // Now tell OpenGL that we start to handle vertex array.
        glEnableVertexAttribArray(0);
        // tell openGl to use this vertex buffer.
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        // tell openGL the structure of this vertex buffer and how to parse it.
        // Parameter: shader 'layout', size, type, is normalized, stride, offset
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        // Draw this vertex buffer.
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        // Swap the back buffer with front buffer
        glfwSwapBuffers(window);
        // Get the events, non-block.
        glfwPollEvents();
        // glfwWaitEvents();
    }

    // Cleanup
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArrayId);
    glDeleteProgram(programID);

    // close window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

