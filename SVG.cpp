#include "grBaseTypes.h"
#include "SVG.h"
#include "grMatrix.h"
using namespace SVG;
using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
const DataTypes::ColorWord DataTypes::colorWords[147] = {
	{"aliceblue",		240, 248, 255},
	{"antiquewhite",	250, 235, 215},
	{"aqua",			  0, 255, 255},
	{"aquamarine",		127, 255, 212},
	{"azure",			240, 255, 255},
	{"beige",			245, 245, 220},
	{"bisque",			255, 228, 196},
	{"black",			  0,   0,   0},
	{"blanchedalmond",	255, 235, 205},
	{"blue",			  0,   0, 255},
	{"blueviolet",		138,  43, 226},
	{"brown",			165,  42,  42},
	{"burlywood",		222, 184, 135},
	{"cadetblue",		 95, 158, 160},
	{"chartreuse",		127, 255,   0},
	{"chocolate",		210, 105,  30},
	{"coral",			255, 127,  80},
	{"cornflowerblue",	100, 149, 237},
	{"cornsilk",		255, 248, 220},
	{"crimson",			220,  20,  60},
	{"cyan",			  0, 255, 255},
	{"darkblue",		  0,   0, 139},
	{"darkcyan",		  0, 139, 139},
	{"darkgoldenrod",	184, 134,  11},
	{"darkgray",		169, 169, 169},
	{"darkgreen",		  0, 100,   0},
	{"darkgrey",		169, 169, 169},
	{"darkkhaki",		189, 183, 107},
	{"darkmagenta",		139,   0, 139},
	{"darkolivegreen",	 85, 107,  47},
	{"darkorange",		255, 140,   0},
	{"darkorchid",		153,  50, 204},
	{"darkred",			139,   0,   0},
	{"darksalmon",		233, 150, 122},
	{"darkseagreen",	143, 188, 143},
	{"darkslateblue",	 72,  61, 139},
	{"darkslategray",	 47,  79,  79},
	{"darkslategrey",	 47,  79,  79},
	{"darkturquoise",	  0, 206, 209},
	{"darkviolet",		148,   0, 211},
	{"deeppink",		255,  20, 147},
	{"deepskyblue",		  0, 191, 255},
	{"dimgray",			105, 105, 105},
	{"dimgrey",			105, 105, 105},
	{"dodgerblue",		 30, 144, 255},
	{"firebrick",		178,  34,  34},
	{"floralwhite",		255, 250, 240},
	{"forestgreen",		 34, 139,  34},
	{"fuchsia",			255,   0, 255},
	{"gainsboro",		220, 220, 220},
	{"ghostwhite",		248, 248, 255},
	{"gold",			255, 215,   0},
	{"goldenrod",		218, 165,  32},
	{"gray",			128, 128, 128},
	{"grey",			128, 128, 128},
	{"green",			  0, 128,   0},
	{"greenyellow",		173, 255,  47},
	{"honeydew",		240, 255, 240},
	{"hotpink",			255, 105, 180},
	{"indianred",		205,  92,  92},
	{"indigo",			 75,   0, 130},
	{"ivory",			255, 255, 240},
	{"khaki",			240, 230, 140},
	{"lavender",		230, 230, 250},
	{"lavenderblush",	255, 240, 245},
	{"lawngreen",		124, 252,   0},
	{"lemonchiffon",	255, 250, 205},
	{"lightblue",		173, 216, 230},
	{"lightcoral",		240, 128, 128},
	{"lightcyan",		224, 255, 255},
	{"lightgoldenrodyellow",250, 250, 210},
	{"lightgray",		211, 211, 211},
	{"lightgreen",		144, 238, 144},
	{"lightgrey",		211, 211, 211},
	{"lightpink",		255, 182, 193},
	{"lightsalmon",		255, 160, 122},
	{"lightseagreen",	 32, 178, 170},
	{"lightskyblue",	135, 206, 250},
	{"lightslategray",	119, 136, 153},
	{"lightslategrey",	119, 136, 153},
	{"lightsteelblue",	176, 196, 222},
	{"lightyellow",		255, 255, 224},
	{"lime",			  0, 255,   0},
	{"limegreen",		 50, 205,  50},
	{"linen",			250, 240, 230},
	{"magenta",			255,   0, 255},
	{"maroon",			128,   0,   0},
	{"mediumaquamarine",102, 205, 170},
	{"mediumblue",		  0,   0, 205},
	{"mediumorchid",	186,  85, 211},
	{"mediumpurple",	147, 112, 219},
	{"mediumseagreen",	 60, 179, 113},
	{"mediumslateblue",	123, 104, 238},
	{"mediumspringgreen", 0, 250, 154},
	{"mediumturquoise",	 72, 209, 204},
	{"mediumvioletred",	199,  21, 133},
	{"midnightblue",	 25,  25, 112},
	{"mintcream",		245, 255, 250},
	{"mistyrose",		255, 228, 225},
	{"moccasin",		255, 228, 181},
	{"navajowhite",		255, 222, 173},
	{"navy",			  0,   0, 128},
	{"oldlace",			253, 245, 230},
	{"olive",			128, 128,   0},
	{"olivedrab",		107, 142,  35},
	{"orange",			255, 165,   0},
	{"orangered",		255,  69,   0},
	{"orchid",			218, 112, 214},
	{"palegoldenrod",	238, 232, 170},
	{"palegreen",		152, 251, 152},
	{"paleturquoise",	175, 238, 238},
	{"palevioletred",	219, 112, 147},
	{"papayawhip",		255, 239, 213},
	{"peachpuff",		255, 218, 185},
	{"peru",			205, 133,  63},
	{"pink",			255, 192, 203},
	{"plum",			221, 160, 221},
	{"powderblue",		176, 224, 230},
	{"purple",			128,   0, 128},
	{"red",				255,   0,   0},
	{"rosybrown",		188, 143, 143},
	{"royalblue",		 65, 105, 225},
	{"saddlebrown",		139,  69,  19},
	{"salmon",			250, 128, 114},
	{"sandybrown",		244, 164,  96},
	{"seagreen",		 46, 139,  87},
	{"seashell",		255, 245, 238},
	{"sienna",			160,  82,  45},
	{"silver",			192, 192, 192},
	{"skyblue",			135, 206, 235},
	{"slateblue",		106,  90, 205},
	{"slategray",		112, 128, 144},
	{"slategrey",		112, 128, 144},
	{"snow",			255, 250, 250},
	{"springgreen",		  0, 255, 127},
	{"steelblue",		 70, 130, 180},
	{"tan",				210, 180, 140},
	{"teal",			  0, 128, 128},
	{"thistle",			216, 191, 216},
	{"tomato",			255,  99,  71},
	{"turquoise",		 64, 224, 208},
	{"violet",			238, 130, 238},
	{"wheat",			245, 222, 179},
	{"white",			255, 255, 255},
	{"whitesmoke",		245, 245, 245},
	{"yellow",			255, 255,   0},
	{"yellowgreen",		154, 205,  50},
};
#pragma GCC diagnostic pop


