#version 450

in vec2 f_texCoord;
in vec4 f_color;

out vec4 FragColor;

uniform sampler2D u_tex;
uniform float u_time;

void main() {
	float psTime = clamp((((sin(u_time) + 1.0) / 2.0) + 0.05), 0.10, 1.0);
	vec2 texer = f_texCoord;
	float weirder = clamp(psTime * 4096.0, 128.0, 4096.0);
	texer *= vec2(weirder);
	texer = round(texer);
	texer /= vec2(weirder);
	vec4 texcolor = texture(u_tex, texer) * vec4(vec2(f_color), sin(u_time), 1.0);
	texcolor.rgb *= f_color.a;
	FragColor = texcolor;
}
