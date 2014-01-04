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
#include "UI/UIMain.h"
using namespace std;
using namespace grEngine;

grEngine::Shape::Shape(int crc32) {
	this->crc32 = crc32;
	this->mouseEventActive = false;
	this->mouseEventRollOver = false;
	this->parent = NULL;
	this->globalx = 0;
	this->x = 0;
	this->globaly = 0;
	this->y = 0;
	this->width = this->height = 0;
	this->offsetPos.x = this->offsetPos.y = 0;
}
void Shape::trace() {
	printf("Shape a='%i' x=%i, y=%i, gx=%i, gy=%i, w=%i, h=%i\n", this->mouseEventActive, this->x, this->y, this->globalx, this->globaly, this->width, this->height);
}
void Shape::drag(short x, short y) {
	this->x = x;
	this->y = y;
	short ny, nx;
	if (this->parent!=NULL) {
		this->globalx = this->parent->globalx+x;
		this->globaly = this->parent->globaly+y;
		if (this->x+this->offsetPos.x < this->parent->offsetPos.x) {
			nx = this->x+this->offsetPos.x;
			this->width -= nx-this->parent->offsetPos.x;
			this->offsetPos.x = nx;
		}
		if (this->y+this->offsetPos.y < this->parent->offsetPos.y) {
			ny = this->y+this->offsetPos.y;
			this->parent->height -= ny-this->parent->offsetPos.y;
			this->parent->offsetPos.y = ny;
		}
		if (this->x+this->offsetPos.x+this->width > this->parent->offsetPos.x+this->parent->width) {
			this->parent->width = this->x+this->offsetPos.x+this->width-this->parent->offsetPos.x;
		}
		if (this->y+this->offsetPos.y+this->height-this->parent->offsetPos.y > this->parent->height) {
			this->parent->height = this->y+this->offsetPos.y+this->height-this->parent->offsetPos.y;
		}
	}
	root.window->renderComplete = false;
}
void Shape::updateGlobalPosition() {
	if (this->parent==NULL) {
		this->globalx = this->globaly = 0;
	}else{
		this->globalx = this->parent->globalx + this->x;
		this->globaly = this->parent->globaly + this->y;
	}
}
int Shape::renderGLComptAll() {
	return false;
}
int Shape::renderGL400() {
	return false;
}
int Shape::renderGL330() {
	return false;
}
int Shape::renderGL210() {
	return false;
}
Shape* Shape::globalHitTest(short x, short y) {
	if ( x>this->globalx+this->offsetPos.x &&
		 y>this->globaly+this->offsetPos.y &&
		 x<this->globalx+this->offsetPos.x+this->width &&
		 y<this->globaly+this->offsetPos.y+this->height) {
		return this;
	}
	return NULL;
}
int Shape::callEvent(EventMouseShape* event) {
	event->shape = this;
	event->localx = event->globalx-this->globalx;
	event->localy = event->globaly-this->globaly;
	for(int i=0, s=this->eventList.size(); i<s; i++) {
		if (this->eventList[i].type == event->type) {
			this->eventList[i].fun(event);
		}
	}
}
int Shape::addEventHandler( int type, void(*fun)(const EventMouseShape*)) {
	EventLinc el;
	el.obj = this;
	el.type = type;
	el.fun = (void(*)(const Event*))fun;
	this->eventList.push_back( el );
	this->mouseEventActive = true;
	Shape *sh = this;
	while (sh->parent != NULL) {
		sh = sh->parent;
		sh->mouseEventActive = true;
	}
}

