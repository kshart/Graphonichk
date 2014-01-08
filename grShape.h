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
	template class EventDispatcher<EventMouseShape>;
	class Shape;
	class Directory;
	class Bitmap;
	class Buffer;
	class FPoint;
	class FLines;
	class FRect;
	class FCircle;
	
	class Buffer {
	protected:
		Buffer();
	public:
		virtual int bufferGLComptAll();
		virtual int bufferGL400();
		virtual int bufferGL330();
		virtual int bufferGL210();
		virtual bool bufferMode(bool mode);
		virtual bool bufferUpdate();
		bool bufferInit;
		bool bufferActivate;
		vector<Buffer*> bufChild;
		Texture *bufferTexture;
		GLuint bufferFrame;
	};
	class Shape :public EventDispatcher<EventMouseShape> {
	protected:
		///Конструктор Shape (использовать только в классах наслнд) 
		Shape(int crc32);
	public:
		///Конструктор Shape (использовать только в классах наслнд) 
		virtual void trace();
		///Конструктор Shape (использовать только в классах наслнд) 
		virtual void drag(short x, short y);
		virtual void updateGlobalPosition();
		///Конструктор Shape (использовать только в классах наслнд) 
		GLuint meshVAO;
		///Ссылка на родительскую директорию
		Directory* parent;
		///
		Point offsetPos;
		///Уникальный индетификатор классов наследников
		int crc32;
		///Конструктор Shape (использовать только в классах наслнд) 
		short globalx, globaly, x, y;
		///Конструктор Shape (использовать только в классах наслнд) 
		unsigned short width, height;
		///
		bool mouseEventActive;
		bool mouseEventRollOver;
		
		virtual int renderGLComptAll();
		virtual int renderGL400();
		virtual int renderGL330();
		virtual int renderGL210();
		
		virtual int addEventHandler( int type, void(*)(const EventMouseShape*));
		virtual int callEvent(EventMouseShape *e);
		
		virtual Shape* globalHitTest(short x, short y);
	};
	class Bitmap :public Shape {
	  public:
		enum {CRC32=0xEFC15B9A};
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
	protected:
		Directory(int crc32);
	public:
		enum {CRC32=0xC489C679};
		enum BUFFER_TYPE {
			PREDEFINED_LIST_SH,
			TO_TEXTURE
		};
		Directory();
		virtual void trace();
		virtual void addChild(Shape*);
		virtual void drag(short, short);
		virtual void updateGlobalPosition();
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
		virtual bool bufferMode(bool mode);
		
		virtual vector<Shape*>* getChildShape();
		virtual void getChildShape(vector<Shape*>*);
		virtual Shape* globalHitTest(short x, short y);
		virtual int callEvent(EventMouseShape *e);
		
		GLuint totalShapeVertexInit;
		vector<Shape*> child;
		vector<Shape*> *shapeCache;
		//Windows *window;
		unsigned int totalShape, totalDir;
	};
	
	class FPoint :public Shape {
	  public:
		enum {CRC32=0xD88563EF};
		FPoint(int, uint32_t);
		int renderGLComptAll();
		int radius;
		argb color;
	};
	class FLines :public Shape {
	  public:
		enum {CRC32=0x20211C5D};
		FLines(void*, short, short, uint32_t);
		int renderGLComptAll();
		short length, lineWidth;
		argb color;
		short *arr;
	};
	class FRect :public Shape {
	  public:
		enum {CRC32=0x27311957};
		FRect(short width, short height, uint32_t backgroundColor);
		FRect(short width, short height, uint32_t borderColor, unsigned short borderSize);
		FRect(short width, short height, uint32_t backgroundColor, uint32_t borderColor, unsigned short borderSize);
		int renderGLComptAll();
		short radius;
		short borderSize;
		char background;
		argb borderColor, backgroundColor;
	};

	
/*CRC32
Bitmap		0xEFC15B9A
Directory	0xC489C679
FPoint		0xD88563EF
FLines		0x20211C5D
FRect		0x27311957
*/
	
}

#endif	/* GRSHAPE_H */

