#version 430

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 pass_tex;
layout (location = 2) in vec3 pass_normal;
layout (location = 3) in uint pass_tex_id;

uniform mat4 MVP;

out float logz;
out vec2 tex;
out vec3 normal;
flat out uint tex_id;

void main()
{
	gl_Position = MVP * vec4(pos, 1.0f);
	
	tex = pass_tex;
	normal = pass_normal;
	tex_id = pass_tex_id;
}
