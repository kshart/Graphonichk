#include <math.h>
#include "SVG.h"
#define M_PI 3.14159265358979323846
using namespace SVG;
using namespace std;

float DataTypes::getPixels(Length l) {
	switch (l.type) {//win->dpi = round((hRes/hSize)*25.4);
		case Length::_PX: return l.value;
		case Length::_IN: return l.value*Windows::window->dpi;
		case Length::_CM: return (l.value*Windows::window->dpi)/25.4*10;
		case Length::_MM: return (l.value*Windows::window->dpi)/25.4;
		case Length::_EM: return l.value;
		case Length::_EX: return l.value;
		case Length::_PT: return l.value;
		case Length::_PC: return l.value;
	}
	return 0;
}

Length DataTypes::getLength(const char* str) {
	Length length;
	length.type = Length::_PX;
	length.value = 0;
	size_t strLength = strlen(str);
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
	return false;
}
int Symbol::renderGL400() {
	return false;
}
int Symbol::renderGL330() {
	return false;
}
int Symbol::renderGL210() {
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
		float vertex[8] = {
			this->x.value,						this->y.value,
			this->x.value+this->width.value,	this->y.value,
			this->x.value+this->width.value,	this->y.value+this->height.value,
			this->x.value,						this->y.value+this->height.value,
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
	glDrawArrays(GL_LINE_LOOP, 0, 4);
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
	return false;
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
	return false;
}
int BasicShapeLine::renderGL210() {
	return false;
}

BasicShapePolyline::BasicShapePolyline() :length(0), points(0) {
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
		for(int i=1; i<this->length; i++) {
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

BasicShapePolygon::BasicShapePolygon() :length(0), points(0) {
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