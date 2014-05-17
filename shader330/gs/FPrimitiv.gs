#version 330 core
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;
layout(shared) uniform grShaderData {
	mat4 viewMatrixValue;
};

in VertexData {
	vec4 rect;
} VertexIn[];

void main () {
	vec4 Pos = VertexIn[0].rect;
	gl_Position = vec4(Pos.x, Pos.y, 0.0, 1.0)*viewMatrixValue;
	EmitVertex();
	
	gl_Position = vec4(Pos.x, Pos.y+Pos.w, 0.0, 1.0)*viewMatrixValue;
	EmitVertex();
	
	gl_Position = vec4(Pos.x+Pos.z, Pos.y, 0.0, 1.0)*viewMatrixValue;
	EmitVertex();
	
	gl_Position = vec4(Pos.x+Pos.z, Pos.y+Pos.w, 0.0, 1.0)*viewMatrixValue;
	EmitVertex();
	
	EndPrimitive();
}