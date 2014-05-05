#version 330 core
uniform sampler2D texture;
in vec2 TexCoord;
out vec4 color;
void main () {
	color = texture(texture, TexCoord);
}