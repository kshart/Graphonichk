#version 330 core
uniform vec4 fillColor;
out vec4 color;
in vec3 w_position;
in vec3 c_normal;
in vec3 c_eye;
in vec3 c_light;
void main () {
	float cosTheta = clamp( dot( c_normal,c_light ), 0, 1 );
	color = vec4(0, 1, 0, 1);//vec4( fillColor.xyz*max(cosTheta, 0.3), fillColor.w);
}