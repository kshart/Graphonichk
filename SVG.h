#ifndef SVG_H
#define	SVG_H

#include "grMain.h"
#include "grDOM.h"
#include <memory>
using namespace std;
using namespace DOM;
using Graphonichk::Array;
using Graphonichk::Screen;
using Graphonichk::TransformMatrix;

/* ContainerElement: a, defs, glyph, g, marker, mask, missingGlyph, pattern, svg, switch, symbol */
/* GraphicsElement: circle, ellipse, image, line, path, polygon, polyline, rect, text, use */
namespace SVG {
	class Group;
	class Symbol;
	
	class _length {
	public:
		float value;
		enum TYPE:short {
			_PX=0,
			_IN,
			_CM,
			_MM,
			_EM,
			_EX,
			_PT,
			_PC
		} type = _PX;
		_length() {};
		_length(float v, TYPE type=TYPE::_PX) {
			this->value = v;
			this->type = type;
		};
		float getPixel() const noexcept {
			switch (this->type) {
				case _length::_PX: return this->value;
				case _length::_IN: return this->value*Screen::dpi;
				case _length::_CM: return (this->value*Screen::dpi)/25.4*10;
				case _length::_MM: return (this->value*Screen::dpi)/25.4;
				case _length::_EM: return this->value;
				case _length::_EX: return this->value;
				case _length::_PT: return this->value;
				case _length::_PC: return this->value;
			}
			return 0.0f;
		}
	};
	typedef _length Length, Coordinate;
	
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
	};
	
	class OtherPropertiesForVisualMedia {
		void* lip;
		void* color;
		void* cursor;
		void* display;
		void* overflow;
		void* visibility;
	};
	class ClippingProperties {
		void* clipPath;
		void* clipRule;
		void* mask;
		void* opacity;
	};
	class FilterEffectsProperties {
		void* enableBackground;
		void* filter;
		void* floodColor;
		void* floodOpacity;
		void* lightingColor;
	};
	class GradientProperties {
		void* stopColor;
		void* stopOpacity;
	};
	class InteractivityProperties {
		void* pointerEvents;
	};
	class ColorProperties {
		void* colorInterpolation;
		void* colorInterpolationFilters;
		void* colorProfile;
		void* colorRendering;
		void* fill;
		void* fillOpacity;
		void* fillRule;
		void* imageRendering;
		void* marker;
		void* markerEnd;
		void* markerMid;
		void* markerStart;
		void* shapeRendering;
		void* stroke;
		void* strokeDasharray;
		void* strokeDashoffset;
		void* strokeLinecap;
		void* strokeLinejoin;
		void* strokeMiterlimit;
		void* strokeOpacity;
		void* strokeWidth;
		void* textRendering;
	};
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
	};
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
	
	/*class SVGStringList:protected vector<string*> {
	protected:
		
	public:
		size_t numberOfItems;

		void clear();																// raises(DOMException);
		DOMString initialize(DOMString newItem);									// raises(DOMException);
		DOMString getItem(size_t index);									// raises(DOMException);
		DOMString insertItemBefore(DOMString newItem, size_t index);	// raises(DOMException);
		DOMString replaceItem(DOMString newItem, size_t index);		// raises(DOMException);
		DOMString removeItem(size_t index);								// raises(DOMException);
		DOMString appendItem(DOMString newItem);									// raises(DOMException);
	};

	class SVGElement;
	class SVGAnimatedBoolean;
	class SVGAnimatedString;
	class SVGStringList;
	class SVGAnimatedEnumeration;
	class SVGAnimatedInteger;
	class SVGNumber;
	class SVGAnimatedNumber;
	class SVGNumberList;
	class SVGAnimatedNumberList;
	class SVGLength;
	class SVGAnimatedLength;
	class SVGLengthList;
	class SVGAnimatedLengthList;
	class SVGAngle;
	class SVGAnimatedAngle;
	class SVGColor;
	class SVGICCColor;
	class SVGRect;
	class SVGAnimatedRect;
	class SVGUnitTypes;
	class SVGStylable;
	class SVGLocatable;
	class SVGTransformable;
	class SVGTests;
	class SVGLangSpace;
	class SVGExternalResourcesRequired;
	class SVGFitToViewBox;
	class SVGZoomAndPan;
	class SVGViewSpec;
	class SVGURIReference;
	class SVGCSSRule;
	class SVGRenderingIntent;*/
	/*class SVGAnimatedRect {
	public:
		SVGRect baseVal;
		SVGRect animVal;
	};
	class SVGFitToViewBox {
	public:
		SVGAnimatedRect viewBox;
		SVGAnimatedPreserveAspectRatio preserveAspectRatio;
	};
	
	class SVGElement :public Element {
	protected:
		SVGSVGElement* _rootSVG = nullptr;
	public:
		DOMString id;// setraises(DOMException);
		DOMString xmlbase;// setraises(DOMException);
		inline const SVGSVGElement* ownerSVGElement() noexcept {
			return _rootSVG;
		}
		SVGElement* viewportElement() noexcept;
	};
	class SVGRect {
	public:
		SVGRect() :x(0), y(0), width(0), height(0) {};
		SVGRect(float x, float y, float width, float height) :x(x), y(y), width(width), height(height) {};
		float x;		// setraises(DOMException);
		float y;		// setraises(DOMException);
		float width;	// setraises(DOMException);
		float height;	// setraises(DOMException);
	};
	class SVGPoint {
	public:
		SVGPoint() :x(0), y(0) {};
		SVGPoint(float x, float y) :x(x), y(y) {};
		float x;// setraises(DOMException);
		float y;// setraises(DOMException);
		SVGPoint matrixTransform(SVGMatrix matrix);
	};
	class SVGMatrix {
	public:
		float a;// setraises(DOMException);
		float b;// setraises(DOMException);
		float c;// setraises(DOMException);
		float d;// setraises(DOMException);
		float e;// setraises(DOMException);
		float f;// setraises(DOMException);

		SVGMatrix multiply(SVGMatrix secondMatrix);
		SVGMatrix inverse();// raises(SVGException);
		SVGMatrix translate(float x, float y);
		SVGMatrix scale(float scaleFactor);
		SVGMatrix scaleNonUniform(float scaleFactorX, float scaleFactorY);
		SVGMatrix rotate(float angle);
		SVGMatrix rotateFromVector(float x, float y);// raises(SVGException);
		SVGMatrix flipX();
		SVGMatrix flipY();
		SVGMatrix skewX(float angle);
		SVGMatrix skewY(float angle);
	};
	
	class SVGSVGElement : SVGElement,
                          SVGTests,
                          SVGLangSpace,
                          SVGExternalResourcesRequired,
                          SVGStylable,
                          SVGLocatable,
                          SVGFitToViewBox,
                          SVGZoomAndPan,
                          DocumentEvent,
                          ViewCSS,
                          DocumentCSS {
		SVGAnimatedLength x;
		SVGAnimatedLength y;
		SVGAnimatedLength width;
		SVGAnimatedLength height;
		DOMString contentScriptType;// setraises(DOMException);
		DOMString contentStyleType;// setraises(DOMException);
		SVGRect viewport;
		float pixelUnitToMillimeterX;
		float pixelUnitToMillimeterY;
		float screenPixelToMillimeterX;
		float screenPixelToMillimeterY;
		boolean useCurrentView;
		SVGViewSpec currentView;
		float currentScale;
		SVGPoint currentTranslate;

		unsigned long suspendRedraw(unsigned long maxWaitMilliseconds);
		void unsuspendRedraw(unsigned long suspendHandleID);
		void unsuspendRedrawAll();
		void forceRedraw();
		void pauseAnimations();
		void unpauseAnimations();
		boolean animationsPaused();
		float getCurrentTime();
		void setCurrentTime(float seconds);
		NodeList getIntersectionList(SVGRect rect, SVGElement referenceElement);
		NodeList getEnclosureList(SVGRect rect, SVGElement referenceElement);
		boolean checkIntersection(SVGElement element, SVGRect rect);
		boolean checkEnclosure(SVGElement element, SVGRect rect);
		void deselectAll();
		SVGNumber createSVGNumber();
		SVGLength createSVGLength();
		SVGAngle createSVGAngle();
		SVGPoint createSVGPoint();
		SVGMatrix createSVGMatrix();
		SVGRect createSVGRect();
		SVGTransform createSVGTransform();
		SVGTransform createSVGTransformFromMatrix(SVGMatrix matrix);
		Element getElementById(DOMString elementId);
	};
	
	*/
	class Symbol {
	public:
		virtual int renderGL100() {
			fputs("SVG::Symbol::renderGLComptAll\n", iovir);
			return false;
		}
		virtual int renderGL330() {
			fputs("SVG::Symbol::renderGL330\n", iovir);
			return false;
		}
		
		TransformMatrix matrix;
	};
	class Group :public Symbol {
	public:
		int renderGL100() override;
		int renderGL330() override;
		
		vector<Symbol*> child;
	};
	//rect circle ellipse line polyline
	class BasicShapeRect :public Symbol {
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
	class BasicShapeCircle :public Symbol {
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
	class BasicShapeEllipse :public Symbol {
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
	class BasicShapeLine :public Symbol {
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
	class BasicShapePolyline :public Symbol {
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
	class BasicShapePolygon :public Symbol {
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

