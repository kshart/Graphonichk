#ifndef GRSHAPE_H
#define	GRSHAPE_H

#include "grMain.h"



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
	class BitmapAtlas;
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
	private:
		bool _toUpdate;
		svec2 offset, global, local;
		unsigned short width, height;
		friend class ShapeGroupRect;
	protected:
		ShapeRect(int crc32);
	public:
		virtual void setPosition(short x, short y);
		virtual void setOffset(short x, short y);
		virtual void setBox(short x, short y, short w, short h);
		virtual void setRect(short w, short h);
		virtual void setVisible(bool vis);
		short getGlobalX() const;
		short getGlobalY() const;
		svec2 getGlobalPosition() const;
		short getX() const;
		short getY() const;
		svec2 getLocalPosition() const;
		short getOffsetX() const;
		short getOffsetY() const;
		svec2 getOffsetPosition() const;
		short getWidth() const;
		short getHeight() const;
		svec2 getRect() const;
		svec4 getBox() const;
		virtual void trace();
		virtual void updateGlobalPosition();
		ShapeGroupRect* parent;
		GLuint vao, vboRect;
		bool visible;
		string name;
		
		bool mouseEventActive;
		bool mouseEventRollOver;
		
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
		
		virtual void setPosition(short x, short y);
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
		Matrix3D view;
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
		Matrix3D view;
	};
	
	
	
	
	class ProcessingShapeRect :public ProcessingQueue<ShapeRect> {
	public:
		int performTasks();
	};
	class ShapeRectTask :public ProcessingShapeRect, public EachFrameTask {
	private:
	public:
		static ShapeRectTask task;
		int processExecute();
	};
	
	class Bitmap :public ShapeRect {
	private:
		static vector<Bitmap*> updateBuffer;
	public:
		enum {CRC32=0xEFC15B9A};
		Bitmap(Texture*);
		void trace();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		int renderGL100();
		int saveAsXML(FILE* str, unsigned short tab);
		Texture *tex;
		
		static void updateBitmaps();
	};
	class BitmapAtlas :public ShapeRect {
	private:
		static vector<BitmapAtlas*> updateBuffer;
	public:
		enum {CRC32=0xEFC15B9A};
		BitmapAtlas(Texture*);
		void trace();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		int renderGL100();
		Texture *tex;
		short texRectID;
		
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
		Matrix3D viewMatrix;
		Matrix3D viewPosMatrix;
		Matrix3D transformMatrix;
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

