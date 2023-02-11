#version 430

in vec2 uv;
flat in uint tex_id;

uniform sampler2DArray texures;

void main() {
    gl_FragColor = texture(texures, vec3(uv, tex_id));
	//gl_FragColor = vec4(uv, 0, 1);
}