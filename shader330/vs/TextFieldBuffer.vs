#version 330 core
in vec4 position;
in vec4 textColor;

out VertexData {
	vec4 position;
	vec4 textColor;
} VertexOut;

void main () {
	VertexOut.position = position;
	VertexOut.textColor = textColor;
}