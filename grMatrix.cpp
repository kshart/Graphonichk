#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "grMatrix.h"

#define M_PI 3.14159265358979323846

using namespace std;
using namespace Graphonichk;


/*void ViewMatrix::trace() {
	printf("matrix=\n%f\t%f\t%f\t%f\t\n%f\t%f\t%f\t%f\t\n%f\t%f\t%f\t%f\t\n%f\t%f\t%f\t%f\t\n", this->a[0], this->a[1], this->a[2], this->a[3], this->a[4], this->a[5], 
			this->a[6], this->a[7], this->a[8], this->a[9], this->a[10], this->a[11], this->a[12], this->a[13], this->a[14], this->a[15]);
}*/


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

Matrix3D::Matrix3D() {
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
Matrix3D::Matrix3D(const Matrix3D *m) {
	memcpy(this->a, m->a, 16*sizeof(float));
}
Matrix3D Matrix3D::ViewOrtho(float l, float r, float t, float b, float f, float n) {
	Matrix3D vm;
	vm.a[0] = 2/(r-l);
	vm.a[4] = 0;
	vm.a[8] = 0;
	vm.a[12] = 0;
	
	vm.a[1] = 0;
	vm.a[5] = 2/(t-b);
	vm.a[9] = 0;
	vm.a[13] = 0;
	
	vm.a[2] = 0;
	vm.a[6] = 0;
	vm.a[10] = -2/(f-n);
	vm.a[14] = 0;
	
	vm.a[3] = -(r+l)/(r-l);
	vm.a[7] = -(t+b)/(t-b);
	vm.a[11] = -(f+n)/(f-n);
	vm.a[15] = 1;
	return vm;
}
Matrix3D Matrix3D::ViewPerspective(float l, float r, float t, float b, float f, float n) {
	Matrix3D vm;
	vm.a[0] = 2*n/(r-l);
	vm.a[4] = 0;
	vm.a[8] = (r+l)/(r-l);
	vm.a[12] = 0;
	
	vm.a[1] = 0;
	vm.a[5] = 2*n/(t-b);
	vm.a[9] = (t+b)/(t-b);
	vm.a[13] = 0;
	
	vm.a[2] = 0;
	vm.a[6] = 0;
	vm.a[10] = -(f+n)/(f-n);
	vm.a[14] = -2*f*n/(f-n);
	
	vm.a[3] = 0;
	vm.a[7] = 0;
	vm.a[11] = -1;
	vm.a[15] = 0;
	return vm;
}
Matrix3D Matrix3D::ViewPerspective2(float fov, float aspect, float n, float f) {
	Matrix3D vm;
	float mf = 1/tanf(fov/2);
	vm.a[0] = mf / aspect;
	vm.a[1] = 0;
	vm.a[2] = 0;
	vm.a[3] = 0;
	
	vm.a[4] = 0;
	vm.a[5] = mf;
	vm.a[6] = 0;
	vm.a[7] = 0;
	
	vm.a[8] = 0;
	vm.a[9] = 0;
	vm.a[10] = (f+n)/(n-f);
	vm.a[11] = (2*f*n)/(n-f);
	
	vm.a[12] = 0;
	vm.a[13] = 0;
	vm.a[14] = -1;
	vm.a[15] = 0;
	return vm;
}
void Matrix3D::trace() {
	printf("matrix=\n%f\t%f\t%f\t%f\t\n%f\t%f\t%f\t%f\t\n%f\t%f\t%f\t%f\t\n%f\t%f\t%f\t%f\t\n", this->a[0], this->a[1], this->a[2], this->a[3], this->a[4], this->a[5], 
			this->a[6], this->a[7], this->a[8], this->a[9], this->a[10], this->a[11], this->a[12], this->a[13], this->a[14], this->a[15]);
}
void Matrix3D::loadIdentity() {
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
void Matrix3D::multiple(Matrix3D &m2) {
	Matrix3D m1(this);
	//i = 4
	//j = 4
	//  0  1  2  3
	//  4  5  6  7
	//  8  9 10 11
	// 12 13 14 15
	this->a[0] = m1.a[0]*m2.a[0] + m1.a[1]*m2.a[4] + m1.a[2]*m2.a[8] + m1.a[3]*m2.a[12];
	this->a[1] = m1.a[0]*m2.a[1] + m1.a[1]*m2.a[5] + m1.a[2]*m2.a[9] + m1.a[3]*m2.a[13];
	this->a[2] = m1.a[0]*m2.a[2] + m1.a[1]*m2.a[6] + m1.a[2]*m2.a[10] + m1.a[3]*m2.a[14];
	this->a[3] = m1.a[0]*m2.a[3] + m1.a[1]*m2.a[7] + m1.a[2]*m2.a[11] + m1.a[3]*m2.a[15];
	
	this->a[4] = m1.a[4]*m2.a[0] + m1.a[5]*m2.a[4] + m1.a[6]*m2.a[8] + m1.a[7]*m2.a[12];
	this->a[5] = m1.a[4]*m2.a[1] + m1.a[5]*m2.a[5] + m1.a[6]*m2.a[9] + m1.a[7]*m2.a[13];
	this->a[6] = m1.a[4]*m2.a[2] + m1.a[5]*m2.a[6] + m1.a[6]*m2.a[10] + m1.a[7]*m2.a[14];
	this->a[7] = m1.a[4]*m2.a[3] + m1.a[5]*m2.a[7] + m1.a[6]*m2.a[11] + m1.a[7]*m2.a[15];
	
	this->a[8] = m1.a[8]*m2.a[0] + m1.a[9]*m2.a[4] + m1.a[10]*m2.a[8] + m1.a[11]*m2.a[12];
	this->a[9] = m1.a[8]*m2.a[1] + m1.a[9]*m2.a[5] + m1.a[10]*m2.a[9] + m1.a[11]*m2.a[13];
	this->a[10] = m1.a[8]*m2.a[2] + m1.a[9]*m2.a[6] + m1.a[10]*m2.a[10] + m1.a[11]*m2.a[14];
	this->a[11] = m1.a[8]*m2.a[3] + m1.a[9]*m2.a[7] + m1.a[10]*m2.a[11] + m1.a[11]*m2.a[15];
	
	this->a[12] = m1.a[12]*m2.a[0] + m1.a[13]*m2.a[4] + m1.a[14]*m2.a[8] + m1.a[15]*m2.a[12];
	this->a[13] = m1.a[12]*m2.a[1] + m1.a[13]*m2.a[5] + m1.a[14]*m2.a[9] + m1.a[15]*m2.a[13];
	this->a[14] = m1.a[12]*m2.a[2] + m1.a[13]*m2.a[6] + m1.a[14]*m2.a[10] + m1.a[15]*m2.a[14];
	this->a[15] = m1.a[12]*m2.a[3] + m1.a[13]*m2.a[7] + m1.a[14]*m2.a[11] + m1.a[15]*m2.a[15];
}
void Matrix3D::scale(float s) {
	// s 0 0 0
	// 0 s 0 0
	// 0 0 s 0
	// 0 0 0 1
	Matrix3D mt;
	mt.a[0] = s;
	mt.a[5] = s;
	mt.a[10] = s;
	this->multiple(mt);
}
void Matrix3D::translate(float x, float y, float z) {
	// 1 0 0 tx
	// 0 1 0 ty
	// 0 0 1 tz
	// 0 0 0 1
	Matrix3D mt;
	mt.a[3] = x;
	mt.a[7] = y;
	mt.a[11] = z;
	this->multiple(mt);
}
void Matrix3D::rotate(float angleX, float angleY, float angleZ) {
	/*
	
	1 0       0        0
	0 cos(ax) -sin(ax) 0
	0 sin(ax) cos(ax)  0
	0 0       0        1
	
	cos(ay)  0 sin(ay) 0
	0        1 0       0
	-sin(ay) 0 cos(ay) 0
	0        0 0       1
	
	1*2
	
	cay	     0   say      0
	sax*say  cax -sax*cay 0
	-cax*say sax cax*cay  0
	0        0   0        1
	
	cos(az) -sin(az) 0 0
	sin(az) cos(az)  0 0
	0       0        1 0
	0       0        0 1
	
	cay*caz              -cay*saz             say      0
	sax*say*caz+cax*saz  -sax*say*saz+cax*caz -sax*cay 0
	-cax*say*caz+sax*saz cax*say*saz+sax*caz  cax*cay  0
	0                    0                    0        1
	
	
	*/
	float cax = cos(angleX), sax = sin(angleX), 
			cay = cos(angleY), say = sin(angleY),
			caz = cos(angleZ), saz = sin(angleZ);
	Matrix3D mt;
	
	mt.a[0] = cay*caz;
	mt.a[1] = -cay*saz;
	mt.a[2] = say;
	
	mt.a[4] = sax*say*caz + cax*saz;
	mt.a[5] = -sax*say*saz + cax*caz;
	mt.a[6] = -sax*cay;
	
	mt.a[8]  = -cax*say*caz + sax*saz;
	mt.a[9]  = cax*say*saz + sax*caz;
	mt.a[10] = cax*cay;
	
	this->multiple(mt);
}
