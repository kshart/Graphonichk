#include <GL/gl.h>
#include <math.h>
#include "SVG.h"
#define M_PI 3.14159265358979323846
using namespace SVG;
using namespace std;

TransformMatrix::TransformMatrix(float ina, float inb, float inc, float ind, float ine, float inf) :a(ina), b(inb), c(inc), d(ind), e(ine), f(inf) {
}
TransformMatrix::TransformMatrix() {
	this->loadIdentity();
}
void TransformMatrix::trace() {
	printf("matrix=\n%f\t%f\t%f\t\n%f\t%f\t%f\t\n", this->a, this->b, this->c, this->d, this->e, this->f);
}
void TransformMatrix::loadIdentity() {
	this->a = 1;
	this->b = 0;
	this->c = 0;
	this->d = 0;
	this->e = 1;
	this->f = 0;
}
void TransformMatrix::scale(Length s) {
	float length;
	switch (s.type) {
		case Length::_PX:
			length = s.value;
			break;
		case Length::_IN:
			length = s.value;
			break;
	}
	//s 0 0
	//0 s 0
	//0 0 1
	this->a *= length;
	this->b *= length;
	this->d *= length;
	this->e *= length;
	/*
	 * a11 a12 a13
	 * a21 a22 a23
	 * 
	 * b11 b12 b13
	 * b21 b22 b23
	 * 
	 * 
	this->a = this->a*b->a+this->b*b->d;
	this->b = this->a*b->b+this->b*b->e;
	this->c = this->a*b->c+this->b*b->f+this->a;
	
	this->d = this->d*b->a+this->e*b->d;
	this->e = this->d*b->b+this->e*b->e;
	this->f = this->d*b->c+this->e*b->f+this->d;
	 * 
	 * this->a = this->a*b->a;
	this->b = this->b*b->e;
	this->d = this->d*b->a;
	this->e = this->e*b->e;
	 * c11 = a11*b11+a12*b21+a13*b31
	 * c12 = a11*b12+a12*b22+a13*b32
	 * c13 = a11*b13+a12*b23+a13*b33
	 * 
	 * c21 = a21*b11+a22*b21+a23*b31
	 * c22 = a21*b12+a22*b22+a23*b32
	 * c23 = a21*b13+a22*b23+a23*b33
	 */
}
void TransformMatrix::scale(Length sx, Length sy) {
	float lengthx = sx.value, lengthy = sy.value;
	//sx 0 0
	//0 sy 0
	//0 0 1
	this->a *= lengthx;
	this->b *= lengthy;
	this->d *= lengthx;
	this->e *= lengthy;
}
void TransformMatrix::translate(Coordinate x, Coordinate y) {
	float cx = x.value, cy = y.value;
	//1 0 x
	//0 1 y
	//0 0 1
	this->c = this->a*cx+this->b*cy+this->c;
	this->f = this->d*cx+this->e*cy+this->f;
}
void TransformMatrix::rotate(Angle angle) {
	//cos	sin 0
	//-sin	cos 0
	//0		0 1
	float ag = angle.value*M_PI/180, a = this->a, b = this->b, d = this->d, e = this->e;
	this->a = a*cos(ag)+b*sin(ag);
	this->b = a*-sin(ag)+b*cos(ag);
	this->d = d*cos(ag)+e*sin(ag);
	this->e = d*-sin(ag)+e*cos(ag);
}
void TransformMatrix::skewX(Angle angle) {
	//1	0 0
	//tg 1 0
	//0	0 1
	float ag = angle.value*M_PI/180;
}
void TransformMatrix::skewY(Angle angle) {
	//1	tg 0
	//0	1 0
	//0	0 1
	float ag = angle.value*M_PI/180;
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
	return true;
	glPopMatrix();
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
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
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
	return false;
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
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	
	glBegin(GL_TRIANGLE_FAN);
		glColor3ub(0xFF, 0, 0);
		float a, radius=this->r.value;
		for(int i=0; i<360; i+=20) {
			a = i*M_PI/180;
			glVertex2s(cos(a)*radius+this->cx.value, sin(a)*radius+this->cy.value);
		}
		
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