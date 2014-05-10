#version 330 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

layout(shared) uniform grShaderData {
	mat4 viewMatrixValue;
};
uniform sampler2D colorTexture;
uniform sampler1D coordRect;

in VertexData {
	vec4 position;
	int rectID;
} VertexIn[];

out vec2 TexCoord;

void main () {
	ivec2 texSize = textureSize(colorTexture, 0);
	vec4 coord = texelFetch(coordRect, VertexIn[0].rectID, 0);
	vec4 Pos = VertexIn[0].position;
	
	gl_Position = vec4(Pos.x, Pos.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(coord.x, coord.y);
	EmitVertex();
	
	gl_Position = vec4(Pos.x, Pos.y+Pos.w, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(coord.x, coord.w);
	EmitVertex();
	
	gl_Position = vec4(Pos.x+Pos.z, Pos.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(coord.z, coord.y);
	EmitVertex();
	
	gl_Position = vec4(Pos.x+Pos.z, Pos.y+Pos.w, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(coord.z, coord.w);
	EmitVertex();
	
	EndPrimitive();
};