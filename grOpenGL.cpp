#include "grMain.h"
#include "grOpenGL.h"

using namespace Graphonichk;
/*
struct struct_glslStr {
	GLShader *sh;
	GLchar *vs, *gs, *fs;
	struct_glslStr() :vs(0), gs(0), fs(0), sh(0) {};
};
typedef struct struct_glslStr glslStr;
void glslLoaded(const EventFileLoad* e) {
	glslStr *sh = (glslStr*)(e->obj);
	size_t nameLength = e->file->path.length();
	if (e->file->path[nameLength-2]=='v' && e->file->path[nameLength-1]=='s') {
		sh->vs = (GLchar*)e->file->data;
	}else if (e->file->path[nameLength-2]=='g' && e->file->path[nameLength-1]=='s') {
		sh->gs = (GLchar*)e->file->data;
	}else if (e->file->path[nameLength-2]=='f' && e->file->path[nameLength-1]=='s') {
		sh->fs = (GLchar*)e->file->data;
	}
	if (sh->vs!=nullptr && sh->gs!=nullptr && sh->fs!=nullptr) {
		GLShaderLoadTask *task = new GLShaderLoadTask(sh->sh, sh->vs, sh->fs, sh->gs);
		Windows::window->eachFrame.addTask(task, 0);
		delete sh;
	}
	//img->load( (char*)(e->file->data), e->file->size);
}*/

