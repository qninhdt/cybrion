#version 430

in vec2 uv;
in float ao;
in float logz;
flat in uint tex_id;
flat in uint normal;

uniform sampler2DArray texures;
uniform uint enable_diffuse;
uniform uint enable_ao;

out vec4 color;

float diffuse_top = 1.0f;
float diffuse_side = 0.80f;
float diffuse_bottom = 0.6f;

float diffuse[6] = { 
	diffuse_side, 
	diffuse_top, 
	diffuse_side, 
	diffuse_side, 
	diffuse_bottom, 
	diffuse_side 
};

void main() {
	float factor = 1;
	float ao_ = 1-(ao-1)*(ao-1)/2;

	if (enable_ao == 1) factor *= ao_;
	if (enable_diffuse == 1) factor *= diffuse[normal];
	
    color = texture(texures, vec3(uv, tex_id));
	
	if (color.w < 0.1)
		discard;
	
	color = vec4(color.xyz * factor, color.w);
	gl_FragDepth = logz;
}