#version 330 core
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

layout(shared) uniform grShaderData {
	mat4 viewMatrixValue;
};
uniform sampler2D colorTexture;

out vec2 TexCoord;
void main () {
	vec4 Pos = gl_in[0].gl_Position;
	gl_Position = vec4(Pos.x, Pos.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(0.0, 0.0);
	EmitVertex();
	
	gl_Position = vec4(Pos.x, Pos.y+Pos.w, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(0.0, 1.0);
	EmitVertex();
	
	gl_Position = vec4(Pos.x+Pos.z, Pos.y, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(1.0, 0.0);
	EmitVertex();
	
	gl_Position = vec4(Pos.x+Pos.z, Pos.y+Pos.w, 0.0, 1.0)*viewMatrixValue;
	TexCoord = vec2(1.0, 1.0);
	EmitVertex();
	
	EndPrimitive();
}