GLuint OpenGL::grShaderData = 0;
GLuint OpenGL::circleBuffer = 0;
GLuint OpenGL::textureGLID = 0;
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
	OpenGL::viewMatrixBuffer.push_back(ViewMatrixIdentity());
	
	float circle[8192+100];
	circle[0] = 0;
	circle[1] = 0;
	float a;
	for(int i=2; i<8292; i+=2) {
		a = M_PI*2 * i/8192.0;
		circle[i] = cos(a);
		circle[i+1] = sin(a);
	}
		
	glGenBuffers(1, &OpenGL::circleBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, OpenGL::circleBuffer);
	glBufferData(GL_ARRAY_BUFFER, (8192+100)*sizeof(float), circle, GL_STATIC_DRAW);
	if ( ver==VER_CORE_100 || ver==VER_CORE_210) {
		
	}else if (ver==VER_CORE_330) {
		glGenBuffers(1, &OpenGL::grShaderData);
		glBindBuffer(GL_UNIFORM_BUFFER, OpenGL::grShaderData);
		glBufferData(GL_UNIFORM_BUFFER, 4*4*sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		
		/*ShaderBitmap::prog = new ShaderBitmap();
		ShaderSVGmain::prog = new ShaderSVGmain();
		ShaderFPrimitiv::prog = new ShaderFPrimitiv();
		ShaderF3D::prog = new ShaderF3D();
		glslStr *BitmapChars = new glslStr();
		FileLoad *vsBitmapChar = new FileLoad("glsl/ShaderBitmap.vs"),
				*gsBitmapChar = new FileLoad("glsl/ShaderBitmap.gs"),
				*fsBitmapChar = new FileLoad("glsl/ShaderBitmap.fs");
		vsBitmapChar->addEventHandler(EventFileLoad::FILE_SUCCESS, glslLoaded, ShaderBitmap::prog);*/
		ShaderBitmap::init33();
		ShaderSVGmain::init33();
		ShaderFPrimitiv::init33();
		ShaderF3D::init33();
		ShaderTextField::init33();
		ShaderTextFieldBuffer::init33();
		
		GLShader::setShader(ShaderBitmap::prog);
		glActiveTexture(GL_TEXTURE0);
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
		case VER_CORE_100:
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
			glBindBuffer(GL_UNIFORM_BUFFER, OpenGL::grShaderData);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, 4*4*sizeof(float), matrix.a);
			//glBindAttribLocation(GLShader::glsl->shaderProgram, GLShader::matrixProjection, "matrixProjection");
			//glUniformMatrix4fv(GLShader::matrixProjection, 1, GL_FALSE, matrix.a);
			return;
		case VER_CORE_400:
			return;
	}
}
void OpenGL::multViewMatrix(ViewMatrix mulmatrix) {
	ViewMatrix *matrix = &OpenGL::viewMatrixBuffer[OpenGL::viewMatrixBuffer.size()-1];
	float copyM1[16];
	memcpy(copyM1, matrix->a, 16*sizeof(float));

	matrix->a[0] = copyM1[0]*mulmatrix.a[0] +copyM1[1]*mulmatrix.a[4] +copyM1[2]*mulmatrix.a[8] +copyM1[3]*mulmatrix.a[12];
	matrix->a[1] = copyM1[0]*mulmatrix.a[1] +copyM1[1]*mulmatrix.a[5] +copyM1[2]*mulmatrix.a[9] +copyM1[3]*mulmatrix.a[13];
	matrix->a[2] = copyM1[0]*mulmatrix.a[2] +copyM1[1]*mulmatrix.a[6] +copyM1[2]*mulmatrix.a[10] +copyM1[3]*mulmatrix.a[14];
	matrix->a[3] = copyM1[0]*mulmatrix.a[3] +copyM1[1]*mulmatrix.a[7] +copyM1[2]*mulmatrix.a[11] +copyM1[3]*mulmatrix.a[15];
	matrix->a[4] = copyM1[4]*mulmatrix.a[0] +copyM1[5]*mulmatrix.a[4] +copyM1[6]*mulmatrix.a[8] +copyM1[7]*mulmatrix.a[12];
	matrix->a[5] = copyM1[4]*mulmatrix.a[1] +copyM1[5]*mulmatrix.a[5] +copyM1[6]*mulmatrix.a[9] +copyM1[7]*mulmatrix.a[13];
	matrix->a[6] = copyM1[4]*mulmatrix.a[2] +copyM1[5]*mulmatrix.a[6] +copyM1[6]*mulmatrix.a[10] +copyM1[7]*mulmatrix.a[14];
	matrix->a[7] = copyM1[4]*mulmatrix.a[3] +copyM1[5]*mulmatrix.a[7] +copyM1[6]*mulmatrix.a[11] +copyM1[7]*mulmatrix.a[15];
	matrix->a[8] = copyM1[8]*mulmatrix.a[0] +copyM1[9]*mulmatrix.a[4] +copyM1[10]*mulmatrix.a[8] +copyM1[11]*mulmatrix.a[12];
	matrix->a[9] = copyM1[8]*mulmatrix.a[1] +copyM1[9]*mulmatrix.a[5] +copyM1[10]*mulmatrix.a[9] +copyM1[11]*mulmatrix.a[13];
	matrix->a[10] = copyM1[8]*mulmatrix.a[2] +copyM1[9]*mulmatrix.a[6] +copyM1[10]*mulmatrix.a[10] +copyM1[11]*mulmatrix.a[14];
	matrix->a[11] = copyM1[8]*mulmatrix.a[3] +copyM1[9]*mulmatrix.a[7] +copyM1[10]*mulmatrix.a[11] +copyM1[11]*mulmatrix.a[15];
	matrix->a[12] = copyM1[12]*mulmatrix.a[0] +copyM1[13]*mulmatrix.a[4] +copyM1[14]*mulmatrix.a[8] +copyM1[15]*mulmatrix.a[12];
	matrix->a[13] = copyM1[12]*mulmatrix.a[1] +copyM1[13]*mulmatrix.a[5] +copyM1[14]*mulmatrix.a[9] +copyM1[15]*mulmatrix.a[13];
	matrix->a[14] = copyM1[12]*mulmatrix.a[2] +copyM1[13]*mulmatrix.a[6] +copyM1[14]*mulmatrix.a[10] +copyM1[15]*mulmatrix.a[14];
	matrix->a[15] = copyM1[12]*mulmatrix.a[3] +copyM1[13]*mulmatrix.a[7] +copyM1[14]*mulmatrix.a[11] +copyM1[15]*mulmatrix.a[15];
	
	OpenGL::setViewMatrix(matrix[0]);
}
void OpenGL::trace() {
	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	fprintf(stdout, "OpenGL render context information:\nRenderer:%s\nVendor\t:%s\nVersion\t:%s\nOpenGL version\t:%d.%d\n", glGetString(GL_RENDERER), glGetString(GL_VENDOR), glGetString(GL_VERSION), major, minor);
}



