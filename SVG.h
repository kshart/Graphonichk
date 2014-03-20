#ifndef SVG_H
#define	SVG_H

#include "grBaseTypes.h"
using namespace std;
using Graphonichk::Array;
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
		unsigned char type, idColorWord;
		unsigned char r, g, b;
	} Color;
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
	
	typedef struct {
		void* font;
		void* fontFamily;
		void* fontSize;
		void* fontSizeAdjust;
		void* fontStretch;
		void* fontStyle;
		void* fontVariant;
		void* fontWeight;
	} FontProperties;
	typedef struct {
		void* direction;
		void* letterSpacing;
		void* textDecoration;
		void* unicodeBidi;
		void* wordSpacing;
	} TextProperties;
	typedef struct {
		void* clip;
		void* color;
		
		Color fill;
		void* stroke;
		void* stopColor;
		void* floodColor;
		void* lightingColor;
		
		void* cursor;
		void* display;
		void* overflow;
		bool visibility;
	} VisualProperties;
	
	//class Array<ColorWord>;
	class DataTypes {
	public:
		typedef struct {
			char *str;
			uint8_t r, g, b;
		} ColorWord;
		static const ColorWord colorWords[147];
		static void init();
		static float		getPixels(Length l);
		static Color		getColor(const char *str);
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
		enum :GLuint {CRC32=0xB7D63381};
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
		//Color color;
		GLuint vao, vbo;
	};
	class BasicShapeCircle :public Symbol{
	public:
		enum :GLuint {CRC32=0xD4B76579};
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
		
		GLuint vao;
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
		enum :GLuint {CRC32=0xD114B4F6};
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
		
		GLuint vao, vbo;
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

