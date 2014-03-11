/* 
 * File:   grOpenGL.cpp
 * Author: Артем
 * 
 * Created on 3 Март 2014 г., 21:47
 */
#include "grBaseTypes.h"
#include "grOpenGL.h"

using namespace Graphonichk;


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
GLuint OpenGL::viewMatrix = 0;
OpenGL::OPENGL_VER OpenGL::ver;
vector<OpenGL::viewport> OpenGL::viewportBuffer;
vector<ViewMatrix> OpenGL::viewMatrixBuffer;
int OpenGL::init(OPENGL_VER ver) {
	glewInit();
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
	}else if (ver==VER_CORE_330) {
		glGenBuffers(1, &OpenGL::viewMatrix);
		glBindBuffer(GL_UNIFORM_BUFFER, OpenGL::viewMatrix);
		glBufferData(GL_UNIFORM_BUFFER, 4*4*sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		
		ShaderBitmap::init33();
		ShaderSVGmain::init33();
		
		GLShader::setShader(ShaderBitmap::prog);
		glActiveTexture(GL_TEXTURE0);
		//GLShader::matrixProjection = glGetUniformLocation(glsl->shaderProgram, "matrixProjection");
		//GLShader::matrixProjection = glGetUniformLocation(glsl->shaderProgram, "matrixProjection");
		//if (GLShader::matrixProjection != -1)
			//glUniformMatrix4fv(GLShader::matrixProjection, 1, GL_TRUE, matrix);
		//GLShader::colorTexture = glGetUniformLocation(glsl->shaderProgram, "texture");
		//if (GLShader::colorTexture != -1)
			//glUniform1i(GLShader::colorTexture , 0);
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
	float transposeMatrix[16];
	switch (OpenGL::ver) {
		case VER_COMPTABLE_ALL:
			transposeMatrix[0] = matrix.a[0];
			transposeMatrix[1] = matrix.a[4];
			transposeMatrix[2] = matrix.a[8];
			transposeMatrix[3] = matrix.a[12];
			
			transposeMatrix[4] = matrix.a[1];
			transposeMatrix[5] = matrix.a[5];
			transposeMatrix[6] = matrix.a[9];
			transposeMatrix[7] = matrix.a[13];
			
			transposeMatrix[8] = matrix.a[2];
			transposeMatrix[9] = matrix.a[6];
			transposeMatrix[10] = matrix.a[10];
			transposeMatrix[11] = matrix.a[14];
			
			transposeMatrix[12] = matrix.a[3];
			transposeMatrix[13] = matrix.a[7];
			transposeMatrix[14] = matrix.a[11];
			transposeMatrix[15] = matrix.a[15];
			
			glLoadMatrixf(transposeMatrix);
			return;
		case VER_CORE_210:
			transposeMatrix[0] = matrix.a[0];
			transposeMatrix[1] = matrix.a[4];
			transposeMatrix[2] = matrix.a[8];
			transposeMatrix[3] = matrix.a[12];
			
			transposeMatrix[4] = matrix.a[1];
			transposeMatrix[5] = matrix.a[5];
			transposeMatrix[6] = matrix.a[9];
			transposeMatrix[7] = matrix.a[13];
			
			transposeMatrix[8] = matrix.a[2];
			transposeMatrix[9] = matrix.a[6];
			transposeMatrix[10] = matrix.a[10];
			transposeMatrix[11] = matrix.a[14];
			
			transposeMatrix[12] = matrix.a[3];
			transposeMatrix[13] = matrix.a[7];
			transposeMatrix[14] = matrix.a[11];
			transposeMatrix[15] = matrix.a[15];
			
			glLoadMatrixf(transposeMatrix);
			return;
		case VER_CORE_330:
			glBindBuffer(GL_UNIFORM_BUFFER, OpenGL::viewMatrix);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, 4*4*sizeof(float), matrix.a);
			//glBindAttribLocation(GLShader::glsl->shaderProgram, GLShader::matrixProjection, "matrixProjection");
			//glUniformMatrix4fv(GLShader::matrixProjection, 1, GL_FALSE, matrix.a);
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

GLShader *GLShader::shader = NULL;
GLShader::GLShader(int c) :shaderProgram(0), vertexShader(0), fragmentShader(0), crc32(c) {
	
}
void GLShader::setShader(GLShader* shader) {
	glUseProgram(shader->shaderProgram);
	GLShader::shader = shader;
}
/*GLShader::GLShader() {
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
}*/

ShaderBitmap* ShaderBitmap::prog = NULL;
ShaderBitmap::ShaderBitmap() :GLShader(1) {
	
}
void ShaderBitmap::init() {
	ShaderBitmap *shi = new ShaderBitmap();
	const GLchar *vrsh = 
		"#version 120"
		"uniform mat4 viewMatrix;"
		"in vec2 texCoord;"
		"in vec2 position;"
		"out vec2 fragTexCoord;"
		"void main () {"
			"fragTexCoord = texCoord;"
			"gl_Position = vec4(position, 0.0, 1.0)*viewMatrix;"
		"}",
		*frsh = 
		"#version 120"
		"uniform sampler2D colorTexture;"
		"in vec2 fragTexCoord;"
		"void main () {"
			"gl_FragColor = vec4(1, 0, 0, 1);"//texture(colorTexture, fragTexCoord);"
		"}";
	size_t frshLength = strlen(frsh), vrshLength = strlen(vrsh);
	GLint success;
	shi->shaderProgram = glCreateProgram();
	shi->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	shi->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(shi->vertexShader, 1, &vrsh, (const GLint*)&vrshLength);
	glCompileShader(shi->vertexShader);
	glGetShaderiv(shi->vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shi->vertexShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shi->vertexShader, InfoLog);
	}
	
	glShaderSource(shi->fragmentShader, 1, &frsh, (const GLint*)&frshLength);
	glCompileShader(shi->fragmentShader);
	glGetShaderiv(shi->fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shi->fragmentShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shi->fragmentShader, InfoLog);
	}
	
	glAttachShader(shi->shaderProgram, shi->vertexShader);
	glAttachShader(shi->shaderProgram, shi->fragmentShader);
	glLinkProgram(shi->shaderProgram);
	shi->position = glGetAttribLocation(shi->shaderProgram, "position");
	shi->texCoord = glGetAttribLocation(shi->shaderProgram, "texCoord");
	shi->texture = glGetUniformLocation(shi->shaderProgram, "colorTexture");
	shi->viewMatrix = glGetUniformLocation(shi->shaderProgram, "viewMatrix");
	glUniform1i(shi->texture , 0);
	//shi->viewMatrix = glGetUniformLocation(shi->shaderProgram, "viewMatrix");
	printf("glGetAttribLocation %i %i\n", shi->position, shi->texCoord);
	ShaderBitmap::prog = shi;
}
void ShaderBitmap::init33() {
	ShaderBitmap *shi = new ShaderBitmap();
	const GLchar *vrsh = 
		//"#version 330 core\n"
		"layout(shared) uniform viewMatrix {"
			"mat4 viewMatrixValue;"
		"};"
		"in vec2 texCoord;"
		"in vec2 position;"
		"out vec2 fragTexCoord;"
		"void main () {"
			"fragTexCoord = texCoord;"
			"gl_Position = vec4(position, 0.0, 1.0)*viewMatrixValue;"
		"}",
		*frsh = 
		//"#version 330 core\n"
		"uniform sampler2D colorTexture;"
		"in vec2 fragTexCoord;"
		"out vec4 color;"
		"void main () {"
			"color = texture(colorTexture, fragTexCoord);"
		"}";
	size_t frshLength = strlen(frsh), vrshLength = strlen(vrsh);
	GLint success;
	shi->shaderProgram = glCreateProgram();
	shi->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	shi->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	// one of: GL_POINTS, GL_LINES, GL_LINES_ADJACENCY_EXT, GL_TRIANGLES, GL_TRIANGLES_ADJACENCY_EXT
	glProgramParameteri(shi->shaderProgram, GL_GEOMETRY_INPUT_TYPE, GL_TRIANGLES);
	// one of: GL_POINTS, GL_LINE_STRIP, GL_TRIANGLE_STRIP
	glProgramParameteri(shi->shaderProgram, GL_GEOMETRY_OUTPUT_TYPE, GL_TRIANGLE_STRIP);
	
	glShaderSource(shi->vertexShader, 1, &vrsh, (const GLint*)&vrshLength);
	glCompileShader(shi->vertexShader);
	glGetShaderiv(shi->vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shi->vertexShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shi->vertexShader, InfoLog);
	}

	glShaderSource(shi->fragmentShader, 1, &frsh, (const GLint*)&frshLength);
	glCompileShader(shi->fragmentShader);
	glGetShaderiv(shi->fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shi->fragmentShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shi->fragmentShader, InfoLog);
	}

	glAttachShader(shi->shaderProgram, shi->vertexShader);
	glAttachShader(shi->shaderProgram, shi->fragmentShader);
	glLinkProgram(shi->shaderProgram);
	
	shi->position = glGetAttribLocation(shi->shaderProgram, "position");
	shi->texCoord = glGetAttribLocation(shi->shaderProgram, "texCoord");
	shi->texture = glGetUniformLocation(shi->shaderProgram, "colorTexture");//glGetUniformLocation
	shi->viewMatrix = glGetUniformBlockIndex(shi->shaderProgram, "viewMatrix");//
	glUniform1i(shi->texture , 0);
	//glUniformBlockBinding(p, blockIndex, bindingPoint);
	glUniformBlockBinding(shi->shaderProgram, shi->viewMatrix, 1);// glBindBufferBase
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, OpenGL::viewMatrix, 0, 4*4*sizeof(float));// glBindBufferBase
	
	printf("glGetAttribLocation %i %i %i %i\n", shi->position, shi->texCoord, shi->texture, shi->viewMatrix );
	ShaderBitmap::prog = shi;




}

