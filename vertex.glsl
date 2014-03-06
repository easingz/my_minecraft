#version 130
in vec3 v;
uniform mat4 MVP;
void main() {
     vec4 v = vec4(v, 1);
     gl_Position = MVP * v;
}