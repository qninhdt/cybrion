#version 430

out vec4 FragColor;
in vec3 TexCoords;

void main()
{    
    FragColor = vec4(1, 1, 1-min(0.25, 0.08f/abs(TexCoords.y)), 1);
}