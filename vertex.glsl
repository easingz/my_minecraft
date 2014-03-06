#version 130
in vec3 vertex;
in vec3 color;
uniform mat4 MVP;
out vec3 fragment_color;
void main() {
     gl_Position = MVP * vec4(vertex, 1);
     fragment_color = color;
}