GLShader *GLShader::shader = nullptr;
GLShader::GLShader(int c) :shaderProgram(0), vertexShader(0), fragmentShader(0), crc32(c) {
}
void GLShader::init() {
	fputs("GLShader::init\n", iovir);
}
void GLShader::mapping() {
	fputs("GLShader::mapping\n", iovir);
}
void GLShader::setShader(GLShader* shader) {
	glUseProgram(shader->shaderProgram);
	GLShader::shader = shader;
}

ShaderShRect::ShaderShRect(int crc32) :GLShader(crc32) {
}
void ShaderShRect::mapping() {
	glBindAttribLocation(this->shaderProgram, 0, "position");
}
void ShaderShRect::init() {
	this->posRect = glGetAttribLocation(this->shaderProgram, "position");
	this->grShaderData = glGetUniformBlockIndex(this->shaderProgram, "grShaderData");
	glUniformBlockBinding(this->shaderProgram, this->grShaderData, 1);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, OpenGL::grShaderData, 0, 4*4*sizeof(float));
}

ShaderBitmap* ShaderBitmap::prog = nullptr;
ShaderBitmap::ShaderBitmap() :ShaderShRect(ShaderBitmap::CRC32) {
	
}
void ShaderBitmap::init() {
	this->ShaderShRect::init();
	this->texture = glGetUniformLocation(this->shaderProgram, "colorTexture");
	glUniform1i(this->texture, 0);
}
void ShaderBitmap::init33() {
	ShaderBitmap *sh = new ShaderBitmap();
	const GLchar *vrsh = 
		//"#version 330 core\n"
		"in vec4 position;"
		"void main () {"
			"gl_Position = position;"
		"}",
		*frsh = 
		//"#version 330 core\n"
		"uniform sampler2D colorTexture;"
		"in vec2 TexCoord;"
		"out vec4 color;"
		"void main () {"
			"color = texture(colorTexture, TexCoord);"
		"}",
		*gmsh = 
		//"#version 330 core\n"
		"layout (points) in;"
		"layout (triangle_strip) out;"
		"layout (max_vertices = 4) out;"
		"layout(shared) uniform grShaderData {"
			"mat4 viewMatrixValue;"
		"};"
		"uniform sampler2D colorTexture;"
		"out vec2 TexCoord;"
		"void main () {"
			"ivec2 texSize = textureSize(colorTexture, 0);"
			"vec2 Pos = gl_in[0].gl_Position.xy;"
			"gl_Position = vec4(Pos.x, Pos.y, 0.0, 1.0)*viewMatrixValue;"
			"TexCoord = vec2(0.0, 0.0);"
			"EmitVertex();"
			
			"gl_Position = vec4(Pos.x, Pos.y+texSize.y, 0.0, 1.0)*viewMatrixValue;"
			"TexCoord = vec2(0.0, 1.0);"
			"EmitVertex();"
			
			"gl_Position = vec4(Pos.x+texSize.x, Pos.y, 0.0, 1.0)*viewMatrixValue;"
			"TexCoord = vec2(1.0, 0.0);"
			"EmitVertex();"
			
			"gl_Position = vec4(Pos.x+texSize.x, Pos.y+texSize.y, 0.0, 1.0)*viewMatrixValue;"
			"TexCoord = vec2(1.0, 1.0);"
			//"gl_PointSize = 10.0;"
			"EmitVertex();"
			"EndPrimitive();"
		"}";
	/*size_t frshLength = strlen(frsh), vrshLength = strlen(vrsh), gmshLength = strlen(gmsh);
	GLint success;
	shi->shaderProgram = glCreateProgram();
	shi->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	shi->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shi->geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	
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
	
	glShaderSource(shi->geometryShader, 1, &gmsh, (const GLint*)&gmshLength);
	glCompileShader(shi->geometryShader);
	glGetShaderiv(shi->geometryShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shi->geometryShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shi->geometryShader, InfoLog);
	}
	
	glAttachShader(shi->shaderProgram, shi->vertexShader);
	glAttachShader(shi->shaderProgram, shi->fragmentShader);
	glAttachShader(shi->shaderProgram, shi->geometryShader);
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
	*/
	ShaderBitmap::prog = sh;
	GLShaderLoadTask *task = new GLShaderLoadTask(sh, vrsh, frsh, gmsh);
	Windows::window->eachFrame.addTask(task, 0);
}

