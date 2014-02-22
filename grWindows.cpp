
#include <stack>
#include <vector>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#include "grBaseTypes.h"

#include <math.h>
#include <thread>
using namespace std;
using namespace Graphonichk;//Graphonichk



OpenGL::OPENGL_VER OpenGL::ver;
vector<OpenGL::viewport> OpenGL::viewportBuffer;
vector<OpenGL::viewportMatrix> OpenGL::viewportMatrixBuffer;

DWORD WINAPI renderThread (void* sys);
DWORD WINAPI windowThread (void* sys);
DWORD WINAPI windowThread (void* sys) {
	printf("windowThread\n");
	//EventWindow *evWindow = new EventWindow();
	Windows::regFirstWin();
	Windows *win = Windows::window;
	win->hWnd = CreateWindowEx(WS_EX_COMPOSITED|WS_EX_APPWINDOW, WIN_CLASS_NAME, "111", WS_CAPTION|WS_SYSMENU|WS_SIZEBOX|WS_MINIMIZEBOX|WS_VISIBLE , win->x, win->y, win->width, win->height, NULL, NULL, System::hInstance, NULL);
	if (!Windows::window->hWnd) {
		printf("<Error str='CreateWindowEx fail (%d)'/>\n", GetLastError());
		return 0;
	}
	if ( !(win->hDC = GetDC(win->hWnd)) ) {
		printf("<Error str='GetDC fail (%d)'/>\n", GetLastError());
		return 0;
	}
	HANDLE mutex = CreateMutex(NULL, TRUE, NULL);
	win->renderThread = CreateThread(NULL, 0, renderThread, &mutex, 0, &win->renderThreadID);
	WaitForSingleObject(mutex, INFINITE);
	CloseHandle(mutex);
	ShowWindow(win->hWnd, SW_SHOW);
	SetForegroundWindow(win->hWnd);
	UpdateWindow(win->hWnd);
	SetFocus(win->hWnd);
#define PARENT_MUTEX ((HANDLE*)sys)[0]
	ReleaseMutex(PARENT_MUTEX);
#undef PARENT_MUTEX
	MSG msg;
	while (IsWindow(win->hWnd)) {
		while(PeekMessage(&msg, win->hWnd, 0, 0, PM_REMOVE)) DispatchMessage(&msg);
		Sleep(10);
	}
	
}
DWORD WINAPI renderThread (void* sys) {
	int format;
	PIXELFORMATDESCRIPTOR pfd;
	HGLRC hRCTemp;
	Windows *win = Windows::window;
	printf("<RenderThread hDC='%i' hRC='%i'/>\n", win->hDC, win->hRC);
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	format = ChoosePixelFormat(win->hDC, &pfd);
	if (!format || !SetPixelFormat(win->hDC, format, &pfd)) {
		printf("<Error str='Setting pixel format fail (%d)'/>\n", GetLastError());
		return 0;
	}
	if (!(hRCTemp = wglCreateContext(win->hDC)) || !wglMakeCurrent(win->hDC, hRCTemp)) {
		printf("<Error str='Сreating temp render context fail (%d)'/>\n", GetLastError());
		return 0;
	}
	win->hRC = hRCTemp;
	int hSize = GetDeviceCaps(win->hDC,HORZSIZE);
	int hRes = GetDeviceCaps(win->hDC,HORZRES);
	win->dpi = round((hRes/hSize)*25.4);
	printf("<LCD size='%i' res='%i' dpi='%i'/>\n", hSize, hRes, win->dpi );
	OpenGL::init(OpenGL::VER_COMPTABLE_ALL);
	win->resize(win->width, win->height);
	printf("SUCCESS\n");
#define PARENT_MUTEX ((HANDLE*)sys)[0]
	ReleaseMutex(PARENT_MUTEX);
#undef PARENT_MUTEX
	while (IsWindow(win->hWnd)) {
		Texture::texturesUpdate();
		Bitmap::updateBitmaps();
		if (!win->FBOBuffer.empty()) {
			win->redrawFBO();
		}
		#ifdef REDRAWN_BY_THE_ACTION
		if (!win->renderComplete) {
			win->redraw();
		}
		#else
		win->redraw();
		#endif
		Sleep(10);
	}
}

