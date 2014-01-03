#include <vector>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/wglext.h>
#include <GL/glext.h>

#include "grBaseTypes.h"
#include "grShape.h"
using namespace std;
using namespace grEngine;

grEngine::Shape::Shape (short type) {
	this->type = type;
	this->parent = NULL;
	this->globalx = 0;
	this->x = 0;
	this->globaly = 0;
	this->y = 0;
	this->width = this->height = 0;
	this->offsetPos.x = this->offsetPos.y = 0;
}
void grEngine::Shape::trace() {
	printf("Shape x=%i, y=%i, gx=%i, gy=%i, w=%i, h=%i\n", this->x, this->y, this->globalx, this->globaly, this->width, this->height);
}
void grEngine::Shape::drag(short x, short y) {
	this->x = x;
	this->y = y;
	if (this->parent!=NULL) {
		this->globalx = parent->globalx+x;
		this->globaly = parent->globaly+y;
	}
	root.window->renderComplete = false;
}
int grEngine::Shape::renderGLComptAll() {
	return false;
}
int grEngine::Shape::renderGL400() {
	return false;
}
int grEngine::Shape::renderGL330() {
	return false;
}
int grEngine::Shape::renderGL210() {
	return false;
}
Shape* grEngine::Shape::globalHitTest(short x, short y) {
	if ( x>this->globalx+this->offsetPos.x &&
		 y>this->globaly+this->offsetPos.y &&
		 x<this->globalx+this->offsetPos.x+this->width &&
		 y<this->globaly+this->offsetPos.y+this->height) {
		return this;
	}
	return NULL;
}
int grEngine::Shape::callEvent(EventMouse* event) {
	event->obj = this;
	for(int i=0, s=this->eventList.size(); i<s; i++) {
		if (this->eventList[i].type == event->type) {
			this->eventList[i].fun(event);
		}
	}
}