ShaderSVGmain* ShaderSVGmain::prog = nullptr;
ShaderSVGmain::ShaderSVGmain() :GLShader(ShaderSVGmain::CRC32) {
	
}
void ShaderSVGmain::init() {
	this->position = glGetAttribLocation(this->shaderProgram, "position");
	this->grShaderData = glGetUniformBlockIndex(this->shaderProgram, "grShaderData");
	this->fillColor = glGetUniformLocation(this->shaderProgram, "fillColor");
	this->typeShape = glGetUniformLocation(this->shaderProgram, "typeShape");
	this->circleTransform = glGetUniformLocation(this->shaderProgram, "circleTransform");
	this->transformMatrix = glGetUniformLocation(this->shaderProgram, "transformMatrix");
	glUniformBlockBinding(this->shaderProgram, this->grShaderData, 1);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, OpenGL::grShaderData, 0, 4*4*sizeof(float));
}
void ShaderSVGmain::init33() {
	ShaderSVGmain *sh = new ShaderSVGmain();
	const GLchar *vrsh = 
		//"#version 330 core\n"
		"layout(shared) uniform grShaderData {"
			"mat4 viewMatrixValue;"
		"};"
		"uniform int typeShape;"
		"uniform mat3x2 transformMatrix;"
		
		"uniform vec4 fillColor;"
		"uniform vec4 circleTransform;"
		//x = x position
		//y = y position
		//z = x scale
		//w = y scale
		"in vec2 position;"
		"out vec2 fragTexCoord;"
		"void main () {"
			"switch (typeShape) {"
				"case 0xD4B76579:"
					"gl_Position = vec4(position.x*circleTransform.z+circleTransform.x, position.y*circleTransform.w+circleTransform.y, 0.0, 1.0)*viewMatrixValue;"
					""
					"break;"
				"case 0xB7D63381:"
				"default:"
					"gl_Position = vec4(position, 0.0, 1.0)*viewMatrixValue;"
			"}"
		"}",
		*frsh = 
		//"#version 330 core\n"
		"uniform vec4 fillColor;"
		"out vec4 color;"
		"void main () {"
			"color = fillColor;"
		"}";
	/*size_t frshLength = strlen(frsh), vrshLength = strlen(vrsh);
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
	shi->viewMatrix = glGetUniformBlockIndex(shi->shaderProgram, "viewMatrix");//glGetUniformLocation
	shi->fillColor = glGetUniformLocation(shi->shaderProgram, "fillColor");
	shi->typeShape = glGetUniformLocation(shi->shaderProgram, "typeShape");
	shi->circleTransform = glGetUniformLocation(shi->shaderProgram, "circleTransform");
	shi->transformMatrix = glGetUniformLocation(shi->shaderProgram, "transformMatrix");
	//glUniformBlockBinding(p, blockIndex, bindingPoint);
	glUniformBlockBinding(shi->shaderProgram, shi->viewMatrix, 1);// glBindBufferBase
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, OpenGL::viewMatrix, 0, 4*4*sizeof(float));// glBindBufferBase
	
	printf("glGetAttribLocation %i %i\n", shi->position, shi->viewMatrix );*/
	ShaderSVGmain::prog = sh;
	GLShaderLoadTask *task = new GLShaderLoadTask(sh, vrsh, frsh);
	Windows::window->eachFrame.addTask(task, 0);
}

