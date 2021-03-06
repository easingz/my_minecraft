// -*- Mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "lodepng/lodepng.h"
#include "util.h"
#include "log.h"

static void flip_image_vertical(
    unsigned char *data, unsigned int width, unsigned int height)
{
    unsigned int size = width * height * 4;
    unsigned int stride = sizeof(char) * width * 4;
    unsigned char *new_data = (unsigned char*)malloc(sizeof(unsigned char) * size);
    for (unsigned int i = 0; i < height; i++) {
        unsigned int j = height - i - 1;
        memcpy(new_data + j * stride, data + i * stride, stride);
    }
    memcpy(data, new_data, size);
    free(new_data);
}

void load_png_texture(const char *file_name) {
    unsigned int error;
    unsigned char *data;
    unsigned int width, height;
    error = lodepng_decode32_file(&data, &width, &height, file_name);
    if (error) {
        LOG_MUST("error %u: %s\n", error, lodepng_error_text(error));
    }
    flip_image_vertical(data, width, height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    free(data);
}

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

static glm::mat4 getViewMatrix(){
        return ViewMatrix;
}
static glm::mat4 getProjectionMatrix(){
        return ProjectionMatrix;
}

glm::vec3 position;
float horizontalAngle, verticalAngle, FoV, speed, mouseSpeed;
int width, height;

static void my_scrollCallback(GLFWwindow* w, double x, double y){
    FoV = FoV - 0.1 * y;
}

static void my_windowSizeCallback(GLFWwindow* win, int w, int h) {
    width = w; height = h;
}

void initPlayControl() {
    // Initial position : on +Z
    position = glm::vec3( 0, 2, 30 );
    // Initial horizontal angle : toward -Z
    horizontalAngle = 3.14f;
    // Initial vertical angle : none
    verticalAngle = 0.0f;
    // Initial Field of View
    FoV = 45.0f;
    speed = 5.0f; // 3 units / second
    mouseSpeed = 0.005f;
    glfwGetWindowSize(window, &width, &height);
    glfwSetCursorPos(window, width/2.0f, height/2.0f);
    glfwSetScrollCallback(window, my_scrollCallback);
    glfwSetWindowSizeCallback(window, my_windowSizeCallback);
}

static void computeMatricesFromInputs(){

        // glfwGetTime is called only once, the first time this function is called
        static double lastTime = glfwGetTime();

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        // Get mouse position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Reset mouse position for next frame
        glfwSetCursorPos(window, width/2, height/2);

        // WORKAROUND: glfwGetCursorPos() bug...
        // See https://github.com/glfw/glfw/issues/129
        // If they are all zeros, the camera is spinning at the beginning until cursor moves.
        if (xpos == 0.0f && ypos == 0.0f) return;

        // Compute new orientation
        horizontalAngle += mouseSpeed * float( width/2 - xpos );
        verticalAngle   += mouseSpeed * float( height/2 - ypos );

        // TODO: add log function for debug.
        LOG_SPECIAL("w: %d h: %d xpos: %f ypos: %f h: %f v %f\n", width, height, xpos, ypos, horizontalAngle, verticalAngle);

        // lock it to disable upside-down.
        if (verticalAngle >= 3.14f/2) verticalAngle = 3.14f/2;
        if (verticalAngle <= -3.14f/2) verticalAngle = -3.14f/2;

        // Direction : Spherical coordinates to Cartesian coordinates conversion
        glm::vec3 viewZ_neg(
                cos(verticalAngle) * sin(horizontalAngle),
                sin(verticalAngle),
                cos(verticalAngle) * cos(horizontalAngle)
        );
       
        // Right vector
        glm::vec3 viewX = glm::vec3(
                sin(horizontalAngle - 3.14f/2.0f),
                0,
                cos(horizontalAngle - 3.14f/2.0f)
        );
       
        // Up vector
        glm::vec3 up = glm::cross( viewX, viewZ_neg );

        // Move forward
        if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
                position += viewZ_neg * deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
                position -= viewZ_neg * deltaTime * speed;
        }
        // Strafe right
        if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
                position += viewX * deltaTime * speed;
        }
        // Strafe left
        if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
                position -= viewX * deltaTime * speed;
        }

        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
        // Camera matrix
        ViewMatrix = glm::lookAt(
				       position,           // Camera is here
				       position+viewZ_neg, // and looks here : at the same position, plus "viewZ_neg"
				       up                  // Head is up (set to 0,-1,0 to look upside-down)
				       );

        // For the next frame, the "last time" will be "now"
        lastTime = currentTime;
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

