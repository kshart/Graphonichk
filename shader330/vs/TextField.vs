#version 330 core
in vec3 position;

out VertexData {
	vec3 position;
} VertexOut;

void main () {
	VertexOut.position = position;
}