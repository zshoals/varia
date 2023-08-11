#version 450

in vec2  v_pos;
in float v_layer;
in vec2  v_uv;
in vec4  v_color;

out vec2 f_texCoord;
out vec4 f_color;

uniform float u_time;

void main() {
	float pstime = (sin(u_time) + 1.0) / 2.0;
	gl_Position = vec4(v_pos.x, -v_pos.y + (pstime * 0.5), v_layer, 1.0);
	f_texCoord = v_uv;
	f_color = v_color;
}
