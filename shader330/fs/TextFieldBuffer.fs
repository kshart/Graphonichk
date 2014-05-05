#version 330 core
uniform sampler2D textTexture;

in VertexData {
	vec2 TexCoord;
	vec4 textColor;
} VertexIn;
out vec4 color;

void main () {
	/*"vec4 textBGColor = "
	"vec4 textBGColor = vec4(0.0);"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01 + vec2(0.0, -0.028))*0.0044299121055113265;"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01 + vec2(0.0, -0.024))*0.00895781211794;"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01 + vec2(0.0, -0.020))*0.0215963866053;"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01 + vec2(0.0, -0.016))*0.0443683338718;"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01 + vec2(0.0, -0.012))*0.0776744219933;"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01 + vec2(0.0, -0.008))*0.115876621105;"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01 + vec2(0.0, -0.004))*0.147308056121;"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01    )*0.159576912161;"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01 + vec2(0.0, 0.004))*0.147308056121;"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01 + vec2(0.0, 0.008))*0.115876621105;"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01 + vec2(0.0, 0.012))*0.0776744219933;"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01 + vec2(0.0, 0.016))*0.0443683338718;"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01 + vec2(0.0, 0.020))*0.0215963866053;"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01 + vec2(0.0, 0.024))*0.00895781211794;"
	"textBGColor += texture2D(textTexture,VertexIn.TexCoord + 0.01 + vec2(0.0, 0.028))*0.0044299121055113265;"
	"textBGColor *= 0.5;"*/
	float textAlpha = texture(textTexture, VertexIn.TexCoord).a;
	color = VertexIn.textColor;
	color.a *= textAlpha;
}