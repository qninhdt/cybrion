#version 430

in float logz;
in vec3 color;
in vec2 tex;
in vec3 normal;
flat in uint tex_id;

out vec4 out_color;

uniform uint use_light;
uniform sampler2DArray texures;

void main() {
	vec3 lightDir = normalize(vec3(-1.5, 2, -1.2));
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1, 1, 1) * 1.5;

    out_color = texture(texures, vec3(tex, tex_id));
	
	if (use_light == 1)
		out_color = vec4(diffuse * out_color.xyz, out_color.w);

	if (out_color.w < 0.5)
		discard;
}