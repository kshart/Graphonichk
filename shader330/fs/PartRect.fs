#version 330 core
uniform sampler2D colorTexture;
//uniform vec2 offset;

in vec2 TexCoord;

out vec4 color;

void main () {
	ivec2 TexSize = textureSize(colorTexture, 0);
	//float vox = 0.1;
	color = texture(colorTexture, TexCoord);
}