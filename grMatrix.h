/* 
 * File:   grMatrix.h
 * Author: Артем
 *
 * Created on 22 Февраль 2014 г., 17:35
 */

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
	class ViewMatrix {
	public:
		float a[16];
		//Orto
		//ViewMatrix(float a, float b, float c, float d, float e, float f);
		void trace();
		ViewMatrix();
		ViewMatrix(float r, float l, float t, float b, float f, float n);//ortho
	};
	
}

#endif	/* GRMATRIX_H */

