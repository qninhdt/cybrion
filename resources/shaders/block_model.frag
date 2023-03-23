#version 430

in float logz;
in vec3 color;
in vec2 tex;
in vec3 normal;
flat in uint tex_id;

out vec4 out_color;

uniform sampler2DArray texures;

void main() {
    out_color = texture(texures, vec3(tex, tex_id));
	
	if (out_color.w < 0.5)
		discard;
		
	gl_FragDepth = logz;
}