#define STRCHAR_TO_INT(ch, value) if ( ch>=0x30&&ch<=0x39 ) {	value = ch-0x30;\
							}else if ( ch>=0x41&&ch<=0x46 ) {	value = ch-0x37;\
							}else if ( ch>=0x61&&ch<=0x66 ) {	value = ch-0x57;}
float DataTypes::getPixels(Length l) {
	switch (l.type) {//win->dpi = round((hRes/hSize)*25.4);
		case Length::_PX: return l.value;
		case Length::_IN: return l.value*Screen::dpi;
		case Length::_CM: return (l.value*Screen::dpi)/25.4*10;
		case Length::_MM: return (l.value*Screen::dpi)/25.4;
		case Length::_EM: return l.value;
		case Length::_EX: return l.value;
		case Length::_PT: return l.value;
		case Length::_PC: return l.value;
	}
	return 0;
}
Color DataTypes::getColor(const char* str) {
	size_t strLength = strlen(str), i=0;
	Color color;
	color.type = 0;
	// "__rgb" 5
	while ( i<strLength && str[i]<=0x20 ) i++;
	if ( (i+3)>strLength ) return color;
	if ( str[i]=='#' ) {// <editor-fold defaultstate="collapsed" desc="#FFFFFF">
		i++;
		if ( (i+3)>strLength ) return color;
		size_t newStr=0;
		while ( i<strLength ) {
			if ( !((str[i]>=0x30&&str[i]<=0x39) || (str[i]>=0x41&&str[i]<=0x46) || (str[i]>=0x61&&str[i]<=0x66)) ) break;
			newStr++;
			i++;
		}
		if ( i>=strLength ) return color;
		if ( newStr==3 ) {
			char c3=str[i-1], c2=str[i-2], c1=str[i-3];
			STRCHAR_TO_INT(c1, color.r);
			STRCHAR_TO_INT(c2, color.g);
			STRCHAR_TO_INT(c3, color.b);
			color.r = color.r*16+color.r;
			color.g = color.g*16+color.g;
			color.b = color.b*16+color.b;
			color.idColorWord = 0;
			color.type = 0xFF;
			return color;
		}else if (newStr==6) {
			char c6=str[i-1], c5=str[i-2], c4=str[i-3], c3=str[i-4], c2=str[i-5], c1=str[i-6];
			unsigned char rr, gg, bb;
			STRCHAR_TO_INT(c2, color.r);
			STRCHAR_TO_INT(c4, color.g);
			STRCHAR_TO_INT(c6, color.b);
			STRCHAR_TO_INT(c1, rr);
			STRCHAR_TO_INT(c3, gg);
			STRCHAR_TO_INT(c5, bb);
			color.r += rr*16;
			color.g += gg*16;
			color.b += bb*16;
			color.idColorWord = 0;
			color.type = 0xFE;
			return color;
		}else{
			return color;
		}// </editor-fold>
	}else if ( (str[i]=='r' || str[i]=='R') &&
				(str[i+1]=='g' || str[i+1]=='G') &&
				(str[i+2]=='b' || str[i+2]=='B')) {// <editor-fold defaultstate="collapsed" desc="rgb">
		size_t newStr;
		bool polys;
		i+=3;//1,2,3
		if ( (i+2+5)>strLength ) return color;
		i++;
		while ( i<strLength && str[i]<=0x20 ) i++;
		if ( (i+1)>=strLength ) return color;
		polys = true;
		newStr = 0;
		if (str[i]=='+') {
			i++;
		}else if (str[i]=='-') {
			polys = false;
			i++;
		}
		while ( i<strLength && (str[i]>=0x30&&str[i]<=0x39) ) {
			newStr++;
			i++;
		}
		if ( i>=strLength ) return color;
		if (!polys) {
			color.r = 0;
		}else if (newStr>0) {
			color.r = str[i-1]-0x30;
			if (newStr>1) {
				color.r += (str[i-2]-0x30)*10;
				if (newStr>2) {
					if ( (int)color.r+(str[i-3]-0x30)*100 > 0xFF ) {
						color.r = 0xFF;
					}else{
						color.r += (str[i-3]-0x30)*100;
					}
					if (newStr>3) color.r = 0xFF;
				}
			}
		}
		while ( i<strLength && (str[i]<=0x20 || str[i]==',') ) i++;
		if ( (i+1)>=strLength ) return color;
		polys = true;
		newStr = 0;
		if (str[i]=='+') {
			i++;
		}else if (str[i]=='-') {
			polys = false;
			i++;
		}
		while ( i<strLength && (str[i]>=0x30&&str[i]<=0x39) ) {
			newStr++;
			i++;
		}
		if ( i>=strLength ) return color;
		if (!polys) {
			color.g = 0;
		}else if (newStr>0) {
			color.g = str[i-1]-0x30;
			if (newStr>1) {
				color.g += (str[i-2]-0x30)*10;
				if (newStr>2) {
					if ( (int)color.g+(str[i-3]-0x30)*100 > 0xFF ) {
						color.g = 0xFF;
					}else{
						color.g += (str[i-3]-0x30)*100;
					}
					if (newStr>3) color.g = 0xFF;
				}
			}
		}
		while ( i<strLength && (str[i]<=0x20 || str[i]==',') ) i++;
		if ( (i+1)>=strLength ) return color;
		polys = true;
		newStr = 0;
		if (str[i]=='+') {
			i++;
		}else if (str[i]=='-') {
			polys = false;
			i++;
		}
		while ( i<strLength && (str[i]>=0x30&&str[i]<=0x39) ) {
			newStr++;
			i++;
		}
		if ( i>=strLength ) return color;
		if (!polys) {
			color.b = 0;
		}else if (newStr>0) {
			color.b = str[i-1]-0x30;
			if (newStr>1) {
				color.b += (str[i-2]-0x30)*10;
				if (newStr>2) {
					if ( (int)color.b+(str[i-3]-0x30)*100 > 0xFF ) {
						color.b = 0xFF;
					}else{
						color.b += (str[i-3]-0x30)*100;
					}
					if (newStr>3) color.b = 0xFF;
				}
			}
		}// </editor-fold>
	}
	
	
	return color;
}
Length DataTypes::getLength(const char* str) {
	size_t strLength = strlen(str);
	Length length;
	length.type = Length::_PX;
	length.value = 0;
	bool dot = false, plus = true;
	unsigned int leftNumber=0, rightNumber=0, rightNumberPos=1;
	char ext[2];
	size_t i=0;
	while ( i<strLength ) {
		if (str[i]<=0x20) {
			i++;
			continue;
		}else if (str[i]=='+') {
			plus = true;
			i++;
			continue;
		}else if (str[i]=='-') {
			plus = false;
			i++;
			continue;
		}else if ( str[i]>=0x30 && str[i]<=0x39 ) {
			leftNumber = leftNumber*10 + (str[i]-0x30);
			i++;
			continue;
		}else if (str[i]=='.') {
			dot = true;
			i++;
			break;
		}else if ( (str[i]>=0x41 && str[i]<=0x5A)||(str[i]>=0x61 && str[i]<=0x7A) ) {
			break;
		}else{
			printf("1");
			return length;
		}
	}
	if (i>=strLength) {
		printf("1.5");
		length.value = (float)leftNumber;
		return length;
	}
	while ( i<strLength ) {
		if (str[i]<=0x20) {
			i++;
			continue;
		}else if ( str[i]>=0x30 && str[i]<=0x39 ) {
			rightNumber = rightNumber*10+ (str[i]-0x30);
			//length.value += ((float)(str[i]-0x30))/rightNumberPos;
			rightNumberPos*=10;
			i++;
			continue;
		}else if ( (str[i]>=0x41 && str[i]<=0x5A)||(str[i]>=0x61 && str[i]<=0x7A) ) {
			break;
		}else{
			printf("2");
			return length;
		}
	}
	length.value = (float)rightNumber/rightNumberPos + leftNumber;
	if ((i+1)>=strLength) {
		printf("3");
		return length;
	}
	while ( (i+1)<strLength ) {
		if (str[i]<=0x20) {
			i++;
			continue;
		}else if ( str[i]>=0x41 && str[i]<=0x5A  ) {
			ext[0] = str[i]+0x20;
			if ( str[i+1]>=0x41 && str[i+1]<=0x5A  ) {
				ext[1] = str[i+1]+0x20;
			}else if ( str[i+1]>=0x61 && str[i+1]<=0x7A  ) {
				ext[1] = str[i+1];
			}else{
				printf("4");
				return length;
			}
			break;
		}else if ( str[i]>=0x61 && str[i]<=0x7A  ) {
			ext[0] = str[i];
			if ( str[i+1]>=0x41 && str[i+1]<=0x5A  ) {
				ext[1] = str[i+1]+0x20;
			}else if ( str[i+1]>=0x61 && str[i+1]<=0x7A  ) {
				ext[1] = str[i+1];
			}else{
				printf("5");
				return length;
			}
			break;
		}else{
			printf("6");
			return length;
		}
	}
	if ( ext[0]=='p'&&ext[1]=='x' ) {
		length.type = Length::_PX;
	}else if (ext[0]=='i'&&ext[1]=='n') {
		length.type = Length::_IN;
	}else if (ext[0]=='c'&&ext[1]=='m') {
		length.type = Length::_CM;
	}else if (ext[0]=='m'&&ext[1]=='m') {
		length.type = Length::_MM;
	}else if (ext[0]=='e'&&ext[1]=='m') {
		length.type = Length::_EM;
	}else if (ext[0]=='e'&&ext[1]=='x') {
		length.type = Length::_EX;
	}else if (ext[0]=='p'&&ext[1]=='t') {
		length.type = Length::_PT;
	}else if (ext[0]=='p'&&ext[1]=='c') {
		length.type = Length::_PC;
	}
	return length;
}
Coordinate DataTypes::getCoordinate(const char *str) {
	
}
Angle DataTypes::getAngle(const char *str) {
	
}
TransformMatrix	DataTypes::getTransformMatrix(const char *str) {
	TransformMatrix matrix;
	return matrix;
}


