#ifndef GRSHAPE_H
#define	GRSHAPE_H

#include "grBaseTypes.h"



using namespace std;
namespace Graphonichk {
	template class EventDispatcher<EventMouseShape>;
	class ShapeRect;
	class ShapeBasic;
	class ShapeMatrix2D;
	class ShapeGroupRect;
	class ShapeGroupMatrix2D;
	class ShapeGroupBasic;
	
	class ShapeRectGateBasic;
	class ShapeRectGateMatrix2D;
	class ShapeRectGateMatrix3D;

	class Bitmap;
	class Buffer;
	class FPoint;
	class FLines;
	class FRect;
	//class FCircle;
	
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
	
	
	class ShapeBasic {
	//CHECKED
	//CROSS
	protected:
		ShapeBasic(int crc32);
	public:
		virtual void trace();
		virtual int renderGL400();
		virtual int renderGL330();
		virtual int renderGL210();
		virtual int renderGL100();
		int crc32;
	};
	class ShapeGroupBasic :public ShapeBasic {
	protected:
		ShapeGroupBasic(int crc32);
	public:
		ShapeGroupBasic();
		virtual void trace();
		virtual int renderGL400();
		virtual int renderGL330();
		virtual int renderGL210();
		virtual int renderGL100();
		vector<ShapeBasic*> child;
	};
	class ShapeRect :public EventDispatcher<EventMouseShape>, public ShapeBasic {
		short meshVertex[12];
	protected:
		ShapeRect(int crc32);
	public:
		virtual void trace();
		virtual void drag(short x, short y);
		virtual void updateGlobalPosition();
		GLuint meshVAO, meshVBO;
		ShapeGroupRect* parent;
		Point offsetPos;
		short globalx, globaly, x, y;
		unsigned short width, height;
		bool visible;
		string name;
		
		bool mouseEventActive;
		bool mouseEventRollOver;
		
		void setVisible(bool vis);
		virtual int saveAsXML(FILE *str, unsigned short tab=0);
		virtual int addEventHandler( int type, void(*)(const EventMouseShape*));
		virtual int callEvent(EventMouseShape *e);
		
		virtual ShapeRect* globalHitTest(short x, short y);
	};
	class ShapeGroupRect :public Buffer, public ShapeRect {
	protected:
		ShapeGroupRect(int crc32);
	public:
		enum {CRC32=0xC489C679};
		enum BUFFER_TYPE {
			PREDEFINED_LIST_SH,
			TO_TEXTURE
		};
		ShapeGroupRect();
		virtual void trace();
		
		virtual bool addChild(ShapeRect *sh);
		virtual bool removeChild(ShapeRect *sh);
		virtual bool setChildDepth(ShapeRect *sh, unsigned short depth);
		virtual unsigned int getChildDepth(ShapeRect *sh);
		virtual ShapeRect* getChild(string str);
		
		virtual void drag(short, short);
		virtual void updateGlobalPosition();
		virtual void updateRect();
		virtual void updateRect(ShapeRect *sh);
		virtual void setBuffer(BUFFER_TYPE, char);
		
		virtual int renderGL100();
		virtual int renderGL400();
		virtual int renderGL330();
		virtual int renderGL210();
		virtual int bufferGLComptAll();
		virtual int bufferGL400();
		virtual int bufferGL330();
		virtual int bufferGL210();
		virtual bool bufferMode(bool mode);
		
		int saveAsXML(FILE *str, unsigned short tab=0);
		
		virtual vector<ShapeRect*>* getChildShape();
		virtual void getChildShape(vector<ShapeRect*>*);
		
		virtual ShapeRect* globalHitTest(short x, short y);
		virtual int callEvent(EventMouseShape *e);
		
		GLuint totalShapeVertexInit;
		vector<ShapeRect*> child;
		vector<ShapeRect*> *shapeCache;
		//Windows *window;
		bool chengeRect;
		bool cutTheRect;
		unsigned int totalShape, totalDir;
		#ifdef WIN32
			HANDLE addChildLock;
		#endif
	};
	
	class ShapeMatrix2D :public ShapeBasic {
	public:
		ShapeMatrix2D();
		TransformMatrix matrix;
	};
	class ShapeGroupMatrix2D :public ShapeMatrix2D {
	protected:
		ShapeGroupMatrix2D(int crc32);
	public:
		ShapeGroupMatrix2D();
		//virtual void trace();
		virtual int renderGL100();
		virtual int renderGL400();
		virtual int renderGL330();
		virtual int renderGL210();
		
		vector<ShapeMatrix2D*> child;
	};
	
	class ShapeRectGateMatrix2D :public ShapeRect {
	public:
		ShapeRectGateMatrix2D();
		//virtual void trace();
		virtual int renderGL100();
		virtual int renderGL400();
		virtual int renderGL330();
		virtual int renderGL210();
		ShapeGroupMatrix2D group;
		ViewMatrix view;
	};
	
	class ShapeRectGateMatrix3D :public ShapeRect {
	public:
		ShapeRectGateMatrix3D();
		//virtual void trace();
		virtual int renderGL100();
		virtual int renderGL400();
		virtual int renderGL330();
		virtual int renderGL210();
		ShapeGroupBasic group;
		ViewMatrix view;
	};
	
	
	
	
	
	
	class Bitmap :public ShapeRect {
	private:
		static vector<Bitmap*> updateBuffer;
	public:
		enum {CRC32=0xEFC15B9A};
		Bitmap(Texture*);
		//~Bitmap();
		void trace();
		int renderGL100();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		int saveAsXML(FILE* str, unsigned short tab);
		Texture *tex;
		
		GLuint vao, vbo;
		static void updateBitmaps();
	};
	class FPoint :public ShapeRect {
	  public:
		enum {CRC32=0xD88563EF};
		FPoint(int, uint32_t);
		int renderGL100();
		int radius;
		argb color;
	};
	class FLines :public ShapeRect {
	  public:
		enum {CRC32=0x20211C5D};
		FLines(void*, short, short, uint32_t);
		int renderGL100();
		short length, lineWidth;
		argb color;
		short *arr;
	};
	class FRect :public ShapeRect {
	  public:
		enum {CRC32=0x27311957};
		FRect(short width, short height, uint32_t backgroundColor);
		int renderGL100();
		int renderGL330();
		argb backgroundColor;
		GLuint vao, vbo;
	};
	class Model3D;
	
	class Scene3D :public ShapeRect {
	public:
		Scene3D();
		int renderGL100();
		int renderGL330();
		ViewMatrix viewMatrix;
		TransformMatrix3D viewPosMatrix;
		TransformMatrix3D transformMatrix;
		Model3D *model;
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

