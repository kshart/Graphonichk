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
		virtual void trace() override;
		virtual int renderGL400() override;
		virtual int renderGL330() override;
		virtual int renderGL210() override;
		virtual int renderGL100() override;
		vector<ShapeBasic*> child;
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
	class ShapeRect :public EventDispatcher<EventMouseShape>, public ShapeBasic {
	protected:
		bool _toUpdate;
		svec2 offset, global, local;
		unsigned short width, height;
		friend class ShapeGroupRect;
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
		virtual bool removeChild(const ShapeRect *sh) ;
		virtual bool setChildDepth(ShapeRect *sh, unsigned short depth);
		virtual unsigned int getChildDepth(ShapeRect *sh) const;
		virtual ShapeRect* getChild(string str) const;
		
		virtual void setPosition(short x, short y) override;
		virtual void updateGlobalPosition() override;
		virtual void updateRect();
		
		virtual int renderGL100() override;
		virtual int renderGL400() override;
		virtual int renderGL330() override;
		virtual int renderGL210() override;
		
		//int saveAsXML(FILE *str, unsigned short tab=0);
		
		virtual vector<ShapeRect*>* getChildShape();
		virtual void getChildShape(vector<ShapeRect*>*);
		
		virtual ShapeRect* globalHitTest(short x, short y) override;
		virtual int callEvent(EventMouseShape *e) override;
		
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
		virtual int renderGL100() override;
		virtual int renderGL400() override;
		virtual int renderGL330() override;
		virtual int renderGL210() override;
		
		vector<ShapeMatrix2D*> child;
	};
	
	class ShapeRectGateMatrix2D :public ShapeRect {
	public:
		ShapeRectGateMatrix2D();
		//virtual void trace();
		virtual int renderGL100() override;
		virtual int renderGL400() override;
		virtual int renderGL330() override;
		virtual int renderGL210() override;
		ShapeGroupMatrix2D group;
		Matrix3D view;
	};
	class ShapeRectGateMatrix3D :public ShapeRect {
	public:
		ShapeRectGateMatrix3D();
		//virtual void trace();
		virtual int renderGL100() override;
		virtual int renderGL400() override;
		virtual int renderGL330() override;
		virtual int renderGL210() override;
		ShapeGroupBasic group;
		Matrix3D view;
	};
	
	
	class ShapePostEffect :public ShapeBasic {
	private:
		//friend class ShapeGroupRect;
	public:
		enum {CRC32=0xC499C679};
		ShapePostEffect(GLShader *shader);
		int renderGL330() override;
		GLShader *shader;
	};
	class ShapeMain :public ShapeGroupRect {
	private:
		//friend class ShapeGroupRect;
	public:
		enum {CRC32=0xC489C679};
		ShapeMain();
		void setRect(short w, short h) override;
		int renderGL330() override;
		int renderGL100() override;
		
		ShapeGroupBasic postEffects;
		struct _fbo {
			GLuint fbo, vao, vbo;
			Texture *color, *depth;
		} frameBuffer;
	};
	class ShapeMainInitTask :public EachFrameTask {
	public:
		ShapeMainInitTask(ShapeMain *sh) :sh(sh) {}
		int processExecute() {
			if (this->sh->frameBuffer.color->GLID==0 || this->sh->frameBuffer.depth->GLID==0) return false;
			svec2 rect = this->sh->getRect();
			glGenVertexArrays(1, &this->sh->frameBuffer.vao);
			glBindVertexArray(this->sh->frameBuffer.vao);
			glGenBuffers(1, &this->sh->frameBuffer.vbo);
			glBindBuffer(GL_ARRAY_BUFFER, this->sh->frameBuffer.vbo);
			glBufferData(GL_ARRAY_BUFFER, 2*sizeof(short), &rect, GL_STATIC_DRAW);
			glVertexAttribPointer(ShaderShRect::POSITION, 2, GL_SHORT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(ShaderShRect::POSITION);
			glGenFramebuffers(1, &this->sh->frameBuffer.fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, this->sh->frameBuffer.fbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->sh->frameBuffer.color->GLID, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->sh->frameBuffer.depth->GLID, 0);
			return true;
		}
		ShapeMain *sh;
	};
	
	class Bitmap :public ShapeRect {
	private:
		static vector<Bitmap*> updateBuffer;
	public:
		enum {CRC32=0xEFC15B9A};
		Bitmap(Texture*);
		void trace() override;
		int renderGL330() override;
		int renderGL100() override;
		Texture *tex;
		
		static void updateBitmaps();
	};
	class BitmapAtlas :public ShapeRect {
	public:
		enum {CRC32=0x7DFA5D4D};
		BitmapAtlas(Texture*, short id);
		bool setRectID(short id);
		void trace() override;
		int renderGL330() override;
		int renderGL100() override;
		Texture *tex;
		short texRectID;
		
		//GLuint vboRectID;
	};
	
	class FPoint :public ShapeRect {
	  public:
		enum {CRC32=0xD88563EF};
		FPoint(int, uint32_t);
		int renderGL100() override;
		int radius;
		argb color;
	};
	class FLines :public ShapeRect {
	public:
		typedef struct {
			enum TYPE:char {
				line, bezier3
			} type;
			svec2 p1, p2;
		} Segment;
		enum {CRC32=0x20211C5D};
		FLines(Segment *sgs, short length, short lineWidth, argb color);
		int renderGL100() override;
		short lineWidth;
		argb color;
		Array<Segment> segments;
	};
	class FRect :public ShapeRect {
	  public:
		enum {CRC32=0x27311957};
		FRect(short width, short height, uint32_t backgroundColor);
		int renderGL100() override;
		int renderGL330() override;
		argb backgroundColor;
		GLuint vao, vbo;
	};
	class Model3D;
	
	class Scene3D :public ShapeRect {
	public:
		Scene3D();
		int renderGL100() override;
		int renderGL330() override;
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

