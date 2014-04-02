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
	class TransformMatrix3D {
	public:
		float a[16];
		TransformMatrix3D();
		TransformMatrix3D(const TransformMatrix3D *m);
		void trace();
		void loadIdentity();
		void multiple(TransformMatrix3D &mt);
		void scale(float s);
		void translate(float x, float y, float z);
		void rotate(float angleX, float angleY, float angleZ);
	};
	typedef struct {
		float a[16];
	} ViewMatrix;
	ViewMatrix ViewMatrixIdentity();
	ViewMatrix ViewMatrixOrtho(float r, float l, float t, float b, float f, float n);
	ViewMatrix ViewMatrixPerspective(float r, float l, float t, float b, float f, float n);
	ViewMatrix ViewMatrixPerspective2(float fov, float aspect, float f, float n);
	void ViewMatrixRotate(ViewMatrix &vm);
	void ViewMatrixTranslate(ViewMatrix &vm, float x, float y, float z);
	
}

#endif	/* GRMATRIX_H */

