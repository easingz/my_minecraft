// -*- Mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#include "util.h"
#include "game_object.h"
#include "map.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
GLFWwindow* window = NULL;

typedef struct {
    GLuint vao;
    GLuint vertex_buffer;
    GLuint color_buffer;
    GLuint texture_buffer;
    GLuint shader_program;
    GLuint shader_mvp;
    unsigned int vertex_size;
} cube;

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

GLFWwindow* create_window(int w, int h) {
    // Know nothing about 4x antialiasing...
    glfwWindowHint(GLFW_SAMPLES, 4);
    // What's the purpose of this...hint glfw we need opengl 3.3 or higher?
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // What's this....
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(w, h, "First Window", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        exit(1);
    }
    return window;
}

glm::mat4 getMVPMatrix(glm::vec3 world_coord) {
    computeMatricesFromInputs();
    // model --> world
    // T * R * S
    glm::mat4 model = glm::translate(glm::mat4(1.0f), world_coord) * glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));
    // world --> camera
    glm::mat4 view = getViewMatrix();
    // camera --> FoV
    glm::mat4 projection = getProjectionMatrix();
    return projection * view * model;
}

GLuint load_cube_texture() {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    load_png_texture("./textures/texture.png");
    return textureID;
}

cube* make_cube() {
    // printf("start make cube");
    cube* ret_cube = (cube*) malloc(sizeof(cube));
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    ret_cube->vao = vao;

    GLuint vertex_buf;
    glGenBuffers(1, &vertex_buf);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    ret_cube->vertex_buffer = vertex_buf;
    ret_cube->vertex_size = sizeof(g_vertex_buffer_data)/sizeof(g_vertex_buffer_data[0]);

    GLuint uv_buf;
    glGenBuffers(1, &uv_buf);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buf);
    ret_cube->texture_buffer = uv_buf;

    // TODO: refactor the shader load interface like:
    // std::vector(GLunit) shaders;
    // shaders.push_back(loadShader("vertex.glsl"));
    // shaders.push_back(loadShader("fragment.glsl"));
    // GLuint program = make_program(shaders);
    ret_cube->shader_program = LoadShaders("vertex.glsl", "fragment.glsl");
    ret_cube->shader_mvp = glGetUniformLocation(ret_cube->shader_program, "MVP");

    GLuint shader_sampler = glGetUniformLocation(ret_cube->shader_program, "sampler");
    // TODO: don't hardcode texture index. using a map like data structure to manage them.
    glUniform1i(shader_sampler, 0);

    GLint vertexLoc = glGetAttribLocation(ret_cube->shader_program, "vertex");
    GLint uvLoc = glGetAttribLocation(ret_cube->shader_program, "vertexUV");
    glEnableVertexAttribArray(vertexLoc);
    glEnableVertexAttribArray(uvLoc);

    glBindBuffer(GL_ARRAY_BUFFER, ret_cube->vertex_buffer);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, ret_cube->texture_buffer);
    glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
    // printf("make cube done!\n");
    return ret_cube;
}

void draw_cube(cube* c, glm::vec3 world_coord, const GLfloat* texture_uv_buf, const GLint buf_size) {
    // need do the transformation in render function.
    // TODO: the uniform index should also be an member of struct cube. no need to query here every time.
    glm::mat4 mvp = getMVPMatrix(world_coord);
    glUniformMatrix4fv(c->shader_mvp, 1, GL_FALSE, &mvp[0][0]);

    // Dynamically bind cube texture.
    glBindBuffer(GL_ARRAY_BUFFER, c->texture_buffer);
    glBufferData(GL_ARRAY_BUFFER, buf_size, texture_uv_buf, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, c->vertex_size);
}

void draw_cubes(cube* c) {
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(c->vao);
    glUseProgram(c->shader_program);
    GLfloat* object_uvs;
    int uv_size;
    for (int i = 0; i < 33; i+=6)
        for (int j = 0; j < 33; j+=6)
            for (int k = 0; k < 33; k+=6) {
                get_object_uvs((t_Object)((i+j+k) % OBJECT_COUNT), &object_uvs, &uv_size);
                draw_cube(c, glm::vec3(i - 16, j - 16, - k), object_uvs, uv_size);
            }
    glBindVertexArray(0);
}

void delete_cube(cube* c) {
    glDeleteBuffers(1, &c->vertex_buffer);
    glDeleteBuffers(1, &c->color_buffer);
    glDeleteTextures(1, &c->texture_buffer);
    glDeleteVertexArrays(1, &c->vao);
    glDeleteProgram(c->shader_program);
    free(c);
}

void draw_chunk(cube* c, MapChunk* mc) {
    // TODO(important!): hidden faces removal
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(c->vao);
    glUseProgram(c->shader_program);
    MapUnit* mu;
    GLfloat* object_uvs;
    int uv_size;
    // origin:left-back-bottom position
    glm::vec3 chunk_off(mc->dx * 256, mc->dy *256, mc->dz * 256);
    for (int i = 0; i < mc->pos; i++) {
        mu = &(mc->data[i]);
        get_object_uvs((t_Object)mu->u.w, &object_uvs, &uv_size);
        draw_cube(c, glm::vec3(mu->u.x, mu->u.y, mu->u.z) + chunk_off, object_uvs, uv_size);
    }
    glBindVertexArray(0);
}

int main(int argc, char **argv) {
    // create window
    if (!glfwInit()) {
        return -1;
    }
    glfwSetErrorCallback(error_callback);
    int width = WINDOW_WIDTH;
    int height = WINDOW_HEIGHT;
    window = create_window(width, height);
    // Set 'window' as the opengl context
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    // init glew for platform opengl support check.
    if (glewInit() != GLEW_OK) {
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    initPlayControl();
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    load_cube_texture();
    init_object_uvs();
    cube* c = make_cube();
    MapChunk* mc = create_random_chunk(0, 0, -1);

    //////////// Main Loop ////////////
    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // draw_cubes(c);
        draw_chunk(c, mc);

        glfwSwapBuffers(window);
        // Get the events, non-block.
        glfwPollEvents();
        // glfwWaitEvents();
    }
    //////////// Loop End /////////////

    // Cleanup
    delete_cube(c);
    delete_object_uvs();
    delete_chunk(mc);

    // close window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