Symbol::Symbol() :matrix() {
}
int Symbol::renderGLComptAll() {
	fputs("SVG::Symbol::renderGLComptAll\n", iovir);
	return false;
}
int Symbol::renderGL400() {
	fputs("SVG::Symbol::renderGL400\n", iovir);
	return false;
}
int Symbol::renderGL330() {
	fputs("SVG::Symbol::renderGL330\n", iovir);
	return false;
}
int Symbol::renderGL210() {
	fputs("SVG::Symbol::renderGL210\n", iovir);
	return false;
}

Group::Group() {
	
}
int Group::renderGLComptAll() {
	float mat[16] = {
		this->matrix.a,	this->matrix.d,	0,	0,
		this->matrix.b,	this->matrix.e,	0,	0,
		this->matrix.c,	this->matrix.f,	1,	0,
		0,	0,	0,	1};
	glPushMatrix();
	glMultMatrixf(mat);
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->renderGLComptAll();
	}
	glPopMatrix();
	return true;
}
int Group::renderGL400() {
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->renderGL400();
	}
	return true;
}
int Group::renderGL330() {
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->renderGL330();
	}
	return true;
}
int Group::renderGL210() {
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->renderGL210();
	}
	return true;
}

BasicShapeRect::BasicShapeRect() {
	/*Coordinate cx, cy;
	cx.value = 50;
	cy.value = 90;
	this->matrix.translate(cx, cy);
	this->matrix.trace();
	
	Angle ag;
	ag.value = -45;
	this->matrix.rotate(ag);
	this->matrix.trace();
	
	cx.value = 130;
	cy.value = 160;
	this->matrix.translate(cx, cy);
	this->matrix.trace();
	
	Length lx, ly;
	lx.type = Length::_PX;
	ly.type = Length::_PX;
	lx.value = 2;
	ly.value = 0.5;
	this->matrix.scale(lx, ly);
	this->matrix.trace();
	this->matrix.a = .707;
	this->matrix.b = .707;
	this->matrix.c = 255.03;
	this->matrix.d = -.707;
	this->matrix.e = .707;
	this->matrix.f = 111.21;*/
}
int BasicShapeRect::renderGLComptAll() {
	float mat[16] = {
		this->matrix.a,	this->matrix.d,	0,	0,
		this->matrix.b,	this->matrix.e,	0,	0,
		this->matrix.c,	this->matrix.f,	1,	0,
		this->matrix.c,	this->matrix.f,	1,	1};
	glPushMatrix();
	glMultMatrixf(mat);
	glBegin(GL_QUADS);
		glColor3ub(0xFF, 0, 0);
		glVertex2s(this->x.value,					this->y.value);
		glVertex2s(this->x.value+this->width.value,	this->y.value);
		glVertex2s(this->x.value+this->width.value,	this->y.value+this->height.value);
		glVertex2s(this->x.value,					this->y.value+this->height.value);
	glEnd();
	glPopMatrix();
	return true;
}
int BasicShapeRect::renderGL400() {
	return false;
}
int BasicShapeRect::renderGL330() {
	ViewMatrix mat = ViewMatrixIdentity();
	mat.a[0] = this->matrix.a;
	mat.a[1] = this->matrix.b;
	mat.a[2] = this->matrix.c;
	mat.a[3] = this->matrix.c;
	
	mat.a[4] = this->matrix.d;
	mat.a[5] = this->matrix.e;
	mat.a[6] = this->matrix.f;
	mat.a[7] = this->matrix.f;
	
	mat.a[11] = 1;
	OpenGL::pushViewMatrix();
	OpenGL::multViewMatrix(mat);
	if (GLShader::shader->crc32!=ShaderSVGmain::CRC32) GLShader::setShader(ShaderSVGmain::prog);
	if (this->vao==0) {
		float vertex[8] = {
			DataTypes::getPixels(this->x),										DataTypes::getPixels(this->y),
			DataTypes::getPixels(this->x)+DataTypes::getPixels(this->width),	DataTypes::getPixels(this->y),
			DataTypes::getPixels(this->x)+DataTypes::getPixels(this->width),	DataTypes::getPixels(this->y)+DataTypes::getPixels(this->height),
			DataTypes::getPixels(this->x),										DataTypes::getPixels(this->y)+DataTypes::getPixels(this->height),
		};
		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);
		
		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float), vertex, GL_STATIC_DRAW);
		glVertexAttribPointer(ShaderBitmap::prog->position, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(ShaderBitmap::prog->position);
	}
	glBindVertexArray(this->vao);
	glUniform1i(ShaderSVGmain::prog->typeShape, BasicShapeRect::CRC32);
	//glUniform4f(ShaderSVGmain::prog->fillColor, (float)this->color.r/0xFF, (float)this->color.g/0xFF, (float)this->color.b/0xFF, 1.0);
	glDrawArrays(GL_QUADS, 0, 4);
	OpenGL::popViewMatrix();
	return true;
}
int BasicShapeRect::renderGL210() {
	return false;
}

