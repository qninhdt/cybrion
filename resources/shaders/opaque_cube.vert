#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 pass_uv;
layout (location = 2) in uint pass_tex_id;
layout (location = 3) in uint pass_face;
layout (location = 4) in uint pass_ao;

uniform mat4 MVP;

out vec2 uv;
flat out uint tex_id;
flat out uint face;
out float ao;

float ao_list[4] = { 0.55, 0.70, 0.85, 1 };

void main() {
    gl_Position = MVP * vec4(position, 1.0f);
	uv = pass_uv;
	tex_id = pass_tex_id;
	face = pass_face;
	ao = ao_list[pass_ao];
}