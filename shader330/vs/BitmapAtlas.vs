#version 330 core
uniform int rectID = -1;

layout(location = 0) in vec4 position;
layout(location = 1) in int attrRectID;

out VertexData {
	vec4 position;
	int rectID;
} VertexOut;

void main () {
	VertexOut.position = position;
	if (rectID<0) {
		VertexOut.rectID = attrRectID;
	}else{
		VertexOut.rectID = rectID;
	}
}
//35970 0 39240 6550
//65535
//65535
//0,54886701762417029068436713206683 0 0,59876401922636758983749141680018 0,09994659342336156252384222171359