BasicShapeCircle::BasicShapeCircle() {
	/*Coordinate cx, cy;
	cx.value = 50;
	cy.value = 90;
	this->matrix.translate(cx, cy);
	this->matrix.trace();
	
	Angle ag;
	ag.value = -45;
	this->matrix.rotate(ag);
	this->matrix.trace();
	
	cx.value = 130;
	cy.value = 160;
	this->matrix.translate(cx, cy);
	this->matrix.trace();
	
	Length lx, ly;
	lx.type = Length::_PX;
	ly.type = Length::_PX;
	lx.value = 2;
	ly.value = 0.5;
	this->matrix.scale(lx, ly);
	this->matrix.trace();
	this->matrix.a = .707;
	this->matrix.b = .707;
	this->matrix.c = 255.03;
	this->matrix.d = -.707;
	this->matrix.e = .707;
	this->matrix.f = 111.21;*/
}
int BasicShapeCircle::renderGLComptAll() {
	float mat[16] = {
		this->matrix.a,	this->matrix.d,	0,	0,
		this->matrix.b,	this->matrix.e,	0,	0,
		this->matrix.c,	this->matrix.f,	1,	0,
		this->matrix.c,	this->matrix.f,	1,	1};
	glPushMatrix();
	glMultMatrixf(mat);
	
	glBegin(GL_TRIANGLE_FAN);
		glColor4ub(0xFF, 0, 0, 0x88);
		glVertex2f(this->cx.value, this->cy.value);
		float a, radius=this->r.value, max=M_PI*2*radius;
		for(int i=0; i<max; i+=5) {
			a = i/radius;
			glVertex2f(cos(a)*radius+this->cx.value, sin(a)*radius+this->cy.value);
		}
		glVertex2f(radius+this->cx.value, this->cy.value);
	glEnd();
	
	glPopMatrix();
	return true;
}
int BasicShapeCircle::renderGL400() {
	return false;
}
int BasicShapeCircle::renderGL330() {
	ViewMatrix mat;
	mat.a[0] = this->matrix.a;
	mat.a[1] = this->matrix.b;
	mat.a[2] = this->matrix.c;
	mat.a[3] = this->matrix.c;
	
	mat.a[4] = this->matrix.d;
	mat.a[5] = this->matrix.e;
	mat.a[6] = this->matrix.f;
	mat.a[7] = this->matrix.f;
	
	mat.a[11] = 1;
	OpenGL::pushViewMatrix();
	OpenGL::multViewMatrix(mat);
	if (GLShader::shader->crc32!=ShaderSVGmain::CRC32) GLShader::setShader(ShaderSVGmain::prog);
	if (this->vao==0) {
		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);
		
		glBindBuffer(GL_ARRAY_BUFFER, OpenGL::circleBuffer);
		glVertexAttribPointer(ShaderBitmap::prog->position, 2, GL_FLOAT, GL_TRUE, 32*2*sizeof(float), 0);//256*sizeof(float)
		glEnableVertexAttribArray(ShaderBitmap::prog->position);
	}
	glBindVertexArray(this->vao);
	glUniform1i(ShaderSVGmain::prog->typeShape, BasicShapeCircle::CRC32);
	glUniform4f(ShaderSVGmain::prog->fillColor, 1, 1, 0, 1);
	glUniform4f(ShaderSVGmain::prog->circleTransform, DataTypes::getPixels(this->cx), DataTypes::getPixels(this->cy), DataTypes::getPixels(this->r), DataTypes::getPixels(this->r));
	glDrawArrays(GL_TRIANGLE_FAN, 0, 130);
	OpenGL::popViewMatrix();
	return true;
}
int BasicShapeCircle::renderGL210() {
	return false;
}