grEngine::Bitmap::Bitmap(Texture *tex) :Shape(Shape::TYPE_TEXTURE) {
	this->tex = tex;
	if (tex->type!=0 && tex->format!=0) {
		this->width = tex->width;
		this->height = tex->height;
	}else{
		root.window->bitmapUpdateBuffer.push_back(this);
	}
}
void grEngine::Bitmap::trace() {
	printf("<Bitmap x='%i' y='%i' gx='%i' gy='%i' w='%i' h='%i' texId='%i'/>\n", this->x, this->y, this->globalx, this->globaly, this->width, this->height, this->tex);
}
int grEngine::Bitmap::renderGLComptAll() {
	Texture *tex = this->tex;
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, tex->GLID);
	glColor4ub(0xFF,0xFF,0xFF,0xFF);
	glBegin( GL_QUADS );// <editor-fold defaultstate="collapsed" desc="GL_QUADS">
		glTexCoord2d( 0.0, 0.0 );	glVertex2s(this->globalx, this->globaly );
		glTexCoord2d( 0.0, 1.0 );	glVertex2s(this->globalx, this->globaly+tex->height );
		glTexCoord2d( 1.0, 1.0 );	glVertex2s(this->globalx+tex->width, this->globaly+tex->height );
		glTexCoord2d( 1.0, 0.0 );	glVertex2s(this->globalx+tex->width, this->globaly );
	glEnd();// </editor-fold>
	glDisable( GL_TEXTURE_2D );
	return true;
}
int grEngine::Bitmap::renderGL400() {
	return false;
}
int grEngine::Bitmap::renderGL330() {
	//glEnable( GL_TEXTURE_2D );
	printf("renderGL330\n");
	short vertex[12] = {
		this->globalx, this->globaly,
		this->globalx+this->width, this->globaly,
		this->globalx+this->width, this->globaly+this->height,
		this->globalx, this->globaly,
		this->globalx+this->width, this->globaly+this->height,
		this->globalx, this->globaly+this->height
	};
	short texCoord[12] = {
		0, 0,
		1, 0,
		1, 1,
		0, 0,
		1, 1,
		0, 1
	};
	glBindTexture(GL_TEXTURE_2D, tex->GLID);
	GLuint meshVAO, meshVBO;
	glGenVertexArrays(1, &meshVAO);
	glBindVertexArray(meshVAO);
	glGenBuffers(1, &meshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
	glBufferData(GL_ARRAY_BUFFER, 12*2, vertex, GL_STATIC_DRAW);
	GLint positionLocation = glGetAttribLocation(GLShader::glsl->shaderProgram, "position");
	glVertexAttribPointer(positionLocation, 2, GL_SHORT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(positionLocation);
	glGenBuffers(1, &meshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
	glBufferData(GL_ARRAY_BUFFER, 12*2, texCoord, GL_STATIC_DRAW);
	positionLocation = glGetAttribLocation(GLShader::glsl->shaderProgram, "texCoord");
	glVertexAttribPointer(positionLocation, 2, GL_SHORT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(positionLocation);
	glBindVertexArray(meshVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDisable(GL_TEXTURE_2D);
	return true;
}
int grEngine::Bitmap::renderGL210() {
	if (this->tex->GLID == 0) return false;
	printf("renderGL210 \n");
	unsigned char faces[4] = {0, 1, 2, 3};
	short texCoord[8] = { 0, 0, 0, 1, 1, 1, 1, 0};
	short vexCoord[8] = { 
		this->globalx, this->globaly,
		this->globalx, this->globaly+this->height,
		this->globalx+this->width, this->globaly+this->height,
		this->globalx+this->width, this->globaly
	};
	Texture *tex = this->tex;
	
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, tex->GLID);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
		glVertexPointer (2, GL_SHORT, 0, vexCoord);
		glTexCoordPointer(2, GL_SHORT, 0, texCoord);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, faces);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable( GL_TEXTURE_2D );
	return true;
}

grEngine::Buffer::Buffer() {
	this->success = false;
	this->status = false;
	this->tex = NULL;
}
bool grEngine::Buffer::switchOn() {
	if (!this->status) {
		root.window->FBOBuffer.push_back(this);
		status = true;
		success = false;
	}
	return false;
}
bool grEngine::Buffer::switchOff() {
	return false;
}
int grEngine::Buffer::bufferGLComptAll() {
	return false;
}
int grEngine::Buffer::bufferGL400() {
	return false;
}
int grEngine::Buffer::bufferGL330() {
	return false;
}
int grEngine::Buffer::bufferGL210() {
	return false;
}

grEngine::Directory::Directory() :Shape(Shape::TYPE_DIRECTORY), Buffer() {
	this->shapeCache = NULL;
	this->totalShape = this->totalDir = 0;
	this->parent = NULL;
}
void grEngine::Directory::trace() {
	printf("<Directory pos='%i, %i' gpos='%i, %i' rect='%i, %i, %i, %i'>\n", this->x, this->y, this->globalx, this->globaly, this->offsetPos.x, this->offsetPos.y, this->width, this->height);
	for (int i=0; i<this->child.size(); i++) this->child[i]->trace(); 
	printf("</Directory>\n");
}
int grEngine::Directory::renderGLComptAll() {
	if (this->success && this->tex->GLID!=0) {
		Texture *tex = this->tex;
		glEnable( GL_TEXTURE_2D );
		glBindTexture(GL_TEXTURE_2D, tex->GLID);
		glColor4ub(0xFF,0xFF,0xFF,0xFF);
		glBegin( GL_QUADS );// <editor-fold defaultstate="collapsed" desc="GL_QUADS">
					glTexCoord2d( 0.0, 0.0 );	glVertex2i(this->globalx+offsetPos.x, this->globaly+offsetPos.y );
					glTexCoord2d( 0.0, 1.0 );	glVertex2i(this->globalx+offsetPos.x, this->globaly+offsetPos.y+tex->height );
					glTexCoord2d( 1.0, 1.0 );	glVertex2i(this->globalx+offsetPos.x+tex->width, this->globaly+offsetPos.y+tex->height );
					glTexCoord2d( 1.0, 0.0 );	glVertex2i(this->globalx+offsetPos.x+tex->width, this->globaly+offsetPos.y );
				glEnd();// </editor-fold>
		glDisable( GL_TEXTURE_2D );
	}else if ( this->shapeCache != NULL ) {
		#ifdef DEBUG
		printf("Directory shapeCache\n");
		#endif
		for (int i=0; i<this->shapeCache->size(); i++) {
			(this->shapeCache->at(i))->renderGLComptAll();
		}
	}else{
		for (int i=0; i<this->child.size(); i++) {
			this->child[i]->renderGLComptAll();
		}
	}
	#ifdef DEBUG
	glLineWidth(1);
	glColor4ub(0xFF,0,0,0xFF);
	glBegin(GL_LINE_STRIP);// <editor-fold defaultstate="collapsed" desc="GL_LINE_STRIP">
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y );
		glVertex2s( this->globalx+this->offsetPos.x+this->width, this->globaly+this->offsetPos.y );
		glVertex2s( this->globalx+this->offsetPos.x+this->width, this->globaly+this->offsetPos.y+this->height );
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y+this->height );
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y );
	glEnd();// </editor-fold>
	#endif
	return true;
}
int grEngine::Directory::renderGL400() {
	printf("Directory p\n");
	for (int i=0; i<this->child.size(); i++) {
		this->child[i]->trace();
		this->child[i]->renderGL400();
	}
	return true;
}
int grEngine::Directory::renderGL330() {
	if (this->success && this->tex->GLID!=0) {
		Texture *tex = this->tex;
		glEnable( GL_TEXTURE_2D );
		glBindTexture(GL_TEXTURE_2D, tex->GLID);
		glColor4ub(0xFF,0xFF,0xFF,0xFF);
		glBegin( GL_QUADS );// <editor-fold defaultstate="collapsed" desc="GL_QUADS">
					glTexCoord2d( 0.0, 0.0 );	glVertex2i(this->globalx+offsetPos.x, this->globaly+offsetPos.y );
					glTexCoord2d( 0.0, 1.0 );	glVertex2i(this->globalx+offsetPos.x, this->globaly+offsetPos.y+tex->height );
					glTexCoord2d( 1.0, 1.0 );	glVertex2i(this->globalx+offsetPos.x+tex->width, this->globaly+offsetPos.y+tex->height );
					glTexCoord2d( 1.0, 0.0 );	glVertex2i(this->globalx+offsetPos.x+tex->width, this->globaly+offsetPos.y );
				glEnd();// </editor-fold>
		glDisable( GL_TEXTURE_2D );
	}else if ( this->shapeCache != NULL ) {
		#ifdef DEBUG
		printf("Directory shapeCache\n");
		#endif
		for (int i=0; i<this->shapeCache->size(); i++) {
			(this->shapeCache->at(i))->renderGL330();
		}
	}else{
		for (int i=0; i<this->child.size(); i++) {
			this->child[i]->renderGL330();
		}
	}
	#ifdef DEBUG
	glLineWidth(1);
	glColor4ub(0xFF,0,0,0xFF);
	glBegin(GL_LINE_STRIP);// <editor-fold defaultstate="collapsed" desc="GL_LINE_STRIP">
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y );
		glVertex2s( this->globalx+this->offsetPos.x+this->width, this->globaly+this->offsetPos.y );
		glVertex2s( this->globalx+this->offsetPos.x+this->width, this->globaly+this->offsetPos.y+this->height );
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y+this->height );
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y );
	glEnd();// </editor-fold>
	#endif
	return true;
}
int grEngine::Directory::renderGL210() {
	if ( this->shapeCache != NULL ) {
		#ifdef DEBUG
		printf("Directory shapeCache\n");
		#endif
		for (int i=0; i<this->shapeCache->size(); i++) {
			(this->shapeCache->at(i))->renderGL210();
		}
	}else{
		for (int i=0; i<this->child.size(); i++) {
			this->child[i]->renderGL210();
		}
	}
	#ifdef DEBUG
	glLineWidth(1);
	glColor4ub(0xFF,0,0,0xFF);
	glBegin(GL_LINE_STRIP);// <editor-fold defaultstate="collapsed" desc="GL_LINE_STRIP">
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y );
		glVertex2s( this->globalx+this->offsetPos.x+this->width, this->globaly+this->offsetPos.y );
		glVertex2s( this->globalx+this->offsetPos.x+this->width, this->globaly+this->offsetPos.y+this->height );
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y+this->height );
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y );
	glEnd();// </editor-fold>
	#endif
	return true;
}
int grEngine::Directory::bufferGLComptAll() {
	for(int i=0; i<this->bufChild.size(); i++) {
		this->bufChild[i]->bufferGLComptAll();
	}
	#ifdef DEBUG
	this->trace();
	#endif
	glGenFramebuffers(1, &root.window->ogl->FBOGL);
	glBindFramebuffer(GL_FRAMEBUFFER, root.window->ogl->FBOGL);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, this->tex->GLID, 0);
	glViewport( 0, 0, this->width, this->height );
	glLoadIdentity( );
	gluOrtho2D( this->offsetPos.x+this->globalx, this->offsetPos.x+this->globalx+this->width, this->offsetPos.y+this->globaly, this->offsetPos.y+this->globaly+this->height );
	glClearColor( 0.1, 0, 0, 0.1 );
	glClear( GL_COLOR_BUFFER_BIT );
	
	glEnable( GL_BLEND );
	glEnable( GL_ALPHA_TEST );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	this->renderGLComptAll();
	
	glDisable( GL_BLEND );
	glDisable( GL_ALPHA_TEST );
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glDeleteFramebuffers(1, &root.window->ogl->FBOGL);
	
	this->success = true;
	return false;
}
int grEngine::Directory::bufferGL400() {
	return false;
}
int grEngine::Directory::bufferGL330() {
	return false;
}
int grEngine::Directory::bufferGL210() {
	return false;
}
bool grEngine::Directory::switchOn() {
	if (!this->status) {
		this->tex = new Texture(this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE);
		root.window->FBOBuffer.push_back(this);
		status = true;
		success = false;
	}
	return false;
}
void grEngine::Directory::updateChPos() {
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->globalx = this->globalx+this->child[i]->x;
		this->child[i]->globaly = this->globaly+this->child[i]->y;
		if (this->child[i]->type == TYPE_DIRECTORY) ((Directory*)(this->child[i]))->updateChPos();
	}
}
void grEngine::Directory::drag(short x, short y) {
	if (this->parent!=NULL) {
		this->globalx = this->parent->globalx+x;
		this->globaly = this->parent->globaly+y;
	}
	this->x = x;
	this->y = y;
	this->updateChPos();
	root.window->renderComplete = false;
}
vector<Shape*>* grEngine::Directory::getChildShape() {
	
	vector<Shape*>* arr = new vector<Shape*>;
	for (int i = 0; i<this->child.size( ); i++) {
		if (this->child[i]->type == TYPE_DIRECTORY) {
			((Directory*)(this->child[i]))->getChildShape(arr);
		}else{
			arr->push_back( this->child[i] );
		}
	}
	return arr;
}
void grEngine::Directory::getChildShape(vector<Shape*>* arr) {
	for (int i = 0; i<this->child.size( ); i++) {
		switch (this->child[i]->type) {
			case Shape::TYPE_DIRECTORY:
				((Directory*)(this->child[i]))->getChildShape(arr);
				break;
			default :
				arr->push_back( this->child[i] );
				break;
		}
	}
}
void grEngine::Directory::addChild(Shape *sh) {
	Directory *dir = this;
	short nx, ny;
	sh->parent = this;
	//	y	by	bh	outB
	//	50	-30	70	20/90
	// by	bh	outB
	// -50	100	ob.h-by
	sh->globalx = this->globalx+sh->x;
	sh->globaly = this->globaly+sh->y;
	#ifdef DEBUG
	sh->trace();
	this->trace();
	#endif
	if (sh->x+sh->offsetPos.x < this->offsetPos.x) {
		nx = sh->x+sh->offsetPos.x;
		this->width -= nx-this->offsetPos.x;
		this->offsetPos.x = nx;
	}
	if (sh->y+sh->offsetPos.y < this->offsetPos.y) {
		ny = sh->y+sh->offsetPos.y;
		this->height -= ny-this->offsetPos.y;
		this->offsetPos.y = ny;
	}
	if (sh->x+sh->offsetPos.x+sh->width > this->offsetPos.x+this->width) {
		this->width = sh->x+sh->offsetPos.x+sh->width-this->offsetPos.x;
	}
	if (sh->y+sh->offsetPos.y+sh->height-this->offsetPos.y > this->height) {
		this->height = sh->y+sh->offsetPos.y+sh->height-this->offsetPos.y;
	}
	this->child.push_back( sh );
	if (sh->type == Shape::TYPE_DIRECTORY) {
		this->totalShape += dir->totalShape;
		this->totalDir += dir->totalDir+1;
		while(dir->parent!=NULL) {
			dir = dir->parent;
			dir->totalShape += ((Directory*)sh)->totalShape;
			dir->totalDir += ((Directory*)sh)->totalDir+1;
		}
		dir->updateChPos();
	}else{
		this->totalShape++;
		while(dir->parent!=NULL) {
			dir = dir->parent;
			dir->totalShape++;
		}
	}
	#ifdef DEBUG
	printf("totalShape = %i\n", this->totalShape);
	#endif
	root.window->renderComplete = false;
}
void grEngine::Directory::setBuffer(Directory::BUFFER_TYPE type, char val) {
	/*switch (type) {
		case Directory::TO_TEXTURE :
			if ( val==Texture::LOC::UNAVAILABLE ) {
				this->buff->tex->clear();
			}else{
				glGenTextures( 1, &this->texGL );
				if (this->texGL==0) {
					this->texType = Buffer::ERROR_GEN;
					printf("Buffer::set NOGEN %i\n");
					return;
				}
				this->texType = Buffer::TO_TEXTURE;
				this->bufTexWidth = (this->width+3)&~3;
				this->bufTexHeight = (this->height+3)&~3;
				glBindTexture( GL_TEXTURE_2D, this->texGL );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, this->bufTexWidth, this->bufTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
				Windows::FBOBuffer.push_back(this);
			}
			return;
		case Directory::PREDEFINED_LIST_SH :
			if (val) {
				this->shapeCache = this->getChildShape();
			}else if (this->shapeCache != NULL) {
				delete this->shapeCache;
				this->shapeCache = NULL;
			}
			return;
	}*/
}
Shape* grEngine::Directory::globalHitTest(short x, short y) {
	Shape* sh;
	for(int i=this->child.size()-1; i>=0; i--) {
		sh = this->child[i]->globalHitTest(x, y);
		if (sh != NULL) return sh;
	}
	return NULL;
}
int grEngine::Directory::callEvent(EventMouse* event) {
	event->obj = this;
	for(int i=0, s=this->eventList.size(); i<s; i++) {
		if (this->eventList[i].type == event->type) {
			this->eventList[i].fun(event);
		}
	}
	for(int i=0, s=this->child.size(); i<s; i++) {
		this->child[i]->callEvent(event);
	}
}
/*void grEngine::Directory::addChild(Directory *dir) {
	Directory *dr = this;
	dir->parent = this;
	dir->globalx = this->globalx+dir->x;
	dir->globaly = this->globaly+dir->y;
	this->child.push_back( dir );
	this->totalShape += dir->totalShape;
	this->totalDir += dir->totalDir+1;
	while(dr->parent!=NULL) {
		dr = dr->parent;
		dr->totalDir += dir->totalDir+1;
	}
	dir->updateChPos();
}
void grEngine::Directory::addChild(Shape *sh) {
	Directory *dir = this;
	sh->parent = this;
	sh->globalx = this->globalx+sh->x;
	sh->globaly = this->globaly+sh->y;
	this->child.push_back( sh );
	this->totalShape++;
	printf("12totalShape = %i\n", this->totalShape);
	while(dir->parent!=NULL) {
		dir = dir->parent;
		dir->totalShape++;
	}
}
int grEngine::Directory::save(string path) {
	FILE *file = fopen(path.c_str(), "wb");
	this->saveFD(file, 0);
	fclose(file);
	return true;
}
 int grEngine::Directory::saveFD(FILE *file, long offset) {
	sdHead head;
	Shape *sh;
	head.signature = LIB_SIGN;
	head.ver = 1;
	head.nameLength = 0;
	head.descriptLength = 0;
	head.imgLength = 0;
	head.dirLength = this->totalDir+1;
	head.size = (sizeof(sdTypeImg)+1)*this->totalShape + (sizeof(sdTypeDir)+1)*head.dirLength+sizeof(sdHead)+head.nameLength+head.descriptLength;
	printf("pprint size=%i\n", head.size);
	head.imgOffset = sizeof(sdHead)+head.nameLength+head.descriptLength;
	fseek(file, offset+sizeof(sdHead)+head.descriptLength+head.nameLength, SEEK_SET);
	this->saveFFD(file, &head);

	fseek(file, offset, SEEK_SET);
	fwrite(&head, sizeof(sdHead), 1, file);
	return true;
}*/
/*int grEngine::Directory::saveFFD (FILE *file, sdHead *head) {
	sdTypeDir dh;
	sdTypeImg ih;
	char type = 0xAA;//GR_DIRECTORY;<bug>
	dh.size = this->child.size();
	dh.x = this->x;
	dh.y = this->y;
	fwrite(&type, 1, 1, file);
	fwrite(&dh, sizeof(sdTypeDir), 1, file);
	head->imgOffset += 1+sizeof(sdTypeDir);
	for (int i=0; i<this->child.size(); i++) {
		switch (this->child[i]->type) {
			case TYPE_DIRECTORY:
				((Directory*)(this->child[i].ln))->saveFFD(file, head);
				break;
			case GR_SHAPE:
				ih.x = ((Shape*)(this->child[i].ln))->x;
				ih.y = ((Shape*)(this->child[i].ln))->y;
				ih.offset = head->size;
				ih.ID = head->imgLength;
				head->imgLength++;
				//type = GR_SHAPE;
				fwrite(&type, 1, 1, file);
				fwrite(&ih, sizeof(sdTypeImg), 1, file);
				head->imgOffset += 1+sizeof(sdTypeImg);

				//head->size += ((Shape*)(this->child[i].ln))->link->saveFD(file, GR_IMG_I_BMP, head->size);
				fseek(file, head->imgOffset, SEEK_SET);
				break;
		}
	}
	return 1;
}*///BUGERT