ShaderFPrimitiv* ShaderFPrimitiv::prog = nullptr;
ShaderFPrimitiv::ShaderFPrimitiv() :GLShader(ShaderFPrimitiv::CRC32) {
	
}
void ShaderFPrimitiv::init() {
	this->position = glGetAttribLocation(this->shaderProgram, "position");
	this->fillColor = glGetUniformLocation(this->shaderProgram, "fillColor");
	this->grShaderData = glGetUniformBlockIndex(this->shaderProgram, "grShaderData");
	glUniformBlockBinding(this->shaderProgram, this->grShaderData, 1);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, OpenGL::grShaderData, 0, 4*4*sizeof(float));
}
void ShaderFPrimitiv::init33() {
	ShaderFPrimitiv *sh = new ShaderFPrimitiv();
	const GLchar *vrsh = 
		"in vec4 position;"
		"void main () {"
			"gl_Position = position;"
		"}",
		*frsh = 
		"uniform vec4 fillColor;"
		"out vec4 color;"
		"void main () {"
			"color = fillColor;"
		"}",
		*gmsh = 
		"layout (points) in;"
		"layout (triangle_strip) out;"
		"layout (max_vertices = 4) out;"
		"layout(shared) uniform grShaderData {"
			"mat4 viewMatrixValue;"
		"};"
		"void main () {"
			"vec4 Pos = gl_in[0].gl_Position.xyzw;"
			"gl_Position = vec4(Pos.x, Pos.y, 0.0, 1.0)*viewMatrixValue;"
			"EmitVertex();"
			
			"gl_Position = vec4(Pos.x, Pos.y+Pos.w, 0.0, 1.0)*viewMatrixValue;"
			"EmitVertex();"
			
			"gl_Position = vec4(Pos.x+Pos.z, Pos.y, 0.0, 1.0)*viewMatrixValue;"
			"EmitVertex();"
			
			"gl_Position = vec4(Pos.x+Pos.z, Pos.y+Pos.w, 0.0, 1.0)*viewMatrixValue;"
			"EmitVertex();"
			"EndPrimitive();"
		"}";
	/*size_t frshLength = strlen(frsh), vrshLength = strlen(vrsh), gmshLength = strlen(gmsh);
	GLint success;
	shi->shaderProgram = glCreateProgram();
	shi->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	shi->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shi->geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	
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
	
	glShaderSource(shi->geometryShader, 1, &gmsh, (const GLint*)&gmshLength);
	glCompileShader(shi->geometryShader);
	glGetShaderiv(shi->geometryShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shi->geometryShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shi->geometryShader, InfoLog);
	}
	
	glAttachShader(shi->shaderProgram, shi->vertexShader);
	glAttachShader(shi->shaderProgram, shi->fragmentShader);
	glAttachShader(shi->shaderProgram, shi->geometryShader);
	glLinkProgram(shi->shaderProgram);
	
	shi->position = glGetAttribLocation(shi->shaderProgram, "position");
	shi->fillColor = glGetUniformLocation(shi->shaderProgram, "fillColor");
	shi->viewMatrix = glGetUniformBlockIndex(shi->shaderProgram, "viewMatrix");
	glUniformBlockBinding(shi->shaderProgram, shi->viewMatrix, 1);// glBindBufferBase
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, OpenGL::viewMatrix, 0, 4*4*sizeof(float));// glBindBufferBase
	*/
	ShaderFPrimitiv::prog = sh;
	GLShaderLoadTask *task = new GLShaderLoadTask(sh, vrsh, frsh, gmsh);
	Windows::window->eachFrame.addTask(task, 0);
}

