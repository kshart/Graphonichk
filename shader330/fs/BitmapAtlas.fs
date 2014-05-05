#version 330 core
uniform sampler2D colorTexture;

in vec2 TexCoord;

out vec4 color;

void main () {
	color = texture(colorTexture, TexCoord);
}