BasicShapeEllipse::BasicShapeEllipse() {
	/*Coordinate cx, cy;
	cx.value = 50;
	cy.value = 90;
	this->matrix.translate(cx, cy);
	this->matrix.trace();
	
	Angle ag;
	ag.value = -45;
	this->matrix.rotate(ag);
	this->matrix.trace();
	
	cx.value = 130;
	cy.value = 160;
	this->matrix.translate(cx, cy);
	this->matrix.trace();
	
	Length lx, ly;
	lx.type = Length::_PX;
	ly.type = Length::_PX;
	lx.value = 2;
	ly.value = 0.5;
	this->matrix.scale(lx, ly);
	this->matrix.trace();
	this->matrix.a = .707;
	this->matrix.b = .707;
	this->matrix.c = 255.03;
	this->matrix.d = -.707;
	this->matrix.e = .707;
	this->matrix.f = 111.21;*/
}
int BasicShapeEllipse::renderGLComptAll() {
	float mat[16] = {
		this->matrix.a,	this->matrix.d,	0,	0,
		this->matrix.b,	this->matrix.e,	0,	0,
		this->matrix.c,	this->matrix.f,	1,	0,
		this->matrix.c,	this->matrix.f,	1,	1};
	glPushMatrix();
	glMultMatrixf(mat);
	
	//GL_TRIANGLE_FAN
	glBegin(GL_TRIANGLE_FAN);
		glColor3ub(0xFF, 0, 0);
		glVertex2f(this->cx.value, this->cy.value);
		float a, irx=this->rx.value, iry=this->ry.value;//, max=M_PI*2*radius;
		for(int i=0; i<360; i+=5) {
			a = M_PI/180*i;
			glVertex2f(cos(a)*irx+this->cx.value, sin(a)*iry+this->cy.value);
		}
		glVertex2f(irx+this->cx.value, this->cy.value);
	glEnd();
	
	glPopMatrix();
	return true;
}
int BasicShapeEllipse::renderGL400() {
	return false;
}
int BasicShapeEllipse::renderGL330() {
	return false;
}
int BasicShapeEllipse::renderGL210() {
	return false;
}