ShaderF3D* ShaderF3D::prog = nullptr;
ShaderF3D::ShaderF3D() :GLShader(ShaderF3D::CRC32) {
	
}
void ShaderF3D::init() {
	this->position = glGetAttribLocation(this->shaderProgram, "position");
	this->fillColor = glGetUniformLocation(this->shaderProgram, "fillColor");
	this->transformMatrix = glGetUniformLocation(this->shaderProgram, "transformMatrix");
	this->grShaderData = glGetUniformBlockIndex(this->shaderProgram, "grShaderData");
	glUniformBlockBinding(this->shaderProgram, this->grShaderData, 1);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, OpenGL::grShaderData, 0, 4*4*sizeof(float));
}
void ShaderF3D::init33() {
	ShaderF3D *sh = new ShaderF3D();
	const GLchar *vrsh = 
		"layout(shared) uniform grShaderData {"
			"mat4 viewMatrixValue;"
		"};"
		"uniform mat4 transformMatrix;"
		"in vec3 position;"
		"out vec3 w_position;"
		"out vec3 c_normal;"
		"out vec3 c_eye;"
		"out vec3 c_light;"
		"void main () {"
			//"cameraPos = vec3(0, 0, 0);"
			"w_position = (vec4(position, 1)*transformMatrix).xyz;"
			"gl_Position = vec4(w_position, 1)*viewMatrixValue;"
			"c_eye = 0 - (vec4(position, 1)*viewMatrixValue*transformMatrix).xyz;"
			"c_light = normalize((vec4(0, 0, 0, 1)*viewMatrixValue).xyz+c_eye);"
			"c_normal = normalize((vec4(position.xyz, 0)*transformMatrix*viewMatrixValue).xyz);"
		"}",
		*frsh = 
		"uniform vec4 fillColor;"
		"out vec4 color;"
		"in vec3 w_position;"
		"in vec3 c_normal;"
		"in vec3 c_eye;"
		"in vec3 c_light;"
		"void main () {"
			"float cosTheta = clamp( dot( c_normal,c_light ), 0, 1 );"
			"color = vec4( fillColor.xyz*max(cosTheta, 0.3), fillColor.w);"
		"}",
		*gmsh = 
		"layout (triangles) in;"
		"layout (triangle_strip) out;"
		"layout (max_vertices = 3) out;"
		"layout(shared) uniform viewMatrix {"
			"mat4 viewMatrixValue;"
		"};"
		"void main () {"
			"vec3 Pos = gl_in[0].gl_Position.xyz;"
			"gl_Position = vec4(gl_in[0].gl_Position.xy, gl_in[0].gl_Position.z-4, 1.0)*viewMatrixValue;"
			"gl_Normal = 1.0;"
			"EmitVertex();"
			
			"gl_Position = vec4(gl_in[1].gl_Position.xy, gl_in[1].gl_Position.z-4, 1.0)*viewMatrixValue;"
			"gl_Normal = 1.0;"
			"EmitVertex();"
			
			"gl_Position = vec4(gl_in[2].gl_Position.xy, gl_in[2].gl_Position.z-4, 1.0)*viewMatrixValue;"
			"gl_Normal = 1.0;"
			"EmitVertex();"
	
			"EndPrimitive();"
		"}";
	gmsh = nullptr;
	ShaderF3D::prog = sh;
	GLShaderLoadTask *task = new GLShaderLoadTask(sh, vrsh, frsh);
	Windows::window->eachFrame.addTask(task, 0);
}



GLShaderLoadTask::GLShaderLoadTask(GLShader* sh, const GLchar *vs, const GLchar *fs, const GLchar *gs) :shader(sh), vs(vs), gs(gs), fs(fs){
	
}
int GLShaderLoadTask::processExecute() {
	if (OpenGL::ver==OpenGL::VER_CORE_100) return true;
	GLint fsLength, vsLength, gsLength;
	fsLength = strlen(this->fs);
	vsLength = strlen(this->vs);
	if (this->gs!=nullptr) gsLength = strlen(this->gs);
	GLint success;
	this->shader->shaderProgram = glCreateProgram();
	this->shader->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	this->shader->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (this->gs!=nullptr) this->shader->geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	
	glShaderSource(this->shader->vertexShader, 1, &this->vs, &vsLength);
	glCompileShader(this->shader->vertexShader);
	glGetShaderiv(this->shader->vertexShader, GL_COMPILE_STATUS, &success);
	delete this->vs;
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(this->shader->vertexShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", this->shader->vertexShader, InfoLog);
		return false;
	}

	glShaderSource(this->shader->fragmentShader, 1, &this->fs, &fsLength);
	glCompileShader(this->shader->fragmentShader);
	glGetShaderiv(this->shader->fragmentShader, GL_COMPILE_STATUS, &success);
	delete this->fs;
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(this->shader->fragmentShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", this->shader->fragmentShader, InfoLog);
		return false;
	}
	
	if (this->gs!=nullptr) {
		glShaderSource(this->shader->geometryShader, 1, &this->gs, &gsLength);
		glCompileShader(this->shader->geometryShader);
		glGetShaderiv(this->shader->geometryShader, GL_COMPILE_STATUS, &success);
		delete this->gs;
		if (!success) {
			GLchar InfoLog[1024];
			glGetShaderInfoLog(this->shader->geometryShader, sizeof(InfoLog), NULL, InfoLog);
			fprintf(stderr, "Error compiling shader type %d: '%s'\n", this->shader->geometryShader, InfoLog);
			return false;
		}
	}
	glAttachShader(this->shader->shaderProgram, this->shader->vertexShader);
	glAttachShader(this->shader->shaderProgram, this->shader->fragmentShader);
	if (this->gs!=nullptr) glAttachShader(this->shader->shaderProgram, this->shader->geometryShader);
	
	this->shader->mapping();
	glLinkProgram(this->shader->shaderProgram);
	
	this->shader->init();
	return true;
}

