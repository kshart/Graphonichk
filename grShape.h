/* 
 * File:   grShape.h
 * Author: Артём Каширин
 *
 * Created on 25 Август 2013 г., 19:10
 */

#ifndef GRSHAPE_H
#define	GRSHAPE_H

#include "grBaseTypes.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <GL/gl.h>


using namespace std;
namespace grEngine {
	class Shape;
	class Directory;
	class Bitmap;
	class Buffer;
	class FPoint;
	class FLines;
	class FRect;
	class FCircle;
	
	//vertex buffer coord
	
	class Buffer {
	  public:
		Buffer();
		virtual int bufferGLComptAll();
		virtual int bufferGL400();
		virtual int bufferGL330();
		virtual int bufferGL210();
		virtual bool switchOn();
		virtual bool switchOff();
		bool success;
		bool status;
		vector<Buffer*> bufChild;
		Texture *tex;
	};
	class Shape :public EventDispatcher<EventMouse> {
	  public:
		enum {
			TYPE_NULL,
			TYPE_DIRECTORY,
			TYPE_TEXTURE,
			TYPE_TEXTFIELD,
			
			TYPE_POINT,
			TYPE_LINE,
			TYPE_RECT,
			TYPE_CIRCLE
		};
		Shape(short type);
		virtual void trace();
		void drag(short x, short y);
		
		GLuint meshVAO;
		Directory* parent;
		Point offsetPos;
		short type;
		short globalx, globaly, x, y;
		unsigned short width, height;
		
		virtual int renderGLComptAll();
		virtual int renderGL400();
		virtual int renderGL330();
		virtual int renderGL210();
		
		virtual int callEvent(EventMouse *e);
		
		virtual Shape* globalHitTest(short x, short y);
	};
	class Bitmap :public Shape {
	  public:
		Bitmap(Texture*);
		//~Bitmap();
		void trace();
		int renderGLComptAll();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		Texture *tex;
	};
	class Directory :public Shape, public Buffer {
	  public:
		enum BUFFER_TYPE {
			PREDEFINED_LIST_SH,
			TO_TEXTURE
		};
		Directory();
		virtual void trace();
		virtual void addChild(Shape*);
		virtual void drag(short, short);
		virtual void updateChPos();
		//virtual int load(string);
		//virtual int loadFD(FILE*, long);
		//virtual int save(string);
		//virtual int saveFD(FILE*, long);
		//virtual int saveFFD(FILE*, sdHead*);
		virtual void setBuffer(BUFFER_TYPE, char);
		
		virtual int renderGLComptAll();
		virtual int renderGL400();
		virtual int renderGL330();
		virtual int renderGL210();
		virtual int bufferGLComptAll();
		virtual int bufferGL400();
		virtual int bufferGL330();
		virtual int bufferGL210();
		virtual bool switchOn();
		virtual vector<Shape*>* getChildShape();
		virtual void getChildShape(vector<Shape*>*);
		virtual Shape* globalHitTest(short x, short y);
		virtual int callEvent(EventMouse *e);
		//vector<unsigned short> childEvent
		
		GLuint totalShapeVertexInit;
		vector<Shape*> child;
		vector<Shape*> *shapeCache;
		//Windows *window;
		unsigned int totalShape, totalDir;
	};
	
	class FPoint :public Shape {
	  public:
		FPoint(int, uint32_t);
		int renderGLComptAll();
		int radius;
		argb color;
	};
	class FLines :public Shape {
	  public:
		FLines(void*, short, short, uint32_t);
		int renderGLComptAll();
		short length, lineWidth;
		argb color;
		short *arr;
	};
	class FRect :public Shape {
	  public:
		FRect(short, short, uint32_t);
		FRect(short, short, uint32_t, unsigned short);
		FRect(short, short, uint32_t, uint32_t, unsigned short);
		int renderGLComptAll();
		short borderSize;
		char background;
		argb borderColor, backgroundColor;
	};

	
	
}

#endif	/* GRSHAPE_H */

