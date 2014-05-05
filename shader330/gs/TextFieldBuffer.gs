#version 330 core
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;
layout(shared) uniform grShaderData {
	mat4 viewMatrixValue;
	vec2 pixelSize;
};
in VertexData {
	vec4 position;
	vec4 textColor;
} VertexIn[];
out VertexData {
	vec2 TexCoord;
	vec4 textColor;
} VertexOut;

uniform sampler2D textTexture;

void main () {
	ivec2 texSize = textureSize(textTexture, 0);
	vec2 Pos = VertexIn[0].position;
	gl_Position = vec4(Pos.x, Pos.y, 0.0, 1.0)*viewMatrixValue;
	VertexOut.TexCoord = vec2(0.0, 0.0);
	VertexOut.textColor = VertexIn[0].textColor;
	EmitVertex();
	
	gl_Position = vec4(Pos.x, Pos.y+texSize.y, 0.0, 1.0)*viewMatrixValue;
	VertexOut.TexCoord = vec2(0.0, 1.0);
	VertexOut.textColor = VertexIn[0].textColor;
	EmitVertex();
	
	gl_Position = vec4(Pos.x+texSize.x, Pos.y, 0.0, 1.0)*viewMatrixValue;
	VertexOut.TexCoord = vec2(1.0, 0.0);
	VertexOut.textColor = VertexIn[0].textColor;
	EmitVertex();
	
	gl_Position = vec4(Pos.x+texSize.x, Pos.y+texSize.y, 0.0, 1.0)*viewMatrixValue;
	VertexOut.TexCoord = vec2(1.0, 1.0);
	VertexOut.textColor = VertexIn[0].textColor;
	EmitVertex();
	
	EndPrimitive();
}