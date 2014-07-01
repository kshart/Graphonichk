#include "grMain.h"
#include "grOpenGL.h"

using namespace Graphonichk;
/* struct struct_glslStr {
	GLShader *sh;
	GLchar *vs, *gs, *fs;
	struct_glslStr() :vs(0), gs(0), fs(0), sh(0) {};
};
typedef struct struct_glslStr glslStr;
*/
GLuint OpenGL::grShaderData = 0;
GLuint OpenGL::circleBuffer = 0;
GLuint OpenGL::textureGLID = 0;
OpenGL::OPENGL_VER OpenGL::ver;
vector<OpenGL::viewport> OpenGL::viewportBuffer;
vector<Matrix3D> OpenGL::viewMatrixBuffer;
int OpenGL::init(OPENGL_VER ver) {
	if (glewInit() != GLEW_OK) {
		puts("glewInit ERROR;");
	}
	if (GLEW_VERSION_1_1) {
		puts("GLEW_VERSION_1_1");
	}
	if (GLEW_VERSION_3_3) {
		puts("GLEW_VERSION_3_3");
	}
	if (GLEW_VERSION_4_0) {
		puts("GLEW_VERSION_4_0");
	}
	OpenGL::ver = ver;
	viewport vp;
	vp.x = 0;
	vp.y = 0;
	vp.width = 0;
	vp.height = 0;
	Matrix3D mat;
	OpenGL::viewportBuffer.push_back(vp);
	OpenGL::viewMatrixBuffer.push_back(mat);
	
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
		//mainFBO
			
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
		ShaderBitmap::prog = new ShaderBitmap();
		ShaderBitmapAtlas::prog = new ShaderBitmapAtlas();
		ShaderTextFieldBuffer::prog = new ShaderTextFieldBuffer();
		ShaderSVGmain::prog = new ShaderSVGmain();
		ShaderFPrimitiv::prog = new ShaderFPrimitiv();
		ShaderPost::prog = new ShaderPost();
		ShaderF3D::prog = new ShaderF3D();
		ShaderPartRect::prog = new ShaderPartRect();
		GLShaderLoad *shLoad;
		shLoad = new GLShaderLoad(ShaderBitmap::prog, "shader330/vs/Bitmap.vs", "shader330/gs/Bitmap.gs", "shader330/fs/Bitmap.fs");
		shLoad = new GLShaderLoad(ShaderBitmapAtlas::prog, "shader330/vs/BitmapAtlas.vs", "shader330/gs/BitmapAtlas.gs", "shader330/fs/BitmapAtlas.fs");
		shLoad = new GLShaderLoad(ShaderTextFieldBuffer::prog, "shader330/vs/TextFieldBuffer.vs", "shader330/gs/TextFieldBuffer.gs", "shader330/fs/TextFieldBuffer.fs");
		shLoad = new GLShaderLoad(ShaderSVGmain::prog, "shader330/vs/SVGmain.vs", "", "shader330/fs/SVGmain.fs");
		shLoad = new GLShaderLoad(ShaderFPrimitiv::prog, "shader330/vs/FPrimitiv.vs", "shader330/gs/FPrimitiv.gs", "shader330/fs/FPrimitiv.fs");
		shLoad = new GLShaderLoad(ShaderPost::prog, "shader330/vs/Post.vs", "shader330/gs/Post.gs", "shader330/fs/Post.fs");
		shLoad = new GLShaderLoad(ShaderF3D::prog, "shader330/vs/F3D.vs", "", "shader330/fs/F3D.fs");
		shLoad = new GLShaderLoad(ShaderPartRect::prog, "shader330/vs/PartRect.vs", "shader330/gs/PartRect.gs", "shader330/fs/PartRect.fs");
		
		ShaderBW::init33();
		
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
void OpenGL::setViewMatrix(Matrix3D &matrix) {
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
void OpenGL::multViewMatrix(Matrix3D &mulmatrix) {
	Matrix3D *matrix = &OpenGL::viewMatrixBuffer[OpenGL::viewMatrixBuffer.size()-1];
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
	printf("<opengl renderer='%s' vendor='%s' version='%s' version_ogl='%i %i'/>\n", glGetString(GL_RENDERER), glGetString(GL_VENDOR), glGetString(GL_VERSION), major, minor);
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

ShaderBitmapAtlas* ShaderBitmapAtlas::prog = nullptr;
ShaderBitmapAtlas::ShaderBitmapAtlas() :ShaderShRect(ShaderBitmapAtlas::CRC32) {
	
}
void ShaderBitmapAtlas::init() {
	this->ShaderShRect::init();
	this->attrRectID = glGetAttribLocation(this->shaderProgram, "attrRectID");
	this->texture = glGetUniformLocation(this->shaderProgram, "colorTexture");
	this->coordRect = glGetUniformLocation(this->shaderProgram, "coordRect");
	this->rectID = glGetUniformLocation(this->shaderProgram, "rectID");
	glUniform1i(this->texture, 0);
	glUniform1i(this->coordRect, 0);
}

ShaderPartRect* ShaderPartRect::prog = nullptr;
ShaderPartRect::ShaderPartRect() :ShaderShRect(ShaderPartRect::CRC32) {
	
}
void ShaderPartRect::init() {
	this->ShaderShRect::init();
	this->texture = glGetUniformLocation(this->shaderProgram, "colorTexture");
	this->coordRect = glGetUniformLocation(this->shaderProgram, "coordRect");
	this->rectID = glGetUniformLocation(this->shaderProgram, "rectID");
	this->offset = glGetUniformLocation(this->shaderProgram, "offset");
	glUniform1i(this->texture, 0);
	glUniform1i(this->coordRect, 0);
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

ShaderFPrimitiv* ShaderFPrimitiv::prog = nullptr;
ShaderFPrimitiv::ShaderFPrimitiv() :ShaderShRect(ShaderFPrimitiv::CRC32) {
	
}
void ShaderFPrimitiv::init() {
	this->ShaderShRect::init();
	this->fillColor = glGetUniformLocation(this->shaderProgram, "fillColor");
}

ShaderF3D* ShaderF3D::prog = nullptr;
ShaderF3D::ShaderF3D() :GLShader(ShaderF3D::CRC32), position(-1), fillColor(-1), transformMatrix(-1), grShaderData(-1) {
	
}
void ShaderF3D::init() {
	this->position = glGetAttribLocation(this->shaderProgram, "position");
	this->fillColor = glGetUniformLocation(this->shaderProgram, "fillColor");
	this->transformMatrix = glGetUniformLocation(this->shaderProgram, "transformMatrix");
	this->grShaderData = glGetUniformBlockIndex(this->shaderProgram, "grShaderData");
	glUniformBlockBinding(this->shaderProgram, this->grShaderData, 1);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, OpenGL::grShaderData, 0, 4*4*sizeof(float));
	printf("ShaderF3D::init() %i %i %i %i %i %i %i\n", this->vertexShader, this->fragmentShader, this->geometryShader, this->position, this->fillColor, this->transformMatrix, this->grShaderData);
}

ShaderPost* ShaderPost::prog = nullptr;
ShaderPost::ShaderPost() :GLShader(ShaderPost::CRC32) {}
void ShaderPost::init() {
	this->texture = glGetUniformLocation(this->shaderProgram, "img");
	glUniform1i(this->texture, 0);
}

ShaderBW* ShaderBW::prog = nullptr;
ShaderBW::ShaderBW() :GLShader(ShaderBW::CRC32) {}
void ShaderBW::init() {
	this->texture = glGetUniformLocation(this->shaderProgram, "texture");
	glUniform1i(this->texture, 0);
}
void ShaderBW::init33() {
	ShaderBW *sh = new ShaderBW();
	const GLchar *vrsh = 
		"#version 330 core\n"
		"in vec2 position;"
		"void main () {"
		"}",
		*frsh = 
		"#version 330 core\n"
		"#define MODE_BRIGHTNESS 1\n"
		"#define MODE_GAMMA 2\n"
		"#define MODE_CONTRAST 3\n"
		"#define MODE_COLOR_BALANCE 4\n"
	
		"uniform sampler2D texture;"
		"uniform float brightnessPower = 1.0;"
		"uniform int mode = MODE_BRIGHTNESS;"
		"in vec2 TexCoord;"
		"out vec4 color;"
		"void main () {"
			"vec4 texColor = texture(texture, TexCoord);"
			"switch (mode) {"
				"case MODE_BRIGHTNESS:"
					"color = texColor*brightnessPower;"
					"break;"
				"case MODE_GAMMA:"
					"color = texColor*brightnessPower;"
					"break;"
				"case MODE_CONTRAST:"
					"color = texColor*brightnessPower;"
					"break;"
				"case MODE_COLOR_BALANCE:"
					"color = texColor*brightnessPower;"
					"break;"
			"}"
		"}",
		*gmsh = 
		"#version 330 core\n"
		"layout (points) in;"
		"layout (triangle_strip) out;"
		"layout (max_vertices = 4) out;"
		"uniform sampler2D texture;"
		"out vec2 TexCoord;"
		"void main () {"
			"ivec2 winSize = textureSize(texture, 0);"
			"gl_Position = vec4(-1, -1, 0, 1);"
			"TexCoord = vec2(0.0, 0.0);"
			"EmitVertex();"
			
			"gl_Position = vec4(-1, 1, 0, 1);"
			"TexCoord = vec2(0.0, 1.0);"
			"EmitVertex();"
			
			"gl_Position = vec4(1, -1, 0, 1);"
			"TexCoord = vec2(1.0, 0.0);"
			"EmitVertex();"
			
			"gl_Position = vec4(1, 1, 0, 1);"
			"TexCoord = vec2(1.0, 1.0);"
			"EmitVertex();"
	
			"EndPrimitive();"
		"}";
	ShaderBW::prog = sh;
	Windows::window->eachFrame.addTask(new GLShaderLoadTask(sh, vrsh, frsh, gmsh));
}


GLShaderLoadTask::GLShaderLoadTask(GLShader* sh, const GLchar *vs, const GLchar *fs, const GLchar *gs, size_t vsLength, size_t fsLength, size_t gsLength ) :
	shader(sh), 
	vs(vs), gs(gs), fs(fs),
	vsLength(vsLength), fsLength(fsLength), gsLength(gsLength) {
	
}
int GLShaderLoadTask::processExecute() {
	if (OpenGL::ver==OpenGL::VER_CORE_100) return true;
	if (this->vs==nullptr || this->fs==nullptr) {
		puts("GLShaderLoadTask::processExecute()");
		return false;
	}
	if (this->fsLength==0) this->fsLength = strlen(this->fs);
	if (this->vsLength==0) this->vsLength = strlen(this->vs);
	if (this->gs!=nullptr && this->gsLength==0) this->gsLength = strlen(this->gs);
	
	GLint vsLength = (GLint)this->vsLength, gsLength = (GLint)this->gsLength, fsLength = (GLint)this->fsLength;
	GLint success;
	this->shader->shaderProgram = glCreateProgram();
	this->shader->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	this->shader->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (this->gs!=nullptr) this->shader->geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	
	glShaderSource(this->shader->vertexShader, 1, &this->vs, &vsLength);
	glCompileShader(this->shader->vertexShader);
	glGetShaderiv(this->shader->vertexShader, GL_COMPILE_STATUS, &success);
	//delete this->vs;
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(this->shader->vertexShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\"%s\"\n", this->shader->vertexShader, InfoLog, this->vs);
		return false;
	}

	glShaderSource(this->shader->fragmentShader, 1, &this->fs, &fsLength);
	glCompileShader(this->shader->fragmentShader);
	glGetShaderiv(this->shader->fragmentShader, GL_COMPILE_STATUS, &success);
	//delete this->fs;
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(this->shader->fragmentShader, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n\"%s\"", this->shader->fragmentShader, InfoLog, this->fs);
		return false;
	}
	
	if (this->gs!=nullptr) {
		glShaderSource(this->shader->geometryShader, 1, &this->gs, &gsLength);
		glCompileShader(this->shader->geometryShader);
		glGetShaderiv(this->shader->geometryShader, GL_COMPILE_STATUS, &success);
		//delete this->gs;
		if (!success) {
			GLchar InfoLog[1024];
			glGetShaderInfoLog(this->shader->geometryShader, sizeof(InfoLog), NULL, InfoLog);
			fprintf(stderr, "Error compiling shader type %d: '%s'\n\"%s\"", this->shader->geometryShader, InfoLog, this->gs);
			return false;
		}
	}
	glAttachShader(this->shader->shaderProgram, this->shader->vertexShader);
	glAttachShader(this->shader->shaderProgram, this->shader->fragmentShader);
	if (this->gs!=nullptr) glAttachShader(this->shader->shaderProgram, this->shader->geometryShader);
	
	this->shader->mapping();
	glLinkProgram(this->shader->shaderProgram);
	
	this->shader->init();
	printf("<shader event='loaded' id='%i'/>\n", this->shader->shaderProgram);
	return true;
}

GLShaderLoad::GLShaderLoad(GLShader* sh, const string vs, const string gs, const string fs) :shader(sh) {
	this->vsLoad = !vs.empty();
	this->gsLoad = !gs.empty();
	this->fsLoad = !fs.empty();
	printf("loading %i %i %i\n", this->vsLoad, this->gsLoad, this->fsLoad);
	FileLoad *file;
	if (vsLoad) {
		file = new FileLoad(vs);
		file->addEventHandler(FileLoad::SUCCESS, loading, this);
	}
	if (gsLoad) {
		file = new FileLoad(gs);
		file->addEventHandler(FileLoad::SUCCESS, loading, this);
	}
	if (fsLoad) {
		file = new FileLoad(fs);
		file->addEventHandler(FileLoad::SUCCESS, loading, this);
	}
}
void GLShaderLoad::loading(const EventFileLoad* e) {
	GLShaderLoad *sh = (GLShaderLoad*)(e->obj);
	
	size_t nameLength = e->file->path.length();
	
	if (sh->vs==nullptr && e->file->path[nameLength-2]=='v' && e->file->path[nameLength-1]=='s') {
		sh->vs = (GLchar*)e->file->data;
		sh->vsLength = e->file->size;
	}else if (sh->gs==nullptr && e->file->path[nameLength-2]=='g' && e->file->path[nameLength-1]=='s') {
		sh->gs = (GLchar*)e->file->data;
		sh->gsLength = e->file->size;
	}else if (sh->fs==nullptr && e->file->path[nameLength-2]=='f' && e->file->path[nameLength-1]=='s') {
		sh->fs = (GLchar*)e->file->data;
		sh->fsLength = e->file->size;
	}
	if (sh->gsLoad) {
		if (sh->vs!=nullptr && sh->gs!=nullptr && sh->fs!=nullptr &&
			sh->vsLength>0 && sh->gsLength>0 && sh->fsLength>0) {
			Windows::window->eachFrame.addTask( new GLShaderLoadTask(sh->shader, sh->vs, sh->fs, sh->gs, sh->vsLength, sh->fsLength, sh->gsLength) );
			delete sh;
		}
	}else{
		if (sh->vs!=nullptr && sh->fs!=nullptr &&
			sh->vsLength>0 && sh->fsLength>0) {
			Windows::window->eachFrame.addTask( new GLShaderLoadTask(sh->shader, sh->vs, sh->fs, nullptr, sh->vsLength, sh->fsLength, 0) );
			delete sh;
		}
	}
	
}