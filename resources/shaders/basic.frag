#version 430

in vec3 color;
in float logz;
out vec4 out_color;

void main() {
    out_color = vec4(color, 1);
	gl_FragDepth = logz - 0.0005;
}