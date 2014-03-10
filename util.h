#ifndef _util_h_
#define _util_h_

#include <glm/glm.hpp>

#define PI 3.14159265359
#define RADIAN(degree) (float)((degree) * PI / 180)
extern GLFWwindow* window;

void load_png_texture(const char *file_name);

void initPlayControl();
void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif
