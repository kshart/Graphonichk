#version 330 core
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;
uniform sampler2D img;
out vec2 TexCoord;
void main () {
	ivec2 winSize = textureSize(img, 0);
	gl_Position = vec4(-1, -1, 0, 1);
	TexCoord = vec2(0.0, 0.0);
	EmitVertex();
	
	gl_Position = vec4(-1, 1, 0, 1);
	TexCoord = vec2(0.0, 1.0);
	EmitVertex();
	
	gl_Position = vec4(1, -1, 0, 1);
	TexCoord = vec2(1.0, 0.0);
	EmitVertex();
	
	gl_Position = vec4(1, 1, 0, 1);
	TexCoord = vec2(1.0, 1.0);
	EmitVertex();
	
	EndPrimitive();
}