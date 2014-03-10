// -*- Mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lodepng/lodepng.h"
#include "util.h"

void load_png_texture(const char *file_name) {
    unsigned int error;
    unsigned char *data;
    unsigned int width, height;
    error = lodepng_decode24_file(&data, &width, &height, file_name);
    if (error) {
        fprintf(stderr, "error %u: %s\n", error, lodepng_error_text(error));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);
}

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
        return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
        return ProjectionMatrix;
}

glm::vec3 position;
float horizontalAngle, verticalAngle, FoV, speed, mouseSpeed;

void my_scrollCallback(GLFWwindow* w, double x, double y){
    FoV = FoV - 0.1 * y;
}

void initPlayControl() {
    // Initial position : on +Z
    position = glm::vec3( 0, 0, 20 );
    // Initial horizontal angle : toward -Z
    horizontalAngle = 0.0f;
    // Initial vertical angle : none
    verticalAngle = 0.0f;
    // Initial Field of View
    FoV = 45.0f;

    speed = 3.0f; // 3 units / second
    mouseSpeed = 0.005f;
    glfwSetScrollCallback(window, my_scrollCallback);
}

void computeMatricesFromInputs(){

        // glfwGetTime is called only once, the first time this function is called
        static double lastTime = glfwGetTime();

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        // Get mouse position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Reset mouse position for next frame
        glfwSetCursorPos(window, 1024/2, 768/2);

        // Compute new orientation
        horizontalAngle += mouseSpeed * float(1024/2 - xpos );
        verticalAngle   += mouseSpeed * float( 768/2 - ypos );

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


