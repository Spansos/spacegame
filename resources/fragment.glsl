#version 400 core

in vec3 position;
in vec3 normal;
in vec3 color;

out vec3 out_color;

void main( ) {
	out_color = color * (dot(normal,vec3(1,1,1))/3+1)/2;
}