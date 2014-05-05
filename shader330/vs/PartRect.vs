#version 330 core
in vec4 position;

out VertexData {
	vec4 position;
} VertexOut;

void main () {
	VertexOut.position = position;
}