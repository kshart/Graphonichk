#version 330 core
layout(shared) uniform grShaderData {
	mat4 viewMatrixValue;
};
uniform mat4 transformMatrix;
in vec3 position;
out vec3 w_position;
out vec3 c_normal;
out vec3 c_eye;
out vec3 c_light;
void main () {
	//"cameraPos = vec3(0, 0, 0);
	w_position = (vec4(position, 1)*transformMatrix).xyz;
	gl_Position = vec4(w_position, 1)*viewMatrixValue;
	//c_eye = 0 - (vec4(position, 1)*viewMatrixValue*transformMatrix).xyz;
	//c_light = normalize((vec4(0, 0, 0, 1)*viewMatrixValue).xyz+c_eye);
	//c_normal = normalize((vec4(position.xyz, 0)*transformMatrix*viewMatrixValue).xyz);
}