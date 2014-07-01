#version 330 core
layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;
void main () {
	vec3 Pos = gl_in[0].gl_Position.xyz;
	gl_Position = vec4(gl_in[0].gl_Position.xy, gl_in[0].gl_Position.z-4, 1.0);
	EmitVertex();
	
	gl_Position = vec4(gl_in[1].gl_Position.xy, gl_in[1].gl_Position.z-4, 1.0);
	EmitVertex();

	gl_Position = vec4(gl_in[2].gl_Position.xy, gl_in[2].gl_Position.z-4, 1.0);
	EmitVertex();

	EndPrimitive();
}