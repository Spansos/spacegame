#version 400 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec3 vertex_color;

out vec3 position;
out vec3 normal;
out vec3 color;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vertex_position,1);
    position = vertex_position;
    normal = vertex_normal;
    color = vertex_color;
}