#version 430

in vec2 uv;
flat in uint tex_id;
flat in uint face;
in float ao;

uniform sampler2DArray texures;
uniform uint enable_diffuse;
uniform uint enable_ao;

float diffuse[6] = { 0.9f, 1.0f, 0.9f, 0.9f, 0.8f, 0.9f };

void main() {
	float factor = 1;
	
	if (enable_ao == 1) factor *= ao;
	if (enable_diffuse == 1) factor *= diffuse[face];
	
    gl_FragColor = factor * texture(texures, vec3(uv, tex_id));
}