Directory::Directory() :Shape(Directory::CRC32), Buffer() {
	this->shapeCache = NULL;
	this->totalShape = this->totalDir = 0;
	this->parent = NULL;
}
Directory::Directory(int crc32) :Shape(crc32), Buffer() {
	this->shapeCache = NULL;
	this->totalShape = this->totalDir = 0;
	this->parent = NULL;
}
void Directory::trace() {
	printf("<Directory mouseActive='%i' pos='%i, %i' gpos='%i, %i' rect='%i, %i, %i, %i'>\n", this->mouseEventActive, this->x, this->y, this->globalx, this->globaly, this->offsetPos.x, this->offsetPos.y, this->width, this->height);
	for (int i=0; i<this->child.size(); i++) this->child[i]->trace(); 
	printf("</Directory>\n");
}
int Directory::renderGLComptAll() {
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

	glLineWidth(1);
	glColor4ub(0xFF,0,0,0xFF);
	glBegin(GL_LINE_STRIP);// <editor-fold defaultstate="collapsed" desc="GL_LINE_STRIP">
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y );
		glVertex2s( this->globalx+this->offsetPos.x+this->width, this->globaly+this->offsetPos.y );
		glVertex2s( this->globalx+this->offsetPos.x+this->width, this->globaly+this->offsetPos.y+this->height );
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y+this->height );
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y );
	glEnd();// </editor-fold>
	return true;
}
int Directory::renderGL400() {
	printf("Directory p\n");
	for (int i=0; i<this->child.size(); i++) {
		this->child[i]->trace();
		this->child[i]->renderGL400();
	}
	return true;
}
int Directory::renderGL330() {
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
int Directory::renderGL210() {
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
int Directory::bufferGLComptAll() {
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
int Directory::bufferGL400() {
	return false;
}
int Directory::bufferGL330() {
	return false;
}
int Directory::bufferGL210() {
	return false;
}
bool Directory::switchOn() {
	if (!this->status) {
		this->tex = new Texture(this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE);
		root.window->FBOBuffer.push_back(this);
		status = true;
		success = false;
	}
	return false;
}
void Directory::updateGlobalPosition() {
	if (this->parent==NULL) {
		this->globalx = this->globaly = 0;
	}else{
		this->globalx = this->parent->globalx + this->x;
		this->globaly = this->parent->globaly + this->y;
	}
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->updateGlobalPosition();
	}
}
void Directory::drag(short x, short y) {
	short nx, ny;
	this->x = x;
	this->y = y;
	if ( this->parent!=NULL || this==root.window->root) {
		this->updateGlobalPosition();
		if (this->x+this->offsetPos.x < this->parent->offsetPos.x) {
			nx = this->x+this->offsetPos.x;
			this->width -= nx-this->parent->offsetPos.x;
			this->offsetPos.x = nx;
		}
		if (this->y+this->offsetPos.y < this->parent->offsetPos.y) {
			ny = this->y+this->offsetPos.y;
			this->parent->height -= ny-this->parent->offsetPos.y;
			this->parent->offsetPos.y = ny;
		}
		if (this->x+this->offsetPos.x+this->width > this->parent->offsetPos.x+this->parent->width) {
			this->parent->width = this->x+this->offsetPos.x+this->width-this->parent->offsetPos.x;
		}
		if (this->y+this->offsetPos.y+this->height-this->parent->offsetPos.y > this->parent->height) {
			this->parent->height = this->y+this->offsetPos.y+this->height-this->parent->offsetPos.y;
		}
	}
	root.window->renderComplete = false;
}
vector<Shape*>* Directory::getChildShape() {
	
	vector<Shape*>* arr = new vector<Shape*>;
	for (int i = 0; i<this->child.size( ); i++) {
		if (dynamic_cast<Directory*>(this->child[i]) != NULL) {
			((Directory*)(this->child[i]))->getChildShape(arr);
		}else{
			arr->push_back( this->child[i] );
		}
	}
	return arr;
}
void Directory::getChildShape(vector<Shape*>* arr) {
	Directory *dir;
	for (int i = 0; i<this->child.size( ); i++) {
		dir = dynamic_cast<Directory*>(this->child[i]);
		if (dir == NULL) {
			arr->push_back( this->child[i] );
		}else{
			dir->getChildShape(arr);
		}
	}
}
void Directory::addChild(Shape *sh) {
	Directory *dir; 
	short nx, ny;
	sh->parent = this;
	//	y	by	bh	outB
	//	50	-30	70	20/90
	// by	bh	outB
	// -50	100	ob.h-by
	if (sh->mouseEventActive) {
		this->mouseEventActive = true;
		Shape *shape = this;
		while (shape->parent != NULL) {
			shape = shape->parent;
			shape->mouseEventActive = true;
		}
	}
	if (this->parent!=NULL || this==root.window->root) sh->updateGlobalPosition();
	
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
	dir = dynamic_cast<Directory*>(sh);
	if ( dir!=NULL ) {
		while(dir->parent!=NULL) {
			dir = dir->parent;
			dir->totalShape += ((Directory*)sh)->totalShape;
			dir->totalDir += ((Directory*)sh)->totalDir+1;
		}
	}else{
		dir = this;
		dir->totalShape++;
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
void Directory::setBuffer(Directory::BUFFER_TYPE type, char val) {
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
Shape* Directory::globalHitTest(short x, short y) {
	Shape* sh;
	for(int i=this->child.size()-1; i>=0; i--) {
		sh = this->child[i]->globalHitTest(x, y);
		if (sh != NULL) return sh;
	}
	return NULL;
}
int Directory::callEvent(EventMouseShape* event) {
	EventMouseShape* eventRollOver;
	event->shape = this;
	event->localx = event->globalx - this->globalx;
	event->localy = event->globaly - this->globaly;
	Shape *sh;
	for(int i=0, s=this->eventList.size(); i<s; i++) {
		if (this->eventList[i].type == event->type) {
			this->eventList[i].fun(event);
		}
	}
	for(int i=this->child.size()-1; i>=0; i--) {
		if (this->child[i]->mouseEventActive) {
			sh = this->child[i];
			if (event->localx>sh->x+sh->offsetPos.x &&
				event->localy>sh->y+sh->offsetPos.y &&
				event->localx<sh->x+sh->offsetPos.x+sh->width &&
				event->localy<sh->y+sh->offsetPos.y+sh->height) {
				if (!sh->mouseEventRollOver) {
					sh->mouseEventRollOver = true;
					eventRollOver = new EventMouseShape();
					memcpy(eventRollOver, event, sizeof(EventMouseShape));
					eventRollOver->type = EventMouseShape::MOUSE_ROLL_OVER;
					sh->callEvent(eventRollOver);
				}
				sh->callEvent(event);
				break;
			}else if (sh->mouseEventRollOver) {
				sh->mouseEventRollOver = false;
				eventRollOver = new EventMouseShape();
				memcpy(eventRollOver, event, sizeof(EventMouseShape));
				eventRollOver->type = EventMouseShape::MOUSE_ROLL_OUT;
				sh->callEvent(eventRollOver);
			}
		}
	}
}

Bitmap::Bitmap(Texture *tex) :Shape(Bitmap::CRC32) {
	this->tex = tex;
	if (tex->type!=0 && tex->format!=0) {
		this->width = tex->width;
		this->height = tex->height;
	}else{
		root.window->bitmapUpdateBuffer.push_back(this);
	}
}
void Bitmap::trace() {
	printf("<Bitmap a='%i' x='%i' y='%i' gx='%i' gy='%i' w='%i' h='%i' texId='%i'/>\n", this->mouseEventActive, this->x, this->y, this->globalx, this->globaly, this->width, this->height, this->tex);
}
int Bitmap::renderGLComptAll() {
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
int Bitmap::renderGL400() {
	return false;
}
int Bitmap::renderGL330() {
	//glEnable( GL_TEXTURE_2D );
	printf("renderGL330\n");
	short vertex[12] = {
		(short)(this->globalx),				(short)(this->globaly),
		(short)(this->globalx+this->width),	(short)(this->globaly),
		(short)(this->globalx+this->width),	(short)(this->globaly+this->height),
		(short)(this->globalx),				(short)(this->globaly),
		(short)(this->globalx+this->width),	(short)(this->globaly+this->height),
		(short)(this->globalx),				(short)(this->globaly+this->height)
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
int Bitmap::renderGL210() {
	if (this->tex->GLID == 0) return false;
	printf("renderGL210 \n");
	unsigned char faces[4] = {0, 1, 2, 3};
	short texCoord[8] = { 0, 0, 0, 1, 1, 1, 1, 0};
	short vexCoord[8] = { 
		(short)(this->globalx),				(short)(this->globaly),
		(short)(this->globalx),				(short)(this->globaly+this->height),
		(short)(this->globalx+this->width),	(short)(this->globaly+this->height),
		(short)(this->globalx+this->width),	(short)(this->globaly)
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

Buffer::Buffer() {
	this->success = false;
	this->status = false;
	this->tex = NULL;
}
bool Buffer::switchOn() {
	if (!this->status) {
		root.window->FBOBuffer.push_back(this);
		status = true;
		success = false;
	}
	return false;
}
bool Buffer::switchOff() {
	return false;
}
int Buffer::bufferGLComptAll() {
	return false;
}
int Buffer::bufferGL400() {
	return false;
}
int Buffer::bufferGL330() {
	return false;
}
int Buffer::bufferGL210() {
	return false;
}

FPoint::FPoint(int rad, uint32_t color=0 ) :Shape(FPoint::CRC32) {
	this->radius = rad;
	this->color.color = color;
}
int FPoint::renderGLComptAll() {
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
FLines::FLines(void *arr, short length, short w, unsigned int color=0) :Shape(FLines::CRC32) {
	this->arr = (short*)arr;
	this->length = length;
	this->lineWidth = w;
	this->color.color = color;
}
int FLines::renderGLComptAll() {
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

FRect::FRect(short width, short height, uint32_t backgroundColor) :Shape(FRect::CRC32) {
	this->width = width;
	this->height = height;
	this->radius = 0;
	this->borderSize = 0;
	this->borderColor.color = 0;
	this->background = true;
	this->backgroundColor.color = backgroundColor;
}
FRect::FRect(short width, short height, uint32_t borderColor, unsigned short borderSize) :Shape(FRect::CRC32) {
	this->width = width;
	this->height = height;
	this->radius = 0;
	this->borderSize = borderSize;
	this->borderColor.color = borderColor;
	this->background = false;
	this->backgroundColor.color = 0;
}
FRect::FRect(short width, short height, uint32_t backgroundColor, uint32_t borderColor, unsigned short borderSize) :Shape(FRect::CRC32) {
	this->width = width;
	this->height = height;
	this->radius = 0;
	this->borderSize = borderSize;
	this->borderColor.color = borderColor;
	this->background = true;
	this->backgroundColor.color = backgroundColor;
}
int FRect::renderGLComptAll() {
	glPushMatrix();
	if (this->background) {
		if (this->radius>0) {
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			glLineWidth(1);
			glBegin(GL_QUADS);
				glColor3ub(this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b );
				glVertex2s(this->globalx+this->radius,				this->globaly);
				glVertex2s(this->globalx+this->width-this->radius,	this->globaly);
				glVertex2s(this->globalx+this->width-this->radius,	this->globaly+this->height);
				glVertex2s(this->globalx+this->radius,				this->globaly+this->height);
				
				glVertex2s(this->globalx,				this->globaly+this->radius);
				glVertex2s(this->globalx+this->width,	this->globaly+this->radius);
				glVertex2s(this->globalx+this->width,	this->globaly+this->height-this->radius);
				glVertex2s(this->globalx,				this->globaly+this->height-this->radius);
			glEnd();
			glPointSize(this->radius*2);
			glBegin(GL_POINTS);
				glColor3ub(this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b );
				glVertex2s(this->globalx+this->radius, this->globaly+this->radius);
				glVertex2s(this->globalx+this->width-this->radius, this->globaly+this->radius);
				glVertex2s(this->globalx+this->width-this->radius, this->globaly+this->height-this->radius);
				glVertex2s(this->globalx+this->radius, this->globaly+this->height-this->radius);
			glEnd();
		}else{
			glBegin(GL_QUADS);
				glColor3ub(this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b );
				glVertex2s(this->globalx, this->globaly);
				glVertex2s(this->globalx+this->width, this->globaly);
				glVertex2s(this->globalx+this->width, this->globaly+this->height);
				glVertex2s(this->globalx, this->globaly+this->height);
			glEnd();
		}
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