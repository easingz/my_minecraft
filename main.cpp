// -*- Mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define PI 3.141592653
static float degree2radian(float d) {
    return d * 2 * PI / 360;
}

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

glm::mat4 getMVPMatrix() {
    // model --> world
    // identity matrix, just put the model orgin as the world origin
    glm::mat4 model = glm::mat4(1.0f);
    // world --> camera
    glm::mat4 view = glm::lookAt(
                                 glm::vec3(4, 3, 3), // Camera is at(4, 3, 3), in world coord.
                                 glm::vec3(0, 0, 0), // and looks at the origin
                                 glm::vec3(0, 1, 0) // Head is up
                                 );
    // camera --> FoV
    // 45 degree Field of view, 4:3 ratio, display range: 0.1 unit <-> 100 units
    glm::mat4 projection = glm::perspective(degree2radian(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
    return projection * view * model;
}

// static const GLfloat g_vertex_buffer_data[] = {
//     -1.0f, -1.0f, 0.0f,
//     1.0f,  -1.0f, 0.0f,
//     0.0f,  1.0f,  0.0f,
// };

// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

// One color for each vertex. They were generated randomly.
static const GLfloat g_color_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
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
    // the color buffer
    // actually we can alocate an int array to siplify this seperated buffer declaration.
    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    // TODO: refactor the shader load interface like:
    // std::vector(GLunit) shaders;
    // shaders.push_back(loadShader("vertex.glsl"));
    // shaders.push_back(loadShader("fragment.glsl"));
    // GLuint program = make_program(shaders);
    GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");

    glm::mat4 MVP;
    GLuint shader_mvp = glGetUniformLocation(programID, "MVP");

    // TODO: seperate the render function with this loop, make it a callback or interface.
    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programID);
        MVP = getMVPMatrix();
        glUniformMatrix4fv(shader_mvp, 1, GL_FALSE, &MVP[0][0]);


        // VAO is actually always bound to a geometry, which has vertex, color, normal, lightmap data etc.
        // VAO can contains several attributes, each one has index assigned.
        // this index can be vertex shader input through 'attribute' qualifier and glBindAttribLocation().
        // each attrib index is bound to a buffer.
        // each buffer contains the actual data which describe the actually vertices, colors etc.
        // we can have many VAOs, use glBindVertexArray(vaoID) and disable it by glBindVertexArray(0)

        // Here we just have one VAO bound and never changed.
        // Now tell OpenGL that we start to handle vertex array.
        // enable the first attribute of VAO
        // Fuck glsl 1.20, coz there is no 'location' qualifier.
        GLint vertexLoc = glGetAttribLocation(programID, "vertex");
        GLint colorLoc = glGetAttribLocation(programID, "color");
        glEnableVertexAttribArray(vertexLoc);
        // tell openGl to use this vertex buffer to fill attribute index 0.
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        // tell openGL the structure of this vertex buffer and how to parse it.
        // Parameter: shader 'layout', size, type, is normalized, stride, offset
        glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        // second attribute which describe the color of the geometry.
        glEnableVertexAttribArray(colorLoc);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        // Alternative way to bind index to shader attribute
        // glBindAttribLocation(programID, 0, "vertex");
        // glBindAttribLocation(programID, 1, "color");
        // glLinkProgram(programID);

        // Draw this vertex buffer.
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // disable the first attribute
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

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

