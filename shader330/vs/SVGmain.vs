#version 330 core

const int RECT = 0xB7D63381;
const int CIRCLE = 0xD4B76579;

layout(shared) uniform grShaderData {
	mat4 viewMatrixValue;
};
uniform int typeShape;
uniform mat3x2 transformMatrix;

uniform vec4 fillColor;
uniform vec4 circleTransform;
//x = x position
//y = y position
//z = x scale
//w = y scale
in vec2 position;
out vec2 fragTexCoord;
void main () {
	switch (typeShape) {
		case CIRCLE:
			gl_Position = vec4(position.x*circleTransform.z+circleTransform.x, position.y*circleTransform.w+circleTransform.y, 0.0, 1.0)*viewMatrixValue;
			break;
		case RECT:
			gl_Position = vec4(position, 0.0, 1.0)*viewMatrixValue;
			break;
		default:
			gl_Position = vec4(position, 0.0, 1.0)*viewMatrixValue;
	}
};