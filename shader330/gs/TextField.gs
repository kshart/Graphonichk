#version 330 core
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

layout(shared) uniform grShaderData {
	mat4 viewMatrixValue;
};
uniform sampler1D coordTex;
uniform sampler2D texture;

in VertexData {
	vec3 position;
} VertexIn[];

out vec2 coord;

void main () {
	int index = int(VertexIn[0].position.z);
	ivec2 sizeTex = textureSize(texture, 0);
	vec4 rectTex = texelFetch(coordTex, index, 0);
	vec4 rect = vec4(VertexIn[0].position.x, VertexIn[0].position.y, VertexIn[0].position.x+sizeTex.x*rectTex.z, VertexIn[0].position.y+sizeTex.y*rectTex.w);
	
	gl_Position = vec4(rect.x, rect.y, 0.0, 1.0)*viewMatrixValue;
	coord = vec2(float(rectTex.x), float(rectTex.y));
	EmitVertex();
	
	gl_Position = vec4(rect.x, rect.w, 0.0, 1.0)*viewMatrixValue;
	coord = vec2(float(rectTex.x), float(rectTex.y+rectTex.w));
	EmitVertex();
	
	gl_Position = vec4(rect.z, rect.y, 0.0, 1.0)*viewMatrixValue;
	coord = vec2(float(rectTex.x+rectTex.z), float(rectTex.y));
	EmitVertex();
	
	gl_Position = vec4(rect.z, rect.w, 0.0, 1.0)*viewMatrixValue;
	coord = vec2(float(rectTex.x+rectTex.z), float(rectTex.y+rectTex.w));
	EmitVertex();
	
	EndPrimitive();
}