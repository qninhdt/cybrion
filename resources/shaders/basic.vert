#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 _color;

uniform mat4 MVP;

out vec3 color;

void main() {
    gl_Position = MVP * vec4(position, 1.0f);
	color = _color;
}