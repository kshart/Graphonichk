/* 
 * File:   grOpenGL.cpp
 * Author: Артем
 * 
 * Created on 3 Март 2014 г., 21:47
 */

#include "grOpenGL.h"

using namespace Graphonichk;

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

OpenGL::OPENGL_VER OpenGL::ver;
vector<OpenGL::viewport> OpenGL::viewportBuffer;
vector<ViewMatrix> OpenGL::viewMatrixBuffer;
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
	ViewMatrix vpm;
	OpenGL::viewMatrixBuffer.push_back(vpm);
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
void OpenGL::clearViewMatrix() {
	OpenGL::viewMatrixBuffer.resize(1);
	OpenGL::setViewMatrix(OpenGL::viewMatrixBuffer[0]);
}
void OpenGL::pushViewMatrix() {
	OpenGL::viewMatrixBuffer.push_back( OpenGL::viewMatrixBuffer[OpenGL::viewMatrixBuffer.size()-1] );
}
void OpenGL::popViewMatrix() {
	OpenGL::viewMatrixBuffer.pop_back();
	setViewMatrix( OpenGL::viewMatrixBuffer[OpenGL::viewMatrixBuffer.size()-1]);
}
void OpenGL::setViewMatrix(ViewMatrix matrix) {
	//short left, short top, short right, short bottom
	OpenGL::viewMatrixBuffer[OpenGL::viewMatrixBuffer.size()-1] = matrix;
	switch (OpenGL::ver) {
		case VER_COMPTABLE_ALL:
			glLoadMatrixf(matrix.a);
			return;
		case VER_CORE_210:
			glLoadMatrixf(matrix.a);
			return;
		case VER_CORE_330:
			glBindAttribLocation(GLShader::glsl->shaderProgram, GLShader::matrixProjection, "matrixProjection");
			glUniformMatrix4fv(GLShader::matrixProjection, 1, GL_FALSE, matrix.a);
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
