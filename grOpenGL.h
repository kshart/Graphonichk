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
	  public:
		GLShader();
		int setVertexShader(const char *str, size_t size);
		int setFragmentShader(const char *str, size_t size);
		int make();
		static GLShader *glsl;
		static GLint matrixProjection, colorTexture;
		GLuint shaderProgram, vertexShader, fragmentShader;
		bool fragment, vertex;
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
		static int init(OPENGL_VER);
	};
};
#ifdef WIN32
// VAO
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNGLGENVERTEXARRAYSPROC    glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC    glBindVertexArray;
// VBO
extern PFNGLGENBUFFERSPROC    glGenBuffers;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLBINDBUFFERPROC    glBindBuffer;
extern PFNGLBUFFERDATAPROC    glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
// FBO
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebuffer;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2D;
// Shaders
extern PFNGLCREATEPROGRAMPROC     glCreateProgram;
extern PFNGLDELETEPROGRAMPROC     glDeleteProgram;
extern PFNGLLINKPROGRAMPROC       glLinkProgram;
extern PFNGLVALIDATEPROGRAMPROC   glValidateProgram;
extern PFNGLUSEPROGRAMPROC        glUseProgram;
extern PFNGLGETPROGRAMIVPROC      glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLCREATESHADERPROC      glCreateShader;
extern PFNGLDELETESHADERPROC      glDeleteShader;
extern PFNGLSHADERSOURCEPROC      glShaderSource;
extern PFNGLCOMPILESHADERPROC     glCompileShader;
extern PFNGLATTACHSHADERPROC      glAttachShader;
extern PFNGLDETACHSHADERPROC      glDetachShader;
extern PFNGLGETSHADERIVPROC       glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC  glGetShaderInfoLog;
// Shaders attributes
extern PFNGLGETATTRIBLOCATIONPROC		glGetAttribLocation;
extern PFNGLBINDATTRIBLOCATIONPROC		glBindAttribLocation;
extern PFNGLVERTEXATTRIBPOINTERPROC		glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC	glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
// Shaders uniforms
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORMMATRIX4FVPROC   glUniformMatrix4fv;
extern PFNGLUNIFORM1IPROC glUniform1i;
//
#endif

#endif	/* GROPENGL_H */

