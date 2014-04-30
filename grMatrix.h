#ifndef GRMATRIX_H
#define	GRMATRIX_H

namespace Graphonichk {
	class TransformMatrix {
	public:
		float a, b, c, d, e, f;
		TransformMatrix();
		TransformMatrix(float a, float b, float c, float d, float e, float f);
		void trace();
		void loadIdentity();
		void scale(float s);
		void scale(float sx, float sy);
		void translate(float x, float y);
		void rotate(float angle);
		void skewX(float angle);
		void skewY(float angle);
	};
	class Matrix3D {
	public:
		float a[16];
		Matrix3D();
		Matrix3D(const Matrix3D *m);
		static Matrix3D ViewOrtho(float r, float l, float t, float b, float f, float n);
		static Matrix3D ViewPerspective(float r, float l, float t, float b, float f, float n);
		static Matrix3D ViewPerspective2(float fov, float aspect, float f, float n);
		void loadIdentity();
		void trace();
		
		void multiple(Matrix3D &mt);
		void scale(float s);
		void translate(float x, float y, float z);
		void rotate(float angleX, float angleY, float angleZ);
		
	};
}

#endif	/* GRMATRIX_H */

