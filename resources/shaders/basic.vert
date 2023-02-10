#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 _uv;
layout (location = 2) in uint _tex_id;

uniform mat4 MVP;

out vec2 uv;
flat out uint tex_id;

void main() {
    gl_Position = MVP * vec4(position, 1.0f);
	uv = _uv;
	tex_id = _tex_id;
}