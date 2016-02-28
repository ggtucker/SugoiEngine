#version 330 core

layout(location = 0) in vec3 position;

out vec4 Color;

void main() {
	gl_Position = vec4(position, 1.0f);
	Color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}