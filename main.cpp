// -*- Mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#include "util.h"
#include "game_object.h"

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
    glm::mat4 model = glm::translate(glm::mat4(1.0f), world_coord);
    // world --> camera
    glm::mat4 view = getViewMatrix();
    // camera --> FoV
    glm::mat4 projection = getProjectionMatrix();
    return projection * view * model;
}

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

GLfloat* gen_cube_uvs() {
    GLfloat* ret = (GLfloat*)malloc(OBJECT_COUNT * 72 * sizeof(GLfloat));
    for (int i = 0; i < OBJECT_COUNT; i++) {
        get_object_texture_uv((t_object) (i + 1), &ret[i * 72]);
    }

    // for (int i = 0; i < OBJECT_COUNT; i++) {
    //     printf("###  texture %d  ###\n", i);
    //     for (int j = 0; j < 72; j+=2) {
    //         printf("%f %f\n", texture_uv_data[i * 72 + j], texture_uv_data[i * 72 + j + 1]);
    //     }
    // }
    return ret;
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
    printf("### start draw_cube\n");
    for (int i = 0; i < buf_size; i+=2) {
        printf("    %f %f\n", texture_uv_buf[i], texture_uv_buf[i+1]);
    }

    // need do the transformation in render function.
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(c->vao);

    // TODO: the uniform index should also be an member of struct cube. no need to query here every time.
    glUseProgram(c->shader_program);
    glm::mat4 mvp = getMVPMatrix(world_coord);
    glUniformMatrix4fv(c->shader_mvp, 1, GL_FALSE, &mvp[0][0]);

    // Dynamically bind cube texture.
    glBindBuffer(GL_ARRAY_BUFFER, c->texture_buffer);
    glBufferData(GL_ARRAY_BUFFER, buf_size, texture_uv_buf, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, c->vertex_size);
    glBindVertexArray(0);
}

void draw_cubes(cube* c, GLfloat* cube_textures) {
    for (int i = 0; i < 33; i+=6)
        for (int j = 0; j < 33; j+=6)
            for (int k = 0; k < 33; k+=6) {
                draw_cube(c, glm::vec3(i - 16, j - 16, k - 16),
                          &cube_textures[((i+j+k) % OBJECT_COUNT) * 72],
                          72);
            }
}

void delete_cube(cube* c) {
    glDeleteBuffers(1, &c->vertex_buffer);
    glDeleteBuffers(1, &c->color_buffer);
    glDeleteTextures(1, &c->texture_buffer);
    glDeleteVertexArrays(1, &c->vao);
    glDeleteProgram(c->shader_program);
    free(c);
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
    GLfloat* texture_uv_data = gen_cube_uvs();
    cube* c = make_cube();

    //////////// Main Loop ////////////
    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        draw_cubes(c, texture_uv_data);

        glfwSwapBuffers(window);
        // Get the events, non-block.
        glfwPollEvents();
        // glfwWaitEvents();
    }
    //////////// Loop End /////////////

    // Cleanup
    delete_cube(c);
    free(texture_uv_data);

    // close window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
