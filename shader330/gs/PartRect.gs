#version 330 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 24) out;

layout(shared) uniform grShaderData {
	mat4 viewMatrixValue;
};
uniform sampler2D colorTexture;
uniform sampler1D coordRect;
uniform ivec2 offset = ivec2(4, 4);
uniform int rectID = 10;

in VertexData {
	vec4 position;
} VertexIn[];

out vec2 TexCoord;

void main () {
	vec4 coord = texelFetch(coordRect, rectID, 0);
	vec2 TexSize = textureSize(colorTexture, 0);
	vec4 Box = VertexIn[0].position;
	vec2 p1, p2, p3, p4;
	p1 = Box.xy;
	p2 = p1+offset;
	p4 = Box.xy+Box.zw;
	p3 = p4-offset;
	vec2 c1, c2, c3, c4;
	c1 = coord.st;
	c4 = coord.pq;
	c2 = c1+vec2(offset)/TexSize;//vec2(c1.x, c1.y);
	c3 = c4-vec2(offset)/TexSize;//coord.zw;//c4-vec2(1/50, 1/25);
	
	//0
	gl_Position = vec4(p1.x, p1.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c1.x, c1.y);
	EmitVertex();
	//1
	gl_Position = vec4(p1.x, p2.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c1.x, c2.y);
	EmitVertex();
	//2
	gl_Position = vec4(p2.x, p1.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c2.x, c1.y);
	EmitVertex();
	//3
	gl_Position = vec4(p2.x, p2.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c2.x, c2.y);
	EmitVertex();
	//4
	gl_Position = vec4(p3.x, p1.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c3.x, c1.y);
	EmitVertex();
	//5
	gl_Position = vec4(p3.x, p2.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c3.x, c2.y);
	EmitVertex();
	//6
	gl_Position = vec4(p4.x, p1.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c4.x, c1.y);
	EmitVertex();
	//7
	gl_Position = vec4(p4.x, p2.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c4.x, c2.y);
	EmitVertex();
	
	EndPrimitive();
	
	
	gl_Position = vec4(p1.x, p2.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c1.x, c2.y);
	EmitVertex();
	
	gl_Position = vec4(p1.x, p3.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c1.x, c3.y);
	EmitVertex();
	
	gl_Position = vec4(p2.x, p2.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c2.x, c2.y);
	EmitVertex();
	
	gl_Position = vec4(p2.x, p3.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c2.x, c3.y);
	EmitVertex();
	/////////////
	gl_Position = vec4(p3.x, p2.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c3.x, c2.y);
	EmitVertex();
	
	gl_Position = vec4(p3.x, p3.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c3.x, c3.y);
	EmitVertex();
	
	gl_Position = vec4(p4.x, p2.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c4.x, c2.y);
	EmitVertex();
	
	gl_Position = vec4(p4.x, p3.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c4.x, c3.y);
	EmitVertex();
	
	EndPrimitive();
	
	
	gl_Position = vec4(p1.x, p3.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c1.x, c3.y);
	EmitVertex();
	
	gl_Position = vec4(p1.x, p4.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c1.x, c4.y);
	EmitVertex();
	
	gl_Position = vec4(p2.x, p3.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c2.x, c3.y);
	EmitVertex();
	
	gl_Position = vec4(p2.x, p4.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c2.x, c4.y);
	EmitVertex();
	/////////////
	gl_Position = vec4(p3.x, p3.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c3.x, c3.y);
	EmitVertex();
	
	gl_Position = vec4(p3.x, p4.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c3.x, c4.y);
	EmitVertex();
	
	gl_Position = vec4(p4.x, p3.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c4.x, c3.y);
	EmitVertex();
	
	gl_Position = vec4(p4.x, p4.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(c4.x, c4.y);
	EmitVertex();
	
	EndPrimitive();
};