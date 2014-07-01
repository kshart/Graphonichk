#ifndef GRMATRIX_H
#define	GRMATRIX_H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


namespace Graphonichk {
	class TransformMatrix {
	public:
		float a, b, c, d, e, f;
		TransformMatrix();
		TransformMatrix(float a, float b, float c, float d, float e, float f);
		void trace() const noexcept;
		void loadIdentity() noexcept;
		void scale(float s) noexcept;
		void scale(float sx, float sy) noexcept;
		void translate(float x, float y) noexcept;
		void rotate(float angle) noexcept;
		void skewX(float angle) noexcept;
		void skewY(float angle) noexcept;
	};
	class Matrix3D {
	public:
		float a[16];
		Matrix3D();
		Matrix3D(float x, float y, float z, float xScale, float yScale, float zScale, float xRotate, float yRotate, float zRotate);
		Matrix3D(const Matrix3D *m);
		static Matrix3D ViewOrtho(float r, float l, float t, float b, float f, float n);
		static Matrix3D ViewPerspective(float r, float l, float t, float b, float f, float n);
		static Matrix3D ViewPerspective2(float fov, float aspect, float f, float n);
		void loadIdentity() noexcept;
		void inverse() noexcept;
		void transpose() noexcept;
		void trace() const noexcept;
		
		void operator*=(const Matrix3D& m2) noexcept {
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
		const friend Matrix3D operator*(const Matrix3D &m1, const Matrix3D &m2) noexcept {
			Matrix3D mat;
			mat.a[0] = m1.a[0]*m2.a[0] + m1.a[1]*m2.a[4] + m1.a[2]*m2.a[8] + m1.a[3]*m2.a[12];
			mat.a[1] = m1.a[0]*m2.a[1] + m1.a[1]*m2.a[5] + m1.a[2]*m2.a[9] + m1.a[3]*m2.a[13];
			mat.a[2] = m1.a[0]*m2.a[2] + m1.a[1]*m2.a[6] + m1.a[2]*m2.a[10] + m1.a[3]*m2.a[14];
			mat.a[3] = m1.a[0]*m2.a[3] + m1.a[1]*m2.a[7] + m1.a[2]*m2.a[11] + m1.a[3]*m2.a[15];

			mat.a[4] = m1.a[4]*m2.a[0] + m1.a[5]*m2.a[4] + m1.a[6]*m2.a[8] + m1.a[7]*m2.a[12];
			mat.a[5] = m1.a[4]*m2.a[1] + m1.a[5]*m2.a[5] + m1.a[6]*m2.a[9] + m1.a[7]*m2.a[13];
			mat.a[6] = m1.a[4]*m2.a[2] + m1.a[5]*m2.a[6] + m1.a[6]*m2.a[10] + m1.a[7]*m2.a[14];
			mat.a[7] = m1.a[4]*m2.a[3] + m1.a[5]*m2.a[7] + m1.a[6]*m2.a[11] + m1.a[7]*m2.a[15];

			mat.a[8] = m1.a[8]*m2.a[0] + m1.a[9]*m2.a[4] + m1.a[10]*m2.a[8] + m1.a[11]*m2.a[12];
			mat.a[9] = m1.a[8]*m2.a[1] + m1.a[9]*m2.a[5] + m1.a[10]*m2.a[9] + m1.a[11]*m2.a[13];
			mat.a[10] = m1.a[8]*m2.a[2] + m1.a[9]*m2.a[6] + m1.a[10]*m2.a[10] + m1.a[11]*m2.a[14];
			mat.a[11] = m1.a[8]*m2.a[3] + m1.a[9]*m2.a[7] + m1.a[10]*m2.a[11] + m1.a[11]*m2.a[15];

			mat.a[12] = m1.a[12]*m2.a[0] + m1.a[13]*m2.a[4] + m1.a[14]*m2.a[8] + m1.a[15]*m2.a[12];
			mat.a[13] = m1.a[12]*m2.a[1] + m1.a[13]*m2.a[5] + m1.a[14]*m2.a[9] + m1.a[15]*m2.a[13];
			mat.a[14] = m1.a[12]*m2.a[2] + m1.a[13]*m2.a[6] + m1.a[14]*m2.a[10] + m1.a[15]*m2.a[14];
			mat.a[15] = m1.a[12]*m2.a[3] + m1.a[13]*m2.a[7] + m1.a[14]*m2.a[11] + m1.a[15]*m2.a[15];
			return mat;
		}
		void scale(float s) noexcept;
		void translate(float x, float y, float z) noexcept;
		void rotate(float angleX, float angleY, float angleZ) noexcept;
		
	};
}

#endif	/* GRMATRIX_H */

