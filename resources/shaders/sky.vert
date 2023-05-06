#version 430

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection_view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection_view * vec4(aPos, 1.0);
    const float C = 1;
	const float far = 1500;
	const float FC = 1.0/log(far*C + 1);
	
	pos.z = (2.0*log(pos.w*C + 1))/log(far*C+1) - 1; 
    pos.z *= pos.w;

    gl_Position = pos.xyww;
}  