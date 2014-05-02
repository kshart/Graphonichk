#ifndef GROPENGL_H
#define	GROPENGL_H


//#define OPENGL_GET_PROC(p,n) n=(p)wglGetProcAddress(#n);
//if (n==NULL){printf("Loading extension '%s' fail (%d)\n", #n,GetLastError());}

#include "grMain.h"

#define SET_SHADER(sh) if (GLShader::shader->crc32!=sh::CRC32) GLShader::setShader(sh::prog);
#define SET_SHADER_H(sh) if (GLShader::shader!=sh) GLShader::setShader(sh);
using namespace std;
namespace Graphonichk {
	class GLShader;
	class GLShaderLoadTask;
	
	class GLShader {
	protected:
		GLShader(int crc32);
	public:
		//int setVertexShader(const char *str, size_t size);
		//int setFragmentShader(const char *str, size_t size);
		//int make();
		
		//static stack<GLShader*> glslBuffer;
		//static void clearShader();
		//static void pushShader(GLShader *shader);
		//static void popShader();
		static GLShader* shader;
		static void setShader(GLShader *shader);
		
		int crc32;
		GLuint shaderProgram, vertexShader, fragmentShader, geometryShader;
		//bool fragment, vertex;
		friend GLShaderLoadTask;
		virtual void init();
		virtual void mapping();
	};
	class ShaderShRect :public GLShader {
	protected:
		ShaderShRect(int crc32);
	public:
		enum :GLint{POSITION=0};
		GLint posRect, grShaderData;
		
		friend GLShaderLoadTask;
		virtual void mapping();
		virtual void init();
	};
	class ShaderBitmap :public ShaderShRect {
	public:
		enum {CRC32=0x587213EC};
		ShaderBitmap();
		static void init33();
		static ShaderBitmap* prog;
		
		GLint texture;
		
		friend GLShaderLoadTask;
		void init();
	};
	class ShaderBitmapAtlas :public ShaderShRect {
	public:
		enum {CRC32=0x587113EC};
		ShaderBitmapAtlas();
		static void init33();
		static ShaderBitmapAtlas* prog;
		
		GLint texture, coordRect, rectID;
		
		friend GLShaderLoadTask;
		void init();
	};
	class ShaderSVGmain :public GLShader {
	public:
		enum {CRC32=0x51};
		ShaderSVGmain();
		//static void init();
		static void init33();
		static ShaderSVGmain* prog;
		
		GLint position, fillColor, typeShape, circleTransform, grShaderData, transformMatrix;
		
		friend GLShaderLoadTask;
		void init();
	};
	class ShaderFPrimitiv :public GLShader {
	public:
		enum {CRC32=0x5123};
		ShaderFPrimitiv();
		//static void init();
		static void init33();
		static ShaderFPrimitiv* prog;
		
		GLint position, fillColor, grShaderData;
		
		friend GLShaderLoadTask;
		void init();
	};
	class ShaderF3D :public GLShader {
	public:
		enum {CRC32=0x5142123};
		ShaderF3D();
		//static void init();
		static void init33();
		static ShaderF3D* prog;
		
		GLint position, fillColor, transformMatrix, grShaderData;
		
		friend GLShaderLoadTask;
		void init();
	};
	class ShaderPost :public GLShader {
	public:
		enum {CRC32=0x5122123};
		ShaderPost();
		//static void init();
		static void init33();
		static ShaderPost* prog;
		
		GLint texture, grShaderData;
		
		friend GLShaderLoadTask;
		void init();
	};
	class ShaderBW :public GLShader {
	public:
		enum {CRC32=0x522123};
		ShaderBW();
		//static void init();
		static void init33();
		static ShaderBW* prog;
		
		GLint texture, grShaderData;
		
		friend GLShaderLoadTask;
		void init();
	};
	class OpenGL {
	  public:
		enum OPENGL_VER:char{
			VER_CORE_100,
			VER_CORE_210,
			VER_CORE_330,
			VER_CORE_400
		};
		static OPENGL_VER ver;
		static void trace();
		
		typedef struct {
			short x, y, width, height;
		} viewport;
		static vector<viewport> viewportBuffer;
		static void pushViewport();
		static void popViewport();
		static void setViewport(short, short, short, short);

		static vector<Matrix3D> viewMatrixBuffer;
		static void clearViewMatrix();
		static void pushViewMatrix();
		static void popViewMatrix();
		static void setViewMatrix(Matrix3D &view);
		static void multViewMatrix(Matrix3D &view);
		static GLuint grShaderData, circleBuffer, textureGLID;
		static int init(OPENGL_VER);
	};
	
	
	class GLShaderLoadTask :public EachFrameTask {
	public:
		GLShaderLoadTask(GLShader *t, const GLchar *vs,  const GLchar *fs, const GLchar *gs=nullptr);
		int processExecute();
		GLShader *shader;
		const GLchar *vs, *fs, *gs;
	};
};

#endif	/* GROPENGL_H */