BasicShapeLine::BasicShapeLine() {
	/*Coordinate cx, cy;
	cx.value = 50;
	cy.value = 90;
	this->matrix.translate(cx, cy);
	this->matrix.trace();
	
	Angle ag;
	ag.value = -45;
	this->matrix.rotate(ag);
	this->matrix.trace();
	
	cx.value = 130;
	cy.value = 160;
	this->matrix.translate(cx, cy);
	this->matrix.trace();
	
	Length lx, ly;
	lx.type = Length::_PX;
	ly.type = Length::_PX;
	lx.value = 2;
	ly.value = 0.5;
	this->matrix.scale(lx, ly);
	this->matrix.trace();
	this->matrix.a = .707;
	this->matrix.b = .707;
	this->matrix.c = 255.03;
	this->matrix.d = -.707;
	this->matrix.e = .707;
	this->matrix.f = 111.21;*/
}
int BasicShapeLine::renderGLComptAll() {
	float mat[16] = {
		this->matrix.a,	this->matrix.d,	0,	0,
		this->matrix.b,	this->matrix.e,	0,	0,
		this->matrix.c,	this->matrix.f,	1,	0,
		this->matrix.c,	this->matrix.f,	1,	1};
	glPushMatrix();
	glMultMatrixf(mat);
	
	
	glBegin(GL_LINES);
		glColor3ub(0xFF, 0, 0);
		glVertex2s(this->x1.value, this->y1.value);
		glVertex2s(this->x2.value, this->y2.value);
	glEnd();
	
	glPopMatrix();
	return true;
}
int BasicShapeLine::renderGL400() {
	return false;
}
int BasicShapeLine::renderGL330() {
	ViewMatrix mat;
	mat.a[0] = this->matrix.a;
	mat.a[1] = this->matrix.b;
	mat.a[2] = this->matrix.c;
	mat.a[3] = this->matrix.c;
	
	mat.a[4] = this->matrix.d;
	mat.a[5] = this->matrix.e;
	mat.a[6] = this->matrix.f;
	mat.a[7] = this->matrix.f;
	
	mat.a[11] = 1;
	OpenGL::pushViewMatrix();
	OpenGL::multViewMatrix(mat);
	if (GLShader::shader->crc32!=ShaderSVGmain::CRC32) GLShader::setShader(ShaderSVGmain::prog);
	if (this->vao==0) {
		float vertex[4] = {
			DataTypes::getPixels(this->x1),	DataTypes::getPixels(this->y1),
			DataTypes::getPixels(this->x2),	DataTypes::getPixels(this->y2),
		};
		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);
		
		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, 4*sizeof(float), vertex, GL_STATIC_DRAW);
		glVertexAttribPointer(ShaderBitmap::prog->position, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(ShaderBitmap::prog->position);
	}
	glBindVertexArray(this->vao);
	glUniform1i(ShaderSVGmain::prog->typeShape, BasicShapeLine::CRC32);
	glDrawArrays(GL_LINES, 0, 2);
	OpenGL::popViewMatrix();
	return true;
}
int BasicShapeLine::renderGL210() {
	return false;
}

