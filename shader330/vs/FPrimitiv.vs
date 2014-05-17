#version 330 core
in vec4 position;

out VertexData {
	vec4 rect;
} VertexOut;

void main () {
	VertexOut.rect = position;
}