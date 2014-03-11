/* 
 * File:   grOpenGL.h
 * Author: Артем
 *
 * Created on 3 Март 2014 г., 21:47
 */

#ifndef GROPENGL_H
#define	GROPENGL_H


#ifndef GL_MAJOR_VERSION
#define GL_MAJOR_VERSION 33307
#endif
#ifndef GL_MINOR_VERSION
#define GL_MINOR_VERSION 33308
#endif
#define OPENGL_GET_PROC(p,n) n=(p)wglGetProcAddress(#n); \
	if (n==NULL){printf("Loading extension '%s' fail (%d)\n", #n,GetLastError());}

#include "grBaseTypes.h"

using namespace std;
namespace Graphonichk {
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
		GLuint shaderProgram, vertexShader, fragmentShader;
		//bool fragment, vertex;
	};
	class ShaderBitmap :public GLShader {
	public:
		enum {CRC32=0x587213EC};
		ShaderBitmap();
		static void init();
		static void init33();
		static ShaderBitmap* prog;
		
		GLint position, texCoord, texture, viewMatrix;
	};
	class ShaderSVGmain :public GLShader {
	public:
		enum {CRC32=0x51};
		ShaderSVGmain();
		static void init();
		static void init33();
		static ShaderSVGmain* prog;
		
		GLint position, viewMatrix;
	};
	class OpenGL {
	  public:
		enum OPENGL_VER:char{
			VER_COMPTABLE_ALL,
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

		static vector<ViewMatrix> viewMatrixBuffer;
		static void clearViewMatrix();
		static void pushViewMatrix();
		static void popViewMatrix();
		static void setViewMatrix(ViewMatrix view);
		static GLuint viewMatrix;
		static int init(OPENGL_VER);
	};
};

#endif	/* GROPENGL_H */

