#ifndef SVG_H
#define	SVG_H

#include "grBaseTypes.h"
using namespace std;
using Graphonichk::TransformMatrix;

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
		Coordinate x, y;
	} BasicPoint;
	typedef struct {
		short type;
		float value;
	} Angle;
	
	typedef struct {
		float value;
	} Percentage;
	typedef struct {
		char *id, *xmlBase, *xmlLang, *xmlSpace;
	} CoreAttributes;
	typedef struct {
		char *onfocusin, *onfocusout, *onactivate, *onclick, *onmousedown, *onmouseup, *onmouseover, *onmousemove, *onmouseout, *onload;
	} GraphicalEventAttributes;
	
	class DataTypes {
	public:
		static float		getPixels(Length l);
		static Length		getLength(const char *str);
		static Coordinate	getCoordinate(const char *str);
		static Angle		getAngle(const char *str);
		static TransformMatrix	getTransformMatrix(const char *str);
	};
	
	
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
	//rect circle ellipse line polyline
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
		
		GLuint vao, vbo;
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
	class BasicShapeEllipse :public Symbol{
	public:
		BasicShapeEllipse();
		int renderGLComptAll();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		
		//class;
		//style;
		//transform;
		bool externalResourcesRequired;
		Coordinate cx, cy;
		Length rx, ry;
	};
	class BasicShapeLine :public Symbol{
	public:
		BasicShapeLine();
		int renderGLComptAll();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		
		//class;
		//style;
		//transform;
		bool externalResourcesRequired;
		Coordinate x1, y1, x2, y2;
	};
	class BasicShapePolyline :public Symbol{
	public:
		BasicShapePolyline();
		int renderGLComptAll();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		
		//class;
		//style;
		//transform;
		bool externalResourcesRequired;
		BasicPoint *points;
		unsigned int length;
	};
	class BasicShapePolygon :public Symbol{
	public:
		BasicShapePolygon();
		int renderGLComptAll();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		
		//class;
		//style;
		//transform;
		bool externalResourcesRequired;
		BasicPoint *points;
		unsigned int length;
	};
}
#include "SVGPresentationAttributes.h"
#endif	/* SVG_H */

