#version 130
in vec3 vertex;
in vec2 vertexUV;
out vec2 uv;
uniform mat4 MVP;
void main() {
     gl_Position = MVP * vec4(vertex, 1);
     uv = vertexUV;
}
