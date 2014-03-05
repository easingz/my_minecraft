#version 130
in vec3 v;
void main() {
     gl_Position.xyz = v;
     gl_Position.w = 1.0;
}