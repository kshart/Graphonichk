/* 
 * File:   grWindows.h
 * Author: Артём Каширин
 *
 * Created on 25 Август 2013 г., 19:32
 */

#ifndef GRWINDOWS_H
#define	GRWINDOWS_H

#include <stack>
#include <vector>
#include <string>
#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/wglext.h>

#include "grBaseTypes.h"
#include "grEvent.h"

#ifndef GL_MAJOR_VERSION
#define GL_MAJOR_VERSION 33307
#endif
#ifndef GL_MINOR_VERSION
#define GL_MINOR_VERSION 33308
#endif
#define WIN_CLASS_NAME "grEWin"
#define OPENGL_GET_PROC(p,n) n=(p)wglGetProcAddress(#n); \
	if (n==NULL){printf("Loading extension '%s' fail (%d)\n", #n,GetLastError());}
//Graphonichk
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

		typedef struct {
			short left, top, right, bottom;
		} viewportMatrix;
		static vector<viewportMatrix> viewportMatrixBuffer;
		static void pushViewportMatrix();
		static void popViewportMatrix();
		static void setViewportMatrix(short, short, short, short);
		static int init(OPENGL_VER);
	};
	
	template class EventDispatcher<EventWindow>;
	template class EventDispatcher<EventKeyboard>;
	template class EventDispatcher<EventMouse>;
	class Windows :public EventDispatcher<EventWindow> {
	public:
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		static Windows* window;
		static void regFirstWin();
		static void deleteLastWin();
		
		Windows(short x, short y, short w, short h);
		void close();
		void hide();
		void show();
		void setFocus();
		void killFocus();
		
		void saveAsXML();
		
		void redraw();
		void redrawFBO();
		void resize(short, short);
		void loop();

		struct {
			EventDispatcher<EventKeyboard> keyboard;
			EventDispatcher<EventMouse> mouse;
		} events;
		HWND hWnd;
		HGLRC hRC;
		HDC hDC;
		HANDLE winThread, renderThread;
		DWORD winThreadID, renderThreadID;
		
		//vector<Bitmap*> bitmapUpdateBuffer;
		vector<Buffer*> FBOBuffer;
		ShapeGroupRect *root;
		bool visible, renderComplete;
		short x, y, width, height;
		int dpi;
	};
}
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
extern PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation;
extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
extern PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
// Shaders uniforms
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORMMATRIX4FVPROC   glUniformMatrix4fv;
extern PFNGLUNIFORM1IPROC glUniform1i;
//
#endif	/* GRWINDOWS_H */

