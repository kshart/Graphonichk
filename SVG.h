/* 
 * File:   SVG.h
 * Author: Артем
 *
 * Created on 19 Февраль 2014 г., 14:53
 */

#ifndef SVG_H
#define	SVG_H

#include <vector>
using namespace std;

namespace SVG {
	class Group;
	class Symbol;
	
	typedef struct {
		enum TYPE:short {
			_PX=0,
			_IN,
			_CM,
			_MM,
			_EM,
			_EX,
			_PT,
			_PC
		};
		TYPE type;
		float value;
	} Length, Coordinate;
	typedef struct {
		short type;
		float value;
	} Angle;
	typedef struct {
		float value;
	} Percentage;
	class TransformMatrix {
	public:
		float a, b, c, d, e, f;
		TransformMatrix();
		TransformMatrix(float a, float b, float c, float d, float e, float f);
		void trace();
		void loadIdentity();
		void scale(Length s);
		void scale(Length sx, Length sy);
		void translate(Coordinate x, Coordinate y);
		void rotate(Angle angle);
		
		
		void skewX(Angle angle);
		void skewY(Angle angle);
	};
	typedef struct {
		char *id, *xmlBase, *xmlLang, *xmlSpace;
	} CoreAttributes;
	typedef struct {
		char *onfocusin, *onfocusout, *onactivate, *onclick, *onmousedown, *onmouseup, *onmouseover, *onmousemove, *onmouseout, *onload;
	} GraphicalEventAttributes;
	
	class Symbol {
	public:
		//SVGElement();
		Symbol();
		virtual int renderGLComptAll();
		virtual int renderGL400();
		virtual int renderGL330();
		virtual int renderGL210();
		
		TransformMatrix matrix;
	};
	class Group :public Symbol {
	public:
		Group();
		virtual int renderGLComptAll();
		virtual int renderGL400();
		virtual int renderGL330();
		virtual int renderGL210();
		
		vector<Symbol*> child;
	};
	
	class BasicShapeRect :public Symbol{
	public:
		BasicShapeRect();
		int renderGLComptAll();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		
		//class;
		//style;
		//transform;
		bool externalResourcesRequired;
		Coordinate x, y;
		Length width, height;
	};
	class BasicShapeCircle :public Symbol{
	public:
		BasicShapeCircle();
		int renderGLComptAll();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		
		//class;
		//style;
		//transform;
		bool externalResourcesRequired;
		Coordinate cx, cy;
		Length r;
	};
}
#include "SVGPresentationAttributes.h"
#endif	/* SVG_H */

