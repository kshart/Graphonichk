#ifndef SVG_H
#define	SVG_H

#include "grMain.h"
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
		unsigned char r, g, b, a;
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
	
	class FontProperties {
		struct {
			enum :char { 
				inherit=0, ltr, rtl
			} value;
		} direction;
		void* font;
		void* fontFamily;
		void* fontSize;
		void* fontSizeAdjust;
		void* fontStretch;
		void* fontStyle;
		void* fontVariant;
		void* fontWeight;
	} ;
	class TextProperties {
		struct {
			enum :char { 
				inherit=0, ltr, rtl
			} value;
		} direction;
		struct {
			enum :char { 
				inherit=0, normal, length
			} value;
			Length _length;
		} letterSpacing;
		struct {
			enum :char { 
				inherit=0, none, decoration
			} value;
			char underline:1;
			char overline:1;
			char lineThrough:1;
			char blink:1;
		} textDecoration;
		struct {
			enum :char { 
				inherit=0, normal, embed, bidiOverride
			} value;
		} unicodeBidi;
		struct {
			enum :char { 
				inherit=0, normal, length
			} value;
			Length _length;
		} wordSpacing;
		
		//NOT CSS
		struct {
			enum :char { 
				inherit=0,
				auto_,
				baseline,
				beforeEdge,
				textBeforeEdge,
				middle,
				central,
				afterEdge,
				textAfterEdge,
				ideographic,
				alphabetic,
				hanging,
				mathematical
			} value;
		} alignmentBaseline;//auto_
		void* baselineShift;
		void* dominantBaseline;
		void* glyphOrientationHorizontal;
		void* glyphOrientationVertical;
		void* kerning;
		void* textAnchor;
		void* writingMode;
	} ;
	typedef struct {
		void* clip;
		void* color;
		
		Color fill;
		void* stroke;
		void* stopColor;
		void* floodColor;
		void* lightingColor;
		
		void* cursor;
		struct {
			enum :char { 
				inherit=0, 
				inline_, 
				block, 
				listItem, 
				runIn, 
				compact, 
				marker, 
				table, 
				inlineTable,
				tableRowGroup,
				tableHeaderGroup,
				tableFooterGroup,
				tableRow,
				tableColumnGroup,
				tableColumn,
				tableCell,
				tableCaption,
				none
			} value;
		} display;//inline_
		struct {
			enum :char { 
				inherit=0, visible, hidden, scroll, auto_
			} value;
		} overflow;
		struct {
			enum :char { 
				inherit=0, visible, hidden, collapse
			} value;
		} visibility;//visible
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
		virtual int renderGL100();
		virtual int renderGL330();
		
		TransformMatrix matrix;
	};
	class Group :public Symbol {
	public:
		Group();
		int renderGL100() override;
		int renderGL330() override;
		
		vector<Symbol*> child;
	};
	//rect circle ellipse line polyline
	class BasicShapeRect :public Symbol{
	public:
		enum :GLuint {CRC32=0xB7D63381};
		BasicShapeRect();
		int renderGL100() override;
		int renderGL330() override;
		
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
		int renderGL100() override;
		int renderGL330() override;
		
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
		int renderGL100() override;
		int renderGL330() override;
		
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
		int renderGL100() override;
		int renderGL330() override;
		
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
		int renderGL100() override;
		int renderGL330() override;
		
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
		int renderGL100() override;
		int renderGL330() override;
		
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

