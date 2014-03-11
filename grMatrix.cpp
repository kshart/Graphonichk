#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "grBaseTypes.h"

#define M_PI 3.14159265358979323846

using namespace std;
using namespace Graphonichk;


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
void TransformMatrix::scale(float s) {
	//s 0 0
	//0 s 0
	//0 0 1
	this->a *= s;
	this->b *= s;
	this->d *= s;
	this->e *= s;
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
void TransformMatrix::scale(float sx, float sy) {
	//sx 0 0
	//0 sy 0
	//0 0 1
	this->a *= sx;
	this->b *= sy;
	this->d *= sx;
	this->e *= sy;
}
void TransformMatrix::translate(float cx, float cy) {
	//1 0 x
	//0 1 y
	//0 0 1
	this->c = this->a*cx+this->b*cy+this->c;
	this->f = this->d*cx+this->e*cy+this->f;
}
void TransformMatrix::rotate(float angle) {
	//cos	sin 0
	//-sin	cos 0
	//0		0 1
	float ag = angle*M_PI/180, a = this->a, b = this->b, d = this->d, e = this->e;
	this->a = a*cos(ag)+b*sin(ag);
	this->b = a*-sin(ag)+b*cos(ag);
	this->d = d*cos(ag)+e*sin(ag);
	this->e = d*-sin(ag)+e*cos(ag);
}
void TransformMatrix::skewX(float angle) {
	//1	0 0
	//tg 1 0
	//0	0 1
	float ag = angle*M_PI/180;
}
void TransformMatrix::skewY(float angle) {
	//1	tg 0
	//0	1 0
	//0	0 1
	float ag = angle*M_PI/180;
}

void ViewMatrix::trace() {
	printf("matrix=\n%f\t%f\t%f\t%f\t\n%f\t%f\t%f\t%f\t\n%f\t%f\t%f\t%f\t\n%f\t%f\t%f\t%f\t\n", this->a[0], this->a[1], this->a[2], this->a[3], this->a[4], this->a[5], 
			this->a[6], this->a[7], this->a[8], this->a[9], this->a[10], this->a[11], this->a[12], this->a[13], this->a[14], this->a[15]);
}
ViewMatrix::ViewMatrix() {
	this->a[0] = 1;
	this->a[1] = 0;
	this->a[2] = 0;
	this->a[3] = 0;
	
	this->a[4] = 0;
	this->a[5] = 1;
	this->a[6] = 0;
	this->a[7] = 0;
	
	this->a[8] = 0;
	this->a[9] = 0;
	this->a[10] = 1;
	this->a[11] = 0;
	
	this->a[12] = 0;
	this->a[13] = 0;
	this->a[14] = 0;
	this->a[15] = 1;
}
ViewMatrix::ViewMatrix(float l, float r, float t, float b, float f, float n) {
	this->a[0] = 2/(r-l);
	this->a[4] = 0;
	this->a[8] = 0;
	this->a[12] = 0;
	
	this->a[1] = 0;
	this->a[5] = 2/(t-b);
	this->a[9] = 0;
	this->a[13] = 0;
	
	this->a[2] = 0;
	this->a[6] = 0;
	this->a[10] = -2/(f-n);
	this->a[14] = 0;
	
	this->a[3] = -(r+l)/(r-l);
	this->a[7] = -(t+b)/(t-b);
	this->a[11] = -(f+n)/(f-n);
	this->a[15] = 1;
}