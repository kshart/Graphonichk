#version 330 core
in vec4 position;

out VertexData {
	vec4 position;
} VertexOut;

void main () {
	VertexOut.position = position;
}
//35970 0 39240 6550
//65535
//65535
//0,54886701762417029068436713206683 0 0,59876401922636758983749141680018 0,09994659342336156252384222171359