/*void ShaderBitmap::init() {
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
void ShaderFPrimitiv::init() {
	ShaderFPrimitiv *shi = new ShaderFPrimitiv();
	const GLchar *vrsh = 
		//"#version 330 core\n"
		"in vec4 position;"
		"void main () {"
			"gl_Position = position;"
		"}",
		*frsh = 
		//"#version 330 core\n"
		"uniform sampler2D colorTexture;"
		"out vec4 color;"
		"void main () {"
			"color = vec4(0, 0, 1, 1);"
		"}",
		*gmsh = 
		//"#version 330 core\n"
		"layout (points) in;"
		"layout (triangle_strip) out;"
		"layout (max_vertices = 4) out;"
		"layout(shared) uniform viewMatrix {"
			"mat4 viewMatrixValue;"
		"};"
		"uniform sampler2D colorTexture;"
		"out vec2 TexCoord;"
		"void main () {"
			"vec4 Pos = gl_in[0].gl_Position.xy;"
			"gl_Position = vec4(Pos.x, Pos.y, 0.0, 1.0)*viewMatrixValue;"
			"EmitVertex();"
			
			"gl_Position = vec4(Pos.x, Pos.y+Pos.w, 0.0, 1.0)*viewMatrixValue;"
			"EmitVertex();"
			
			"gl_Position = vec4(Pos.x+Pos.z, Pos.y, 0.0, 1.0)*viewMatrixValue;"
			"EmitVertex();"
			
			"gl_Position = vec4(Pos.x+Pos.z, Pos.y+Pos.w, 0.0, 1.0)*viewMatrixValue;"
			"EmitVertex();"
			"EndPrimitive();"
		"}";
	size_t frshLength = strlen(frsh), vrshLength = strlen(vrsh), gmshLength = strlen(gmsh);
	GLint success;
	shi->shaderProgram = glCreateProgram();
	shi->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	shi->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shi->geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	
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
	
	glShaderSource(shi->geometryShader, 1, &gmsh, (const GLint*)&gmshLength);
	glCompileShader(shi->geometryShader);
	glGetShaderiv(shi->geometryShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shi->geometryShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shi->geometryShader, InfoLog);
	}
	
	glAttachShader(shi->shaderProgram, shi->vertexShader);
	glAttachShader(shi->shaderProgram, shi->fragmentShader);
	glAttachShader(shi->shaderProgram, shi->geometryShader);
	glLinkProgram(shi->shaderProgram);
	
	shi->position = glGetAttribLocation(shi->shaderProgram, "position");
	shi->fillColor = glGetUniformLocation(shi->shaderProgram, "fillColor");
	shi->viewMatrix = glGetUniformBlockIndex(shi->shaderProgram, "viewMatrix");
	glUniformBlockBinding(shi->shaderProgram, shi->viewMatrix, 1);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, OpenGL::viewMatrix, 0, 4*4*sizeof(float));
	ShaderFPrimitiv::prog = shi;
}*/