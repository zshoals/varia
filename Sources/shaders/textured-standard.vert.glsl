#version 450

in vec3 vertexPosition;
in vec2 vertexUV;
in vec4 vertexColor;
uniform mat4 projectionMatrix;
uniform float vTime;
out vec2 texCoord;
out vec4 color;

void main() {
	//gl_Position = projectionMatrix * vec4(vertexPosition, 1.0);
	float pstime = (sin(vTime) + 1.0) / 2.0;
	gl_Position = vec4(vertexPosition.x, -vertexPosition.y + (pstime * 0.5), vertexPosition.z, 1.0);
	texCoord = vertexUV;
	color = vertexColor;
}