grEngine::FPoint::FPoint(int rad, uint32_t color=0 ) :Shape(Shape::TYPE_POINT) {
	this->radius = rad;
	this->color.color = color;
}
int grEngine::FPoint::renderGLComptAll() {
	glPushMatrix();
	//glTranslatef();
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPEAT);
	glPointSize(this->radius);
	glBegin(GL_POINTS);
	  glColor3ub(this->color.r, this->color.g, this->color.b );
	  glVertex2s(this->globalx, this->globaly);
	glEnd();
	glPopMatrix();
}
grEngine::FLines::FLines(void *arr, short length, short w, unsigned int color=0) :Shape(Shape::TYPE_LINE) {
	this->arr = (short*)arr;
	this->length = length;
	this->lineWidth = w;
	this->color.color = color;
}
int grEngine::FLines::renderGLComptAll() {
	glPushMatrix();
	glLineWidth(this->lineWidth);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glBegin(GL_LINE_STRIP);
		glColor3ub(this->color.r, this->color.g, this->color.b );
		for (int i=0; i<this->length; i++) {
			glVertex2s(this->globalx+this->arr[i*2], this->globaly+this->arr[i*2+1]);
		}
		//
		
		//glColor3ub(0xFF, 0xFF, 0xFF);
	glEnd();
	glPopMatrix();
}
grEngine::FRect::FRect(short width, short height, uint32_t backgroundColor) :Shape(Shape::TYPE_RECT) {
	this->width = width;
	this->height = height;
	this->borderSize = 0;
	this->borderColor.color = 0;
	this->background = true;
	this->backgroundColor.color = backgroundColor;
}
grEngine::FRect::FRect(short width, short height, uint32_t borderColor, unsigned short borderSize) :Shape(Shape::TYPE_RECT) {
	this->width = width;
	this->height = height;
	this->borderSize = borderSize;
	this->borderColor.color = borderColor;
	this->background = false;
	this->backgroundColor.color = 0;
}
grEngine::FRect::FRect(short width, short height, uint32_t backgroundColor, uint32_t borderColor, unsigned short borderSize) :Shape(Shape::TYPE_RECT) {
	this->width = width;
	this->height = height;
	this->borderSize = borderSize;
	this->borderColor.color = borderColor;
	this->background = true;
	this->backgroundColor.color = backgroundColor;
}
int grEngine::FRect::renderGLComptAll() {
	glPushMatrix();
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	if (this->background) {
		glBegin(GL_QUADS);
			glColor3ub(this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b );
			glVertex2s(this->globalx, this->globaly);
			glVertex2s(this->globalx+this->width, this->globaly);
			glVertex2s(this->globalx+this->width, this->globaly+this->height);
			glVertex2s(this->globalx, this->globaly+this->height);
		glEnd();
	}
	if (this->borderSize) {
		glLineWidth(this->borderSize);
		glPointSize(this->borderSize);
		glBegin(GL_LINE_LOOP);
			glColor3ub(this->borderColor.r, this->borderColor.g, this->borderColor.b );
			glVertex2s(this->globalx, this->globaly);
			glVertex2s(this->globalx+this->width, this->globaly);
			glVertex2s(this->globalx+this->width, this->globaly+this->height);
			glVertex2s(this->globalx, this->globaly+this->height);
		glEnd();
		glBegin(GL_POINTS);
			glColor3ub(this->borderColor.r, this->borderColor.g, this->borderColor.b );
			glVertex2s(this->globalx, this->globaly);
			glVertex2s(this->globalx+this->width, this->globaly);
			glVertex2s(this->globalx+this->width, this->globaly+this->height);
			glVertex2s(this->globalx, this->globaly+this->height);
		glEnd();
	}
	glPopMatrix();
}