BasicShapePolyline::BasicShapePolyline() :length(0), points(nullptr) {
}
int BasicShapePolyline::renderGLComptAll() {
	if (length == 0) return true;
	float mat[16] = {
		this->matrix.a,	this->matrix.d,	0,	0,
		this->matrix.b,	this->matrix.e,	0,	0,
		this->matrix.c,	this->matrix.f,	1,	0,
		this->matrix.c,	this->matrix.f,	1,	1};
	glPushMatrix();
	glMultMatrixf(mat);
	
	
	glBegin(GL_LINE_STRIP);
		glColor3ub(0xFF, 0, 0);
		glVertex2f(this->points[0].x.value, this->points[0].y.value);
		for(uint i=1; i<this->length; i++) {
			glVertex2f(this->points[i].x.value, this->points[i].y.value);
		}
	glEnd();
	
	glPopMatrix();
	return true;
}
int BasicShapePolyline::renderGL400() {
	return false;
}
int BasicShapePolyline::renderGL330() {
	return false;
}
int BasicShapePolyline::renderGL210() {
	return false;
}

BasicShapePolygon::BasicShapePolygon() :length(0), points(nullptr) {
}
int BasicShapePolygon::renderGLComptAll() {
	if (length < 2) return true;
	float mat[16] = {
		this->matrix.a,	this->matrix.d,	0,	0,
		this->matrix.b,	this->matrix.e,	0,	0,
		this->matrix.c,	this->matrix.f,	1,	0,
		this->matrix.c,	this->matrix.f,	1,	1};
	glPushMatrix();
	glMultMatrixf(mat);
	
	
	glBegin(GL_POLYGON);
		glColor3ub(0xFF, 0, 0);
		glVertex2f(this->points[0].x.value, this->points[0].y.value);
		for(int i=1; i<this->length; i++) {
			glVertex2f(this->points[i].x.value, this->points[i].y.value);
		}
		glVertex2f(this->points[0].x.value, this->points[0].y.value);
	glEnd();
	
	glPopMatrix();
	return true;
}
int BasicShapePolygon::renderGL400() {
	return false;
}
int BasicShapePolygon::renderGL330() {
	return false;
}
int BasicShapePolygon::renderGL210() {
	return false;
}