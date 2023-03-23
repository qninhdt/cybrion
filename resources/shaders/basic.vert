#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 _color;

uniform mat4 MVP;

out vec3 color;
out float logz;

void main() {
    gl_Position = MVP * vec4(position, 1.0f);
	
	// logarithmic depth buffers
	// https://outerra.blogspot.com/2012/11/maximizing-depth-buffer-range-and.html
	const float C = 1;
	const float far = 1500;
	const float FC = 1.0/log(far*C + 1);
	
	gl_Position.z = (2.0*log(gl_Position.w*C + 1))/log(far*C+1) - 1; 
    gl_Position.z *= gl_Position.w;
	logz = log(gl_Position.w*C + 1)*FC;
	
	color = _color;
}