ShaderSVGmain* ShaderSVGmain::prog = NULL;
ShaderSVGmain::ShaderSVGmain() :GLShader(1) {
	
}
void ShaderSVGmain::init() {
	ShaderSVGmain *shi = new ShaderSVGmain();
	const GLchar *vrsh = 
		"#version 120"
		"uniform mat4 viewMatrix;"
		"in vec2 texCoord;"
		"in vec2 position;"
		"out vec2 fragTexCoord;"
		"void main () {"
			"fragTexCoord = texCoord;"
			"gl_Position = vec4(position, 0.0, 1.0)*viewMatrix;"
		"}",
		*frsh = 
		"#version 120"
		"uniform sampler2D colorTexture;"
		"in vec2 fragTexCoord;"
		"void main () {"
			"gl_FragColor = vec4(1, 0, 0, 1);"//texture(colorTexture, fragTexCoord);"
		"}";
	size_t frshLength = strlen(frsh), vrshLength = strlen(vrsh);
	GLint success;
	shi->shaderProgram = glCreateProgram();
	shi->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	shi->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(shi->vertexShader, 1, &vrsh, (const GLint*)&vrshLength);
	glCompileShader(shi->vertexShader);
	glGetShaderiv(shi->vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shi->vertexShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shi->vertexShader, InfoLog);
	}
	
	glShaderSource(shi->fragmentShader, 1, &frsh, (const GLint*)&frshLength);
	glCompileShader(shi->fragmentShader);
	glGetShaderiv(shi->fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shi->fragmentShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shi->fragmentShader, InfoLog);
	}
	
	glAttachShader(shi->shaderProgram, shi->vertexShader);
	glAttachShader(shi->shaderProgram, shi->fragmentShader);
	glLinkProgram(shi->shaderProgram);
	shi->position = glGetAttribLocation(shi->shaderProgram, "position");
	shi->viewMatrix = glGetUniformLocation(shi->shaderProgram, "viewMatrix");
	//shi->viewMatrix = glGetUniformLocation(shi->shaderProgram, "viewMatrix");
	printf("glGetAttribLocation %i\n", shi->position);
	ShaderSVGmain::prog = shi;
}
void ShaderSVGmain::init33() {
	ShaderSVGmain *shi = new ShaderSVGmain();
	const GLchar *vrsh = 
		//"#version 330 core\n"
		"layout(shared) uniform viewMatrix {"
			"mat4 viewMatrixValue;"
		"};"
		"in vec2 position;"
		"out vec2 fragTexCoord;"
		"void main () {"
			"gl_Position = vec4(position, 0.0, 1.0)*viewMatrixValue;"
		"}",
		*frsh = 
		//"#version 330 core\n"
		"out vec4 color;"
		"void main () {"
			"color = vec4(1, 0, 0, 1);"
		"}";
	size_t frshLength = strlen(frsh), vrshLength = strlen(vrsh);
	GLint success;
	shi->shaderProgram = glCreateProgram();
	shi->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	shi->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	// one of: GL_POINTS, GL_LINES, GL_LINES_ADJACENCY_EXT, GL_TRIANGLES, GL_TRIANGLES_ADJACENCY_EXT
	glProgramParameteri(shi->shaderProgram, GL_GEOMETRY_INPUT_TYPE, GL_TRIANGLES);
	// one of: GL_POINTS, GL_LINE_STRIP, GL_TRIANGLE_STRIP
	glProgramParameteri(shi->shaderProgram, GL_GEOMETRY_OUTPUT_TYPE, GL_TRIANGLE_STRIP);
	
	glShaderSource(shi->vertexShader, 1, &vrsh, (const GLint*)&vrshLength);
	glCompileShader(shi->vertexShader);
	glGetShaderiv(shi->vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shi->vertexShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shi->vertexShader, InfoLog);
	}

	glShaderSource(shi->fragmentShader, 1, &frsh, (const GLint*)&frshLength);
	glCompileShader(shi->fragmentShader);
	glGetShaderiv(shi->fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shi->fragmentShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shi->fragmentShader, InfoLog);
	}

	glAttachShader(shi->shaderProgram, shi->vertexShader);
	glAttachShader(shi->shaderProgram, shi->fragmentShader);
	glLinkProgram(shi->shaderProgram);
	
	shi->position = glGetAttribLocation(shi->shaderProgram, "position");
	shi->viewMatrix = glGetUniformBlockIndex(shi->shaderProgram, "viewMatrix");//
	//glUniformBlockBinding(p, blockIndex, bindingPoint);
	glUniformBlockBinding(shi->shaderProgram, shi->viewMatrix, 1);// glBindBufferBase
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, OpenGL::viewMatrix, 0, 4*4*sizeof(float));// glBindBufferBase
	
	printf("glGetAttribLocation %i %i\n", shi->position, shi->viewMatrix );
	ShaderSVGmain::prog = shi;
}