int ft_init () {
	FT_Library library;
	FT_Face face;
	//FT_Glyph  glyph;
	FT_GlyphSlot gs;
	FT_Vector  delta;
	FT_Matrix matrix;
	FT_BitmapGlyphRec bmp;
	int error;
	
	matrix.xx = (FT_Fixed)( cos( 0 ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( 0 ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( 0 ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( 0 ) * 0x10000L );
	delta.x = 0;
	delta.y = 0;
	
	error = FT_Init_FreeType( &library );
	if (error) return error;
	printf("FT_Init %i\n", error);
	error = FT_New_Face( library, "C:/Windows/Fonts/arial.ttf", 0, &face );
	if (error) return error;
	printf("FT_Init %i\n", error);
	printf("FT_Face %i\n", face->num_glyphs);
	//error = FT_Set_Pixel_Sizes(face, 0, 30);
	error = FT_Set_Char_Size(face, 0, 30*64, 72, 72 );
	//FT_Set_Transform(face, &matrix, &delta);
	error = FT_Load_Glyph( face, FT_Get_Char_Index(face, 'W'), FT_LOAD_DEFAULT );
	//error = FT_Get_Glyph( face->glyph, &glyph );
	error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_LIGHT   );
	gs = face->glyph;
	printf(" width %i ", gs->metrics.width>>6);
	printf(" height %i\n", gs->metrics.height>>6);
	//printf("FT_Init %i\n", error);
	//printf("FT_Init %i\n", error);
	printf("FT_Init %i\n", error);
	glRasterPos2s(100, 100);
	glPixelZoom(1,-1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(gs->bitmap.width, gs->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, gs->bitmap.buffer);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	//printf("GLERR %i\n");
	return error;
}

/*void FBO () {
	GLuint fb, color_tex;
	
	glGenTextures(1, &color_tex);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 2000, 2000, 0, GL_ALPHA, GL_UNSIGNED_BYTE, NULL);

	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, fb);
	glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, color_tex, 0);

		glViewport( 0, 0, 2000, 2000 );
		glLoadIdentity();
		gluOrtho2D( 0, 2000, 0, 2000 );
		/*glViewport( 0, 0, 994, 571 );
		glLoadIdentity();
		gluOrtho2D( 0, 994, 0, 571 );*//*
		
		glClearColor(1.0, 0.0, 0.0, 0.1);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glEnable( GL_BLEND );
		glEnable( GL_ALPHA_TEST );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable( GL_LINE_SMOOTH );
			glBegin( GL_LINES );
				glColor4ub(0xFF, 0, 0, 0xFF); glVertex2i(0, 0);
				glColor4ub(0xFF, 0, 0, 0xFF); glVertex2i(2000, 2000);
			glEnd();
		glDisable( GL_LINE_SMOOTH );
			ft_init();
		glDisable( GL_BLEND );
		glDisable( GL_ALPHA_TEST );
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	glViewport( 0, 0, 994, 571 );
	glLoadIdentity( );
	gluOrtho2D( 0, 994, 571, 0 );
	
	glEnable( GL_BLEND );
	glEnable( GL_ALPHA_TEST );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_TEXTURE_2D );
		glBindTexture(GL_TEXTURE_2D, color_tex);
		glColor4ub(0xFF,0xFF,0xFF,0xFF);
		glBegin( GL_QUADS );
			glTexCoord2d( 0.0, 0.0 );	glVertex2i(100, 0 );
			glTexCoord2d( 0.0, 1.0 );	glVertex2i(100, 2000 );
			glTexCoord2d( 1.0, 1.0 );	glVertex2i(2100, 2000 );
			glTexCoord2d( 1.0, 0.0 );	glVertex2i(2100, 0 );
		glEnd();
	glDisable( GL_TEXTURE_2D );
}
*/
#ifdef WIN32
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
PFNGLGENVERTEXARRAYSPROC    glGenVertexArrays    = NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC    glBindVertexArray    = NULL;

PFNGLGENBUFFERSPROC    glGenBuffers    = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLBINDBUFFERPROC    glBindBuffer    = NULL;
PFNGLBUFFERDATAPROC    glBufferData    = NULL;
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;

PFNGLGENFRAMEBUFFERSPROC			glGenFramebuffers		= NULL;
PFNGLDELETEFRAMEBUFFERSPROC			glDeleteFramebuffers	= NULL;
PFNGLBINDFRAMEBUFFEREXTPROC			glBindFramebuffer		= NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC	glFramebufferTexture2D	= NULL;

PFNGLCREATEPROGRAMPROC     glCreateProgram	= NULL;
PFNGLDELETEPROGRAMPROC     glDeleteProgram	= NULL;
PFNGLLINKPROGRAMPROC       glLinkProgram	= NULL;
PFNGLVALIDATEPROGRAMPROC   glValidateProgram= NULL;
PFNGLUSEPROGRAMPROC        glUseProgram		= NULL;
PFNGLGETPROGRAMIVPROC      glGetProgramiv	= NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLCREATESHADERPROC      glCreateShader	= NULL;
PFNGLDELETESHADERPROC      glDeleteShader	= NULL;
PFNGLSHADERSOURCEPROC      glShaderSource	= NULL;
PFNGLCOMPILESHADERPROC     glCompileShader	= NULL;
PFNGLATTACHSHADERPROC      glAttachShader	= NULL;
PFNGLDETACHSHADERPROC      glDetachShader	= NULL;
PFNGLGETSHADERIVPROC       glGetShaderiv	= NULL;
PFNGLGETSHADERINFOLOGPROC  glGetShaderInfoLog = NULL;

PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation		= NULL;
PFNGLBINDATTRIBLOCATIONPROC		  glBindAttribLocation		= NULL;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer		= NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray	= NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray= NULL;

PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLUNIFORMMATRIX4FVPROC   glUniformMatrix4fv	 = NULL;
PFNGLUNIFORM1IPROC			glUniform1i	 = NULL;
#else
#endif

float *matrixOrto(float left, float top, float right, float bottom, float f, float n) {
	float *matrix = (float*)malloc(sizeof(float)*16);
	//0 1 2 3
	//4 5 6 7
	//8 9 10 11
	//12 13 14 15
	/*matrix[0] = 2.0f/w;
	matrix[1] = 0;
	matrix[2] = 0;
	matrix[3] = 0;
	
	matrix[4] = 0;
	matrix[5] = -2.0f/h;
	matrix[6] = 0;
	matrix[7] = 0;
	
	matrix[8] = 0;
	matrix[9] = 0;
	matrix[10] = -2.0f/(f - n);
	matrix[11] = 0;

	matrix[12] = -1;
    matrix[13] = 1;
    matrix[14] = -(f+n) / (f - n);
    matrix[15] = 1;*/
	printf("\t%f\t%f\t%f\t%f\t\n", matrix[0], matrix[1], matrix[2], matrix[3]);
	printf("\t%f\t%f\t%f\t%f\t\n", matrix[4], matrix[5], matrix[6], matrix[7]);
	printf("\t%f\t%f\t%f\t%f\t\n", matrix[8], matrix[9], matrix[10], matrix[11]);
	printf("\t%f\t%f\t%f\t%f\t\n", matrix[12], matrix[13], matrix[14], matrix[15]);
	return matrix;
}

int OpenGL::init(OPENGL_VER ver) {
	OPENGL_GET_PROC(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
	OPENGL_GET_PROC(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
	OPENGL_GET_PROC(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);

	OPENGL_GET_PROC(PFNGLGENBUFFERSPROC, glGenBuffers);
	OPENGL_GET_PROC(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
	OPENGL_GET_PROC(PFNGLBINDBUFFERPROC, glBindBuffer);
	OPENGL_GET_PROC(PFNGLBUFFERDATAPROC, glBufferData);
	OPENGL_GET_PROC(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
	OPENGL_GET_PROC(PFNGLACTIVETEXTUREPROC, glActiveTexture);
	
	OPENGL_GET_PROC(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
	OPENGL_GET_PROC(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers);
	OPENGL_GET_PROC(PFNGLBINDFRAMEBUFFEREXTPROC, glBindFramebuffer);
	OPENGL_GET_PROC(PFNGLFRAMEBUFFERTEXTURE2DEXTPROC, glFramebufferTexture2D);
	
	OPENGL_GET_PROC(PFNGLCREATEPROGRAMPROC, glCreateProgram);
	OPENGL_GET_PROC(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
	OPENGL_GET_PROC(PFNGLLINKPROGRAMPROC, glLinkProgram);
	OPENGL_GET_PROC(PFNGLVALIDATEPROGRAMPROC, glValidateProgram);
	OPENGL_GET_PROC(PFNGLUSEPROGRAMPROC, glUseProgram);
	OPENGL_GET_PROC(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
	OPENGL_GET_PROC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
	OPENGL_GET_PROC(PFNGLCREATESHADERPROC, glCreateShader);
	OPENGL_GET_PROC(PFNGLDELETESHADERPROC, glDeleteShader);
	OPENGL_GET_PROC(PFNGLSHADERSOURCEPROC, glShaderSource);
	OPENGL_GET_PROC(PFNGLCOMPILESHADERPROC, glCompileShader);
	OPENGL_GET_PROC(PFNGLATTACHSHADERPROC, glAttachShader);
	OPENGL_GET_PROC(PFNGLDETACHSHADERPROC, glDetachShader);
	OPENGL_GET_PROC(PFNGLGETSHADERIVPROC, glGetShaderiv);
	OPENGL_GET_PROC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
	
	OPENGL_GET_PROC(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
	OPENGL_GET_PROC(PFNGLBINDATTRIBLOCATIONPROC, glBindAttribLocation);
	OPENGL_GET_PROC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
	OPENGL_GET_PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
	OPENGL_GET_PROC(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
	
	OPENGL_GET_PROC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
	OPENGL_GET_PROC(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
	OPENGL_GET_PROC(PFNGLUNIFORM1IPROC, glUniform1i);
	OpenGL::ver = ver;
	viewport vp;
	vp.x = 0;
	vp.y = 0;
	vp.width = 0;
	vp.height = 0;
	OpenGL::viewportBuffer.push_back(vp);
	viewportMatrix vpm;
	vpm.left = 0;
	vpm.top = 0;
	vpm.right = 0;
	vpm.bottom = 0;
	OpenGL::viewportMatrixBuffer.push_back(vpm);
	if ( ver==VER_COMPTABLE_ALL || ver==VER_CORE_210) {
	}else{
		string str1 = "\
		#version 330 core\n\
		uniform mat4 matrixProjection; \n\
in vec2 position;\n\
in vec2 texCoord;\n\
		out vec2 fragTexCoord;\n\
		void main(void){\n\
			fragTexCoord = texCoord;\n\
			gl_Position   = matrixProjection * vec4(position, -1.0, 1.0);\n\
			//fragTexcoord = vec2(1, 1);\n\
		}", 
		str2 = "#version 330 core\n\
		uniform sampler2D texture;\n\
		in vec2 fragTexCoord;\n\
		out vec4 color;\n\
		void main(void){\n\
				color = texture(texture, fragTexCoord);\n\
		}";
		const char *script1 = str1.c_str(), *script2 = str2.c_str();
		GLint length1 = str1.length(), length2 = str2.length();
		GLShader *glsl = new GLShader();
		glsl->setVertexShader(script1, length1);
		glsl->setFragmentShader(script2, length2);
		glsl->make();
		glUseProgram(glsl->shaderProgram);
		glActiveTexture(GL_TEXTURE0);
		GLShader::matrixProjection = glGetUniformLocation(glsl->shaderProgram, "matrixProjection");
		//GLShader::matrixProjection = glGetUniformLocation(glsl->shaderProgram, "matrixProjection");
		//if (GLShader::matrixProjection != -1)
			//glUniformMatrix4fv(GLShader::matrixProjection, 1, GL_TRUE, matrix);
		GLShader::colorTexture = glGetUniformLocation(glsl->shaderProgram, "texture");
		if (GLShader::colorTexture != -1)
			glUniform1i(GLShader::colorTexture , 0);
		printf("Windows size %i %i\n", Windows::window->width, Windows::window->height);
		printf("ERROR %i\n", glGetError());
	}
}
void OpenGL::pushViewport() {
	viewport vp;
	vp.x = OpenGL::viewportBuffer[OpenGL::viewportBuffer.size()-1].x;
	vp.y = OpenGL::viewportBuffer[OpenGL::viewportBuffer.size()-1].y;
	vp.width = OpenGL::viewportBuffer[OpenGL::viewportBuffer.size()-1].width;
	vp.height = OpenGL::viewportBuffer[OpenGL::viewportBuffer.size()-1].height;
	
	OpenGL::viewportBuffer.push_back(vp);
}
void OpenGL::popViewport() {
	OpenGL::viewportBuffer.pop_back();
	glViewport( OpenGL::viewportBuffer[OpenGL::viewportBuffer.size()-1].x,
			OpenGL::viewportBuffer[OpenGL::viewportBuffer.size()-1].y,
			OpenGL::viewportBuffer[OpenGL::viewportBuffer.size()-1].width,
			OpenGL::viewportBuffer[OpenGL::viewportBuffer.size()-1].height);
}
void OpenGL::setViewport(short x, short y, short width, short height) {
	OpenGL::viewportBuffer[OpenGL::viewportBuffer.size()-1].x = x;
	OpenGL::viewportBuffer[OpenGL::viewportBuffer.size()-1].y = y;
	OpenGL::viewportBuffer[OpenGL::viewportBuffer.size()-1].width = width;
	OpenGL::viewportBuffer[OpenGL::viewportBuffer.size()-1].height = height;
	glViewport(x, y, width, height);
}
void OpenGL::pushViewportMatrix() {
	OpenGL::viewportMatrixBuffer.push_back( OpenGL::viewportMatrixBuffer[OpenGL::viewportMatrixBuffer.size()-1] );
}
void OpenGL::popViewportMatrix() {
	OpenGL::viewportMatrixBuffer.pop_back();
	setViewportMatrix( OpenGL::viewportMatrixBuffer[OpenGL::viewportMatrixBuffer.size()-1].left,
			OpenGL::viewportMatrixBuffer[OpenGL::viewportMatrixBuffer.size()-1].top,
			OpenGL::viewportMatrixBuffer[OpenGL::viewportMatrixBuffer.size()-1].right,
			OpenGL::viewportMatrixBuffer[OpenGL::viewportMatrixBuffer.size()-1].bottom);
}
void OpenGL::setViewportMatrix(short left, short top, short right, short bottom) {
	float *matrix;
	OpenGL::viewportMatrixBuffer[OpenGL::viewportMatrixBuffer.size()-1].left = left;
	OpenGL::viewportMatrixBuffer[OpenGL::viewportMatrixBuffer.size()-1].top = top;
	OpenGL::viewportMatrixBuffer[OpenGL::viewportMatrixBuffer.size()-1].right = right;
	OpenGL::viewportMatrixBuffer[OpenGL::viewportMatrixBuffer.size()-1].bottom = bottom;
	switch (OpenGL::ver) {
		case VER_COMPTABLE_ALL:
			glLoadIdentity( );
			gluOrtho2D( left, right, bottom, top );
			return;
		case VER_CORE_210:
			glLoadIdentity( );
			gluOrtho2D( left, right, bottom, top );
			return;
		case VER_CORE_330:
			matrix = matrixOrto(left, top, right, bottom, 0.1, 5);
			glBindAttribLocation(GLShader::glsl->shaderProgram, GLShader::matrixProjection, "matrixProjection");
			glUniformMatrix4fv(GLShader::matrixProjection, 1, GL_FALSE, matrix);
			return;
		case VER_CORE_400:
			return;
	}
}
void OpenGL::trace() {
	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	fprintf(stdout, "OpenGL render context information:\nRenderer:%s\nVendor\t:%s\nVersion\t:%s\nOpenGL version\t:%d.%d\n", glGetString(GL_RENDERER), glGetString(GL_VENDOR), glGetString(GL_VERSION), major, minor);
}

GLShader *GLShader::glsl = NULL;
GLint GLShader::colorTexture = 0;
GLint GLShader::matrixProjection = 0;
GLShader::GLShader() {
	GLShader::glsl = this;
	this->shaderProgram = glCreateProgram();
	this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
}
int GLShader::setFragmentShader(const char* str, size_t size) {
	glShaderSource(this->fragmentShader, 1, &str, (const GLint*)&size);
	glCompileShader(this->fragmentShader);
	if (glGetError() == 0) {
		this->fragment = true;
		return true;
	}
	return false;
}
int GLShader::setVertexShader(const char* str, size_t size) {
	glShaderSource(this->vertexShader, 1, &str, (const GLint*)&size);
	glCompileShader(this->vertexShader);
	if (glGetError() == 0) {
		this->vertex = true;
		return true;
	}
	return false;
}
int GLShader::make() {
	if (!this->fragment && !this->vertex) return false;
	glAttachShader(this->shaderProgram, this->vertexShader);
	glAttachShader(this->shaderProgram, this->fragmentShader);
	glLinkProgram(this->shaderProgram);
	return true;
}


void Windows::regFirstWin() {
	WNDCLASSEX wcx;
	memset(&wcx, 0, sizeof(WNDCLASSEX));
	wcx.cbSize        = sizeof(WNDCLASSEX);
	wcx.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcx.lpfnWndProc   = reinterpret_cast<WNDPROC>(Windows::WndProc);
	wcx.hInstance     = System::hInstance;
	wcx.lpszClassName = WIN_CLASS_NAME;
	wcx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor       = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClassEx(&wcx)) {
		printf("RegisterClassEx fail (%d)\n", GetLastError());
		return;
	}

}
Windows::Windows(short x, short y, short width, short height) {
	printf("Windows start\n");
	if (Windows::window!=NULL) return;
	Windows::window = this;
	this->visible = false;
	this->renderComplete = false;
	this->root = new ShapeGroupRect();
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	HANDLE mutex = CreateMutex(NULL, TRUE, NULL);
	this->winThread = CreateThread(NULL, 0, windowThread, &mutex, 0, &this->winThreadID);
	WaitForSingleObject(mutex, INFINITE);
	CloseHandle(mutex);
	printf("Windows end\n");
	/*int attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB,	3,
		WGL_CONTEXT_MINOR_VERSION_ARB,	3,
		WGL_CONTEXT_FLAGS_ARB,			WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, 0};
	OPENGL_GET_PROC(PFNWGLCREATECONTEXTATTRIBSARBPROC, wglCreateContextAttribsARB);
	if (!wglCreateContextAttribsARB) {
		fprintf(stderr, "wglCreateContextAttribsARB fail (%d)\n", GetLastError());
		return;
	}
	Windows::hRC = wglCreateContextAttribsARB(Windows::hDC, 0, attributes);
	if (!Windows::hRC|| !wglMakeCurrent(Windows::hDC, Windows::hRC)) {// <editor-fold defaultstate="collapsed" desc="***">
		fprintf(stderr, "Creating render context fail (%d)\n", GetLastError());
		return;
	}// </editor-fold>
	wglDeleteContext(hRCTemp);*/
}
void Windows::show() {
	fprintf(stdout, "Win show\n");
	SetWindowPos(this->hWnd, HWND_TOP, this->x, this->y, this->width+this->x, this->height+this->y, SWP_SHOWWINDOW );
	ShowWindow(this->hWnd, SW_SHOWNORMAL);
	this->visible = true;
	ResumeThread(this->renderThread);
	SetForegroundWindow(this->hWnd);
	SetFocus(this->hWnd);
	UpdateWindow(this->hWnd);
	EventWindow *e = new EventWindow();
	e->window = this;
	e->type = EventWindow::WIN_SHOW;
	this->callEvent(e);
	delete e;
}
void Windows::hide() {
	fprintf(stdout, "Win hide\n");
	ShowWindow(this->hWnd, SW_MINIMIZE);
	SuspendThread(this->renderThread);
	this->visible = false;
	EventWindow *e = new EventWindow();
	e->window = this;
	e->type = EventWindow::WIN_HIDE;
	this->callEvent(e);
	delete e;
}
void Windows::close() {
	CloseHandle(this->renderThread);
	CloseHandle(this->winThread);
	wglDeleteContext(this->hRC);
	PostQuitMessage(0);
	DestroyWindow(this->hWnd);
	Windows::window = NULL;
	printf("Windows close\n");
	delete this;
}
void Windows::setFocus() {
	fprintf(stdout, "Win setFocus\n");
}
void Windows::saveAsXML() {
	/*FILE *file = fopen("root.xml", "wb");
	fprintf(file, "<?xml version='1.0' encoding='utf-8'?>\n<xml>\n\t<Textures>\n");
	for(int i=0; i<this->texture.buffer.size(); i++) {
		this->texture.buffer[i]->saveAsXML(file, 2);
	}
	fprintf(file, "\t</Textures>\n\t<WindowsRootShapes>\n");
	this->root->saveAsXML(file, 1);
	fprintf(file, "\t</WindowsRootShapes>\n");
	fprintf(file, "</xml>\n");
	fclose(file);*/
}
void Windows::redraw() {
	glClearColor( 0.5, 0.5, 0.5, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	switch (OpenGL::ver) {
		case OpenGL::VER_COMPTABLE_ALL:// <editor-fold defaultstate="collapsed" desc="GL_COMPTABLE_ALL">
			glEnable( GL_BLEND );
			glEnable( GL_ALPHA_TEST );
			glEnable( GL_POINT_SMOOTH );
			glEnable( GL_LINE_SMOOTH );
			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			this->root->renderGLComptAll();
			glDisable( GL_LINE_SMOOTH );
			glDisable( GL_POINT_SMOOTH );
			glDisable( GL_BLEND );
			glDisable( GL_ALPHA_TEST );
			glFlush( );
			break;// </editor-fold>
		case OpenGL::VER_CORE_210:// <editor-fold defaultstate="collapsed" desc="VER_CORE_210">
			glEnable( GL_BLEND );
			glEnable( GL_ALPHA_TEST );
			glEnable( GL_POINT_SMOOTH );
			glEnable( GL_LINE_SMOOTH );
			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			this->root->renderGL210();
			glDisable( GL_LINE_SMOOTH );
			glDisable( GL_POINT_SMOOTH );
			glDisable( GL_BLEND );
			glDisable( GL_ALPHA_TEST );
			glFlush( );
			break;// </editor-fold>
		case OpenGL::VER_CORE_330:// <editor-fold defaultstate="collapsed" desc="VER_CORE_330">
			glEnable( GL_BLEND );
			glEnable( GL_ALPHA_TEST );
			glEnable( GL_POINT_SMOOTH );
			glEnable( GL_LINE_SMOOTH );
			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glUseProgram(GLShader::glsl->shaderProgram);
			printf("GLShader::glsl->shaderProgram %i\n", GLShader::glsl->shaderProgram);
			this->root->renderGL330();
			glDisable( GL_LINE_SMOOTH );
			glDisable( GL_POINT_SMOOTH );
			glDisable( GL_BLEND );
			glDisable( GL_ALPHA_TEST );
			glFlush( );
			break;// </editor-fold>
	}
	/*const int vertexCount = 6, vertexSize = 2;
	static const unsigned short triangleMesh[vertexCount*vertexSize] = {
		0, 0,
		0, 300,
		404, 0,
		404, 0,
		404, 300,
		0, 300,
	};
	static const unsigned char texMesh[vertexCount*vertexSize] = {
		0, 0,
		0, 1,
		1, 0,
		1, 0,
		1, 1,
		0, 1
	};
#define WIDTCH 301
#define HEIGHT 401	
	unsigned char bmp[WIDTCH*HEIGHT*3+((WIDTCH*3+3)&~3-WIDTCH)*HEIGHT];
	int offset=((WIDTCH*3+3)&~3)-WIDTCH*3;
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTCH; x++) {
			bmp[ offset*y+(x+WIDTCH*y)*3   ] = 0xFF;
			bmp[ offset*y+(x+WIDTCH*y)*3+1 ] = 0;
			bmp[ offset*y+(x+WIDTCH*y)*3+2 ] = 0;
			
		}
	}
	GLuint meshVAO, meshVBO[2], img;
	
	glGenTextures( 1, &img );
	glBindTexture( GL_TEXTURE_2D, img );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, WIDTCH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp );
	
	glGenVertexArrays(1, &meshVAO);
	glBindVertexArray(meshVAO);
	glGenBuffers(2, meshVBO);

	GLint positionLocation, texcoordLocation;
	const int vertexOffsetPosition = 0;
	const int vertexOffsetColor    = 2;
	positionLocation = glGetAttribLocation(OpenGL::shaderProgram, "position");
	if (positionLocation != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, meshVBO[0]);
		glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize*2, triangleMesh, GL_STATIC_DRAW);
		glVertexAttribPointer(positionLocation, 2, GL_UNSIGNED_SHORT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(positionLocation);
	}else{
		printf("ERROR positionLocation\n");
	}
	texcoordLocation = glGetAttribLocation(OpenGL::shaderProgram, "texcoord");
	if (texcoordLocation != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, meshVBO[1]);
		glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, texMesh, GL_STATIC_DRAW);
		glVertexAttribPointer(texcoordLocation, 2, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(texcoordLocation);
	}else{
		printf("ERROR texcoordLocation\n");
	}
	//w = (float)(Windows::width), h = (float)(Windows::height)
	const float left = 0, right = Windows::width, bottom = Windows::height, top = 0, zNear = 0.001, zFar = 100;
	const float tx = - (right + left) / (right - left),
		ty = - (top + bottom) / (top - bottom),
		tz = - (zFar + zNear) / (zFar - zNear);
	float projectionMatrix[16] = {
		2 / (right - left), 0, 0, tx,
		0, 2 / (top - bottom), 0, ty,
		0, 0, -2 / (zFar - zNear), tz,
		0, 0, 0, 1
	};

	GLint projectionMatrixLocation = glGetUniformLocation(OpenGL::shaderProgram, "projectionMatrix");
	if (projectionMatrixLocation != -1) glUniformMatrix4fv(projectionMatrixLocation, 1, GL_TRUE, projectionMatrix);

	glBindVertexArray(meshVAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
*/
	SwapBuffers(this->hDC);
	this->renderComplete = true;
}
void Windows::redrawFBO () {
	//Directory *dir;
	//GLuint fb;
	//glGenFramebuffers(1, &fb);
	//glBindFramebuffer(GL_FRAMEBUFFER, fb);
	//typeid
	//glGenFramebuffers(1, &Windows::FBOGL);
	//glBindFramebuffer(GL_FRAMEBUFFER, Windows::FBOGL);
	unsigned int countUpdateFBO = 0;
	Buffer *buf;
	for(int i=0; i<this->FBOBuffer.size()-countUpdateFBO; i++) {
		buf = this->FBOBuffer[i];
		if (buf->bufferFrame==0) glGenFramebuffers(1, &(buf->bufferFrame));
		if ( buf->bufferGLComptAll() ) {
			buf->bufferInit = true;
			this->renderComplete = false;
			countUpdateFBO++;
			if (countUpdateFBO >= this->FBOBuffer.size()) {
				this->FBOBuffer.clear();
				break;
			}
			this->FBOBuffer[i] = this->FBOBuffer[ this->FBOBuffer.size()-countUpdateFBO ]; 
			this->FBOBuffer[ this->FBOBuffer.size()-countUpdateFBO ] = NULL;
		}
	}
	if (countUpdateFBO < this->FBOBuffer.size()) {
		this->FBOBuffer.resize( this->FBOBuffer.size()-countUpdateFBO );
	}
	//Windows::FBOBuffer.clear();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDeleteFramebuffers(1, &Windows::FBOGL);
	glViewport(0, 0, this->width, this->height);
	OpenGL::setViewportMatrix(0, 0, this->width, this->height);
}
void Windows::resize(short width, short height) {
	this->width = width;
	this->height = height;
	RECT rect;
	rect.left = this->x;
	rect.top = this->y;
	rect.bottom = this->y+this->height;
	rect.right = this->x+this->width;
	AdjustWindowRectEx (&rect, WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX, FALSE, WS_EX_COMPOSITED|WS_EX_APPWINDOW|WS_EX_TOPMOST);
	GetClientRect(this->hWnd, &rect);
	this->width  = rect.right-rect.left;
	this->height = rect.bottom-rect.top;
	OpenGL::setViewport(0, 0, this->width, this->height);
	OpenGL::setViewportMatrix(0, 0, this->width, this->height);
	EventWindow *e = new EventWindow();
	e->window = this;
	e->type = EventWindow::WIN_SIZE;
	this->callEvent(e);
}
/*
int Windows::addEventHandler(int type, void(*fun)(const EventWindows*)) {
	EventLinc el;
	el.type = type;
	el.fun = (void(*)(const Event*))fun;
	Keyboard::arr.push_back( el );
}
int Windows::callEvent(const EventWindows *event) {
	for(int i=0, s=Keyboard::arr.size(); i<s; i++) {
		if (Keyboard::arr[i].type == event->type) {
			Keyboard::arr[i].fun(event);
		}
	}
}
int Windows::removeEventHandler( void(*fun)(const EventWindows*) ) {
	
}
vector<EventLinc> Windows::arr;*/

LRESULT CALLBACK Windows::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	EventKeyboard *eventKey = new EventKeyboard();
	EventMouse *eventMouse = new EventMouse();
	EventMouseShape *eventMouseShape = new EventMouseShape();
	EventWindow *eventWin = new EventWindow();
	Windows *win = Windows::window;
	if (win->hWnd == hWnd) {
		switch (msg) {
		/*	case WM_KEYDOWN:
				case WM_SETFOCUS:
				case WM_KILLFOCUS:
				case WM_ACTIVATE:
				case WA_CLICKACTIVE:*/
			case WM_KEYDOWN:
				eventKey->type = EventKeyboard::KEY_DOWN;
				eventKey->keyCode = (char)wParam;
				win->events.keyboard.callEvent(eventKey);
				return 0;
			
			case WM_MOUSEMOVE:
				eventMouse->type = EventMouse::MOUSE_MOVE;
				eventMouse->x = GET_X_LPARAM(lParam); 
				eventMouse->y = GET_Y_LPARAM(lParam);
				eventMouse->ctrlKey = (wParam&MK_CONTROL);
				eventMouse->shiftKey = (wParam&MK_SHIFT);
				eventMouseShape->type = EventMouseShape::MOUSE_MOVE;
				eventMouseShape->globalx = GET_X_LPARAM(lParam);
				eventMouseShape->globaly = GET_Y_LPARAM(lParam);
				eventMouseShape->ctrlKey = (wParam&MK_CONTROL);
				eventMouseShape->shiftKey = (wParam&MK_SHIFT);
				win->events.mouse.callEvent(eventMouse);
				win->root->callEvent(eventMouseShape);
				return 0;
			case WM_MOUSEWHEEL://GET_KEYSTATE_WPARAM
				eventMouse->type = EventMouse::MOUSE_WHEEL;
				eventMouse->wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				eventMouse->ctrlKey = (wParam&MK_CONTROL);
				eventMouse->shiftKey = (wParam&MK_SHIFT);
				win->events.mouse.callEvent(eventMouse);
				return 0;
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case 0x020B://WM_XBUTTONDOWN
				eventMouse->type = EventMouse::MOUSE_DOWN;
				eventMouse->ctrlKey = (wParam&MK_CONTROL);
				eventMouse->shiftKey = (wParam&MK_SHIFT);
				eventMouse->leftKey = (wParam&MK_LBUTTON);
				eventMouse->middleKey = (wParam&MK_MBUTTON);
				eventMouse->rightKey = (wParam&MK_RBUTTON);
				eventMouse->firstX = (wParam&0x0020);//MK_XBUTTON1
				eventMouse->secondX = (wParam&0x0040);//MK_XBUTTON2
				eventMouse->x = GET_X_LPARAM(lParam);
				eventMouse->y = GET_Y_LPARAM(lParam);
				eventMouseShape->type = EventMouseShape::MOUSE_DOWN;
				eventMouseShape->ctrlKey = (wParam&MK_CONTROL);
				eventMouseShape->shiftKey = (wParam&MK_SHIFT);
				eventMouseShape->leftKey = (wParam&MK_LBUTTON);
				eventMouseShape->middleKey = (wParam&MK_MBUTTON);
				eventMouseShape->rightKey = (wParam&MK_RBUTTON);
				eventMouseShape->firstX = (wParam&0x0020);
				eventMouseShape->secondX = (wParam&0x0040);
				eventMouseShape->globalx = GET_X_LPARAM(lParam);
				eventMouseShape->globaly = GET_Y_LPARAM(lParam);
				win->events.mouse.callEvent(eventMouse);
				win->root->callEvent(eventMouseShape);
				return 0;
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			case 0x020C://WM_XBUTTONUP
				eventMouse->type = EventMouse::MOUSE_UP;
				eventMouse->ctrlKey = (wParam&MK_CONTROL);
				eventMouse->shiftKey = (wParam&MK_SHIFT);
				eventMouse->leftKey = (wParam&MK_LBUTTON);
				eventMouse->middleKey = (wParam&MK_MBUTTON);
				eventMouse->rightKey = (wParam&MK_RBUTTON);
				eventMouse->firstX = (wParam&0x0020);//MK_XBUTTON1
				eventMouse->secondX = (wParam&0x0040);//MK_XBUTTON2
				eventMouse->x = GET_X_LPARAM(lParam); 
				eventMouse->y = GET_Y_LPARAM(lParam);
				eventMouseShape->type = EventMouseShape::MOUSE_UP;
				eventMouseShape->ctrlKey = (wParam&MK_CONTROL);
				eventMouseShape->shiftKey = (wParam&MK_SHIFT);
				eventMouseShape->leftKey = (wParam&MK_LBUTTON);
				eventMouseShape->middleKey = (wParam&MK_MBUTTON);
				eventMouseShape->rightKey = (wParam&MK_RBUTTON);
				eventMouseShape->firstX = (wParam&0x0020);
				eventMouseShape->secondX = (wParam&0x0040);
				eventMouseShape->globalx = GET_X_LPARAM(lParam);
				eventMouseShape->globaly = GET_Y_LPARAM(lParam);
				win->events.mouse.callEvent(eventMouse);
				win->root->callEvent(eventMouseShape);
				return 0;
			case WM_PAINT:
				if (win->visible) win->renderComplete = false;
				return 0;
			/*case WM_CLOSE:
				fprintf(stdout, "WndProc CLOSE\n");
				PostQuitMessage(0);
				return 0;
			case WM_DESTROY:
				fprintf(stdout, "WndProc WM_DESTROY\n");
				PostQuitMessage(0);
				return 0;*/
			case WM_MOVE:
				win->x = (short) LOWORD(lParam);
				win->y = (short) HIWORD(lParam); 
				eventWin->window = win;
				eventWin->type = EventWindow::WIN_MOVE;
				win->callEvent(eventWin);
				return 0;
			case WM_SIZING:
				win->x = (short) ((RECT*)lParam)->left;
				win->y = (short) ((RECT*)lParam)->top; 
				win->width = (unsigned short)((RECT*)lParam)->right - ((RECT*)lParam)->left;
				win->height = (unsigned short)((RECT*)lParam)->bottom - ((RECT*)lParam)->top; 
				eventWin->window = win;
				eventWin->type = EventWindow::WIN_SIZE;
				win->callEvent(eventWin);
				//return TRUE;
			case WM_SYSCOMMAND:
				fprintf(stdout, "\tWM_SYSCOMMAND %i\n", wParam&0xFFF0);
				switch (wParam & 0xFFF0) {
					case SC_CLOSE :
						win->close();
						return 0;
					case SC_SIZE :
						win->resize(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
						return 0;
					case SC_MINIMIZE:
						win->hide();
						return 0;
					case SC_RESTORE:
						win->show();
						return 0;
				}
			case WM_GETICON:
				switch (wParam) {
					case ICON_BIG:
					case ICON_SMALL:
					//case ICON_SMALL2:
						return DefWindowProc(hWnd, msg, wParam, lParam);
				}
			case WM_CREATE:
				break;
		}
	}
	//printf("WndProc msg=%i\n", msg);
	//fprintf(stdout, "WndProc msg=%i window.size=%i\n", msg, Windows::window.size());
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
