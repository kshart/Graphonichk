#include "grBaseTypes.h"
#include "grShape.h"
#include "UI/UIMain.h"
#include "grProcessingQueue.h"
using namespace std;
using namespace Graphonichk;


vector<Bitmap*> Bitmap::updateBuffer;


ShapeMatrix2D::ShapeMatrix2D() :ShapeBasic(0) {
	
}

ShapeBasic::ShapeBasic(int crc32) :crc32(crc32) {
	
}
ShapeBasic::ShapeBasic() :crc32(0) {
	
}
void ShapeBasic::trace() {
	printf("<ShapeBasic empty/>\n");
}
int ShapeBasic::renderGLComptAll() {
	return false;
}
int ShapeBasic::renderGL400() {
	return false;
}
int ShapeBasic::renderGL330() {
	return false;
}
int ShapeBasic::renderGL210() {
	return false;
}

ShapeGroupBasic::ShapeGroupBasic(int crc32) :ShapeBasic(crc32) {
	
}
ShapeGroupBasic::ShapeGroupBasic() :ShapeBasic(1) {
	
}
void ShapeGroupBasic::trace() {
	printf("<ShapeGroupBasic empty/>\n");
}
int ShapeGroupBasic::renderGLComptAll() {
	return false;
}
int ShapeGroupBasic::renderGL400() {
	return false;
}
int ShapeGroupBasic::renderGL330() {
	return false;
}
int ShapeGroupBasic::renderGL210() {
	return false;
}

ShapeGroupMatrix2D::ShapeGroupMatrix2D() {
}
ShapeGroupMatrix2D::ShapeGroupMatrix2D(int crc32) {
}
int ShapeGroupMatrix2D::renderGLComptAll() {
	float mat[16] = {
		this->matrix.a,	this->matrix.d,	0,	0,
		this->matrix.b,	this->matrix.e,	0,	0,
		this->matrix.c,	this->matrix.f,	1,	0,
		this->matrix.c,	this->matrix.f,	1,	1};
	glPushMatrix();
	glMultMatrixf(mat);
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->renderGLComptAll();
	}
	glPopMatrix();
	return true;
}
int ShapeGroupMatrix2D::renderGL400() {
	return false;
}
int ShapeGroupMatrix2D::renderGL330() {
	return false;
}
int ShapeGroupMatrix2D::renderGL210() {
	return false;
}

ShapeRectGateMatrix2D::ShapeRectGateMatrix2D() :ShapeRect(0) {
}
int ShapeRectGateMatrix2D::renderGLComptAll() {
	glPushMatrix();
	OpenGL::pushViewport();
	OpenGL::setViewport(this->globalx, this->globaly, this->width, this->height);
	glMultMatrixf(this->view.a);
	this->group.renderGLComptAll();
	OpenGL::popViewport();
	glPopMatrix();
	return true;
}
int ShapeRectGateMatrix2D::renderGL400() {
	return false;
}
int ShapeRectGateMatrix2D::renderGL330() {
	return false;
}
int ShapeRectGateMatrix2D::renderGL210() {
	return false;
}


ShapeRect::ShapeRect(int crc32) {
	this->crc32 = crc32;
	this->mouseEventActive = false;
	this->mouseEventRollOver = false;
	this->visible = true;
	this->parent = NULL;
	this->globalx = 0;
	this->x = 0;
	this->globaly = 0;
	this->y = 0;
	this->width = this->height = 0;
	this->offsetPos.x = this->offsetPos.y = 0;
	this->name.clear();
}
void ShapeRect::trace() {
	printf("ShapeRect a='%i' x=%i, y=%i, gx=%i, gy=%i, w=%i, h=%i\n", this->mouseEventActive, this->x, this->y, this->globalx, this->globaly, this->width, this->height);
}
void ShapeRect::setVisible(bool vis) {
	if (this->parent==NULL) return;
	this->parent->updateRect();
}
void ShapeRect::drag(short x, short y) {
	VBOUpdateTask update;
	update.bufferID = this->meshVBO;
	update.data = this->meshVertex;
	update.dataSize = 24;
	short ny, nx;
	this->x = x;
	this->y = y;
	if (this->parent!=NULL) {
		this->globalx = this->parent->globalx+x;
		this->globaly = this->parent->globaly+y;
		this->parent->updateRect();
	}
	this->meshVertex[0] = (short)(this->globalx);
	this->meshVertex[1] = (short)(this->globaly);
	this->meshVertex[2] = (short)(this->globalx+this->width);
	this->meshVertex[3] = (short)(this->globaly);
	this->meshVertex[4] = (short)(this->globalx+this->width);
	this->meshVertex[5] = (short)(this->globaly+this->height);
	this->meshVertex[6] = (short)(this->globalx);
	this->meshVertex[7] = (short)(this->globaly);
	this->meshVertex[8] = (short)(this->globalx+this->width);
	this->meshVertex[9] = (short)(this->globaly+this->height);
	this->meshVertex[10] = (short)(this->globalx);
	this->meshVertex[11] = (short)(this->globaly+this->height);
	//Windows::window->VBOupdate.addTask(update, 0);
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}
void ShapeRect::updateGlobalPosition() {
	if (this->parent==NULL) {
		this->globalx = this->globaly = 0;
	}else{
		this->globalx = this->parent->globalx + this->x;
		this->globaly = this->parent->globaly + this->y;
	}
}
int ShapeRect::saveAsXML(FILE* str, unsigned short tab) {
	for (int i=0; i<tab; i++) fprintf(str, "\t");
	fprintf(str, "<ShapeRect name='%s' crc32='%i' x='%i' y='%i' width='%i' height='%i' offsetX='%i' offsetY='%i'/>\n",
			this->name.c_str(), this->crc32, this->x, this->y, this->width, this->height, this->offsetPos.x, this->offsetPos.y);
}
ShapeRect* ShapeRect::globalHitTest(short x, short y) {
	if ( x>this->globalx+this->offsetPos.x &&
		 y>this->globaly+this->offsetPos.y &&
		 x<this->globalx+this->offsetPos.x+this->width &&
		 y<this->globaly+this->offsetPos.y+this->height) {
		return this;
	}
	return NULL;
}
int ShapeRect::callEvent(EventMouseShape* event) {
	event->shape = this;
	event->localx = event->globalx-this->globalx;
	event->localy = event->globaly-this->globaly;
	for(int i=0, s=this->eventList.size(); i<s; i++) {
		if (this->eventList[i].type == event->type) {
			this->eventList[i].fun(event);
		}
	}
}
int ShapeRect::addEventHandler( int type, void(*fun)(const EventMouseShape*)) {
	EventLinc el;
	el.obj = this;
	el.type = type;
	el.fun = (void(*)(const Event*))fun;
	this->eventList.push_back( el );
	this->mouseEventActive = true;
	ShapeRect *sh = this;
	while (sh->parent != NULL) {
		sh = sh->parent;
		sh->mouseEventActive = true;
	}
}

ShapeGroupRect::ShapeGroupRect() :ShapeRect(ShapeGroupRect::CRC32), Buffer() {
	this->shapeCache = NULL;
	this->totalShape = this->totalDir = 0;
	this->parent = NULL;
	this->cutTheRect = false;
	this->chengeRect = true;
	this->offsetPos.x = SHRT_MAX;
	this->offsetPos.y = SHRT_MAX;
	this->addChildLock = CreateMutex(NULL, FALSE, NULL);
}
ShapeGroupRect::ShapeGroupRect(int crc32) :ShapeRect(crc32), Buffer() {
	this->shapeCache = NULL;
	this->totalShape = this->totalDir = 0;
	this->parent = NULL;
	this->cutTheRect = false;
	this->chengeRect = true;
	this->addChildLock = CreateMutex(NULL, FALSE, NULL);
}
void ShapeGroupRect::trace() {
	printf("<ShapeGroupRect mouseActive='%i' pos='%i, %i' gpos='%i, %i' rect='%i, %i, %i, %i'>\n", this->mouseEventActive, this->x, this->y, this->globalx, this->globaly, this->offsetPos.x, this->offsetPos.y, this->width, this->height);
	for (int i=0; i<this->child.size(); i++) this->child[i]->trace(); 
	printf("</ShapeGroupRect>\n");
}
int ShapeGroupRect::renderGLComptAll() {
	bool ctr = false;
	if (this->cutTheRect) {
		ctr = true;
		OpenGL::pushViewport();
		OpenGL::setViewport(this->globalx+this->offsetPos.x,
				Windows::window->height-this->globaly+this->offsetPos.y-this->height,
				this->width,
				this->height);
		OpenGL::pushViewMatrix();
		ViewMatrix vm(this->globalx+this->offsetPos.x, this->globalx+this->offsetPos.x+this->width, 
				this->globaly+this->offsetPos.y+this->height, this->globaly+this->offsetPos.y, -1, 1);
		OpenGL::setViewMatrix(vm);
	}
	if (this->bufferInit) {
		Texture *tex = this->bufferTexture;
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
		printf("ShapeGroupRect shapeCache\n");
		#endif
		for (int i=0; i<this->shapeCache->size(); i++) {
			(this->shapeCache->at(i))->renderGLComptAll();
		}
	}else{
		WaitForSingleObject(this->addChildLock, INFINITE);
		for (int i=0; i<this->child.size(); i++) {
			if (this->child[i]->visible) this->child[i]->renderGLComptAll();
		}
		ReleaseMutex(this->addChildLock);
	}
	if (ctr) {
		OpenGL::popViewport();
		OpenGL::popViewMatrix();
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
int ShapeGroupRect::renderGL400() {
	printf("ShapeGroupRect p\n");
	for (int i=0; i<this->child.size(); i++) {
		this->child[i]->trace();
		this->child[i]->renderGL400();
	}
	return true;
}
int ShapeGroupRect::renderGL330() {
	for (int i=0; i<this->child.size(); i++) {
		this->child[i]->renderGL330();
	}
	#ifdef DEBUG
/*	glLineWidth(1);
	glColor4ub(0xFF,0,0,0xFF);
	glBegin(GL_LINE_STRIP);// <editor-fold defaultstate="collapsed" desc="GL_LINE_STRIP">
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y );
		glVertex2s( this->globalx+this->offsetPos.x+this->width, this->globaly+this->offsetPos.y );
		glVertex2s( this->globalx+this->offsetPos.x+this->width, this->globaly+this->offsetPos.y+this->height );
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y+this->height );
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y );
	glEnd();// </editor-fold>*/
	#endif
	return true;
}
int ShapeGroupRect::renderGL210() {
	if ( this->shapeCache != NULL ) {
		#ifdef DEBUG
		printf("ShapeGroupRect shapeCache\n");
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

int ShapeGroupRect::bufferGLComptAll() {
	for(int i=0; i<this->bufChild.size(); i++) {
		this->bufChild[i]->bufferGLComptAll();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, this->bufferFrame);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, this->bufferTexture->GLID, 0);
	
	glViewport( 0, 0, this->width, this->height );
	glLoadIdentity( );
	gluOrtho2D( this->offsetPos.x+this->globalx, this->offsetPos.x+this->globalx+this->width, this->offsetPos.y+this->globaly, this->offsetPos.y+this->globaly+this->height );
	
	glClearColor( 0.1, 0, 0, 0.1 );
	glClear( GL_COLOR_BUFFER_BIT );
	
	this->renderGLComptAll();
	
	
	//glDeleteFramebuffers(1, &root.window->ogl->FBOGL);
	return true;
}
int ShapeGroupRect::bufferGL400() {
	return false;
}
int ShapeGroupRect::bufferGL330() {
	return false;
}
int ShapeGroupRect::bufferGL210() {
	return false;
}
bool ShapeGroupRect::bufferMode(bool mode) {
	if (mode && !this->bufferActivate ) {
		this->bufferTexture = new Texture(this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE);
		Windows::window->FBOBuffer.push_back(this);
		
		this->bufferActivate = true;
		this->bufferInit = false;
	}else if ( !mode && this->bufferActivate ) {
		this->bufferActivate = false;
		delete this->bufferTexture;
		this->bufferTexture = NULL;
	}
	return true;
}
int ShapeGroupRect::saveAsXML(FILE* str, unsigned short tab) {
	for (int i=0; i<tab; i++) fprintf(str, "\t");
	fprintf(str, "<ShapeGroupRect name='%s' crc32='%i' childSize='%i' x='%i' y='%i' width='%i' height='%i' offsetX='%i' offsetY='%i'>\n",
			this->name.c_str(), this->crc32, this->child.size(), this->x, this->y, this->width, this->height, this->offsetPos.x, this->offsetPos.y);
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->saveAsXML(str, tab+1);
	}
	
	for (int i=0; i<tab; i++) fprintf(str, "\t");
	fprintf(str, "</ShapeGroupRect>\n");
}
void ShapeGroupRect::updateGlobalPosition() {
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
void ShapeGroupRect::drag(short x, short y) {
	short nx, ny;
	this->x = x;
	this->y = y;
	if ( this->parent!=NULL || this==Windows::window->root) {
		this->updateGlobalPosition();
		if (this->visible) this->parent->updateRect();
	}
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}
void ShapeGroupRect::updateRect(ShapeRect* sh) {
	if (!this->chengeRect) return;
	short nx, ny;
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
}
void ShapeGroupRect::updateRect() {
	if (this->child.empty() || !this->chengeRect) return;
	short nx, ny;
	ShapeRect* sh;
	int i=0;
	while (i<this->child.size()) {
		if (this->child[i]->visible) break;
	}
	if (i>=this->child.size()) return;
	sh = this->child[i];
	this->offsetPos.x = sh->x+sh->offsetPos.x;
	this->offsetPos.y = sh->y+sh->offsetPos.y;
	this->width = sh->width;
	this->height = sh->height;
	for(i; i<this->child.size(); i++) {
		sh = this->child[i];
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
	}
}
vector<ShapeRect*>* ShapeGroupRect::getChildShape() {
	
	vector<ShapeRect*>* arr = new vector<ShapeRect*>;
	for (int i = 0; i<this->child.size( ); i++) {
		if (dynamic_cast<ShapeGroupRect*>(this->child[i]) != NULL) {
			((ShapeGroupRect*)(this->child[i]))->getChildShape(arr);
		}else{
			arr->push_back( this->child[i] );
		}
	}
	return arr;
}
void ShapeGroupRect::getChildShape(vector<ShapeRect*>* arr) {
	ShapeGroupRect *dir;
	for (int i = 0; i<this->child.size( ); i++) {
		dir = dynamic_cast<ShapeGroupRect*>(this->child[i]);
		if (dir == NULL) {
			arr->push_back( this->child[i] );
		}else{
			dir->getChildShape(arr);
		}
	}
}
bool ShapeGroupRect::addChild(ShapeRect *sh) {
	WaitForSingleObject(this->addChildLock, INFINITE);
	ShapeGroupRect *dir;
	sh->parent = this;
	//	y	by	bh	outB
	//	50	-30	70	20/90
	// by	bh	outB
	// -50	100	ob.h-by
	if (sh->mouseEventActive) {
		this->mouseEventActive = true;
		ShapeRect *shape = this;
		while (shape->parent != NULL) {
			shape = shape->parent;
			shape->mouseEventActive = true;
		}
	}
	if (this->parent!=NULL || this==Windows::window->root) sh->updateGlobalPosition();
	if (this->offsetPos.x==SHRT_MAX||this->offsetPos.y==SHRT_MAX) {
		if (sh->visible) this->updateRect();
	}else if (sh->visible) {
		this->updateRect(sh);
	}
	this->child.push_back( sh );
	dir = dynamic_cast<ShapeGroupRect*>(sh);
	if ( dir!=NULL ) {
		while(dir->parent!=NULL) {
			dir = dir->parent;
			dir->totalShape += ((ShapeGroupRect*)sh)->totalShape;
			dir->totalDir += ((ShapeGroupRect*)sh)->totalDir+1;
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
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
	ReleaseMutex(this->addChildLock);
}
bool ShapeGroupRect::removeChild(ShapeRect* sh) {
	
}
bool ShapeGroupRect::setChildDepth(ShapeRect* sh, unsigned short depth) {
	
}
unsigned short ShapeGroupRect::getChildDepth(ShapeRect* sh) {
	
}
ShapeRect* ShapeGroupRect::getChild(string str) {
	
}
void ShapeGroupRect::setBuffer(ShapeGroupRect::BUFFER_TYPE type, char val) {
	/*switch (type) {
		case ShapeGroupRect::TO_TEXTURE :
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
		case ShapeGroupRect::PREDEFINED_LIST_SH :
			if (val) {
				this->shapeCache = this->getChildShape();
			}else if (this->shapeCache != NULL) {
				delete this->shapeCache;
				this->shapeCache = NULL;
			}
			return;
	}*/
}
ShapeRect* ShapeGroupRect::globalHitTest(short x, short y) {
	ShapeRect* sh;
	for(int i=this->child.size()-1; i>=0; i--) {
		sh = this->child[i]->globalHitTest(x, y);
		if (sh != NULL) return sh;
	}
	return NULL;
}
int ShapeGroupRect::callEvent(EventMouseShape* event) {
	EventMouseShape* eventRollOver;
	event->shape = this;
	event->localx = event->globalx - this->globalx;
	event->localy = event->globaly - this->globaly;
	ShapeRect *sh;
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

Bitmap::Bitmap(Texture *tex) :ShapeRect(Bitmap::CRC32), vao(0) {
	this->tex = tex;
	if (tex->event == Texture::LOADED) {
		this->width = tex->width;
		this->height = tex->height;
	}else{
		Bitmap::updateBuffer.push_back(this);
	}
}
void Bitmap::trace() {
	printf("<Bitmap a='%i' x='%i' y='%i' gx='%i' gy='%i' w='%i' h='%i' texId='%i'/>\n", this->mouseEventActive, this->x, this->y, this->globalx, this->globaly, this->width, this->height, this->tex);
}
int Bitmap::renderGLComptAll() {
	Texture *tex = this->tex;
	//printf("tex->GLID = %i", tex->GLID);
	if (GLShader::shader!=NULL && GLShader::shader->crc32!=1) GLShader::setShader(ShaderBitmap::prog);
	glBindTexture(GL_TEXTURE_2D, tex->GLID);
	glColor4ub(0xFF,0xFF,0xFF,0xFF);
	glEnable( GL_TEXTURE_2D );
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
	if (GLShader::shader->crc32!=ShaderBitmap::CRC32) GLShader::setShader(ShaderBitmap::prog);
	if (OpenGL::textureGLID!=this->tex->GLID) {
		glBindTexture(GL_TEXTURE_2D, this->tex->GLID);
		OpenGL::textureGLID = this->tex->GLID;
	}
	if (this->vao==0 && this->tex->event==Texture::LOADED) {
		short vertex[2] = {
			this->globalx, this->globaly
		};
		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);
		
		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, 4, vertex, GL_STATIC_DRAW);
		glVertexAttribPointer(ShaderBitmap::prog->position, 2, GL_SHORT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(ShaderBitmap::prog->position);
	}
	glBindVertexArray(this->vao);
	glDrawArrays(GL_POINTS, 0, 1);
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
int Bitmap::saveAsXML(FILE* str, unsigned short tab) {
	for (int i=0; i<tab; i++) fprintf(str, "\t");
	fprintf(str, "<Bitmap name='%s' crc32='%i' x='%i' y='%i' width='%i' height='%i' texture='%i'/>\n",
			this->name.c_str(), this->crc32, this->x, this->y, this->width, this->height, this->tex);
}
void Bitmap::updateBitmaps() {
	if ( !Bitmap::updateBuffer.empty() ) {
		Bitmap *bmp;
		int countUpdateBMP = 0;
		for(int i=0; i<Bitmap::updateBuffer.size()-countUpdateBMP; i++) {
			bmp = Bitmap::updateBuffer[i];
			if (bmp->tex->GLID!=0 && bmp->tex->event==Texture::LOADED) {
				bmp->width = bmp->tex->width;
				bmp->height = bmp->tex->height;
				bmp->tex->trace();
				countUpdateBMP++;
				if (countUpdateBMP >= Bitmap::updateBuffer.size()) {
					Bitmap::updateBuffer.clear();
					break;
				}
				Bitmap::updateBuffer[i] = Bitmap::updateBuffer[ Bitmap::updateBuffer.size()-countUpdateBMP ]; 
				Bitmap::updateBuffer[ Bitmap::updateBuffer.size()-countUpdateBMP ] = NULL;
			}
		}
		if (countUpdateBMP < Bitmap::updateBuffer.size()) {
			Bitmap::updateBuffer.resize( Bitmap::updateBuffer.size()-countUpdateBMP );
		}
	}
}
Buffer::Buffer() {
	this->bufferInit = false;
	this->bufferActivate = false;
	this->bufferTexture = NULL;
	this->bufferFrame = 0;
}
bool Buffer::bufferMode(bool mode) {
	/*if (mode && !this->bufferActivate ) {
		root.window->FBOBuffer.push_back(this);
		this->bufferActivate = true;
		this->bufferStatus = false;
	}else if ( !mode && this->bufferActivate ) {
		this->bufferActivate = false;
	}*/
	return false;
}
bool Buffer::bufferUpdate() {
	return false;
}
int Buffer::bufferGLComptAll() {
	return true;
}
int Buffer::bufferGL400() {
	return true;
}
int Buffer::bufferGL330() {
	return true;
}
int Buffer::bufferGL210() {
	return true;
}

FPoint::FPoint(int rad, uint32_t color=0 ) :ShapeRect(FPoint::CRC32) {
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
FLines::FLines(void *arr, short length, short w, unsigned int color=0) :ShapeRect(FLines::CRC32) {
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

FRect::FRect(short width, short height, uint32_t backgroundColor) :ShapeRect(FRect::CRC32) {
	this->width = width;
	this->height = height;
	this->backgroundColor.color = backgroundColor;
}
int FRect::renderGLComptAll() {
	glPushMatrix();
	glBegin(GL_QUADS);
		glColor3ub(this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b );
		glVertex2s(this->globalx, this->globaly);
		glVertex2s(this->globalx+this->width, this->globaly);
		glVertex2s(this->globalx+this->width, this->globaly+this->height);
		glVertex2s(this->globalx, this->globaly+this->height);
	glEnd();
	glPopMatrix();
}
int FRect::renderGL330() {
	//glUseProgram();
	if (GLShader::shader->crc32!=ShaderFPrimitiv::CRC32) GLShader::setShader(ShaderFPrimitiv::prog);
	if (this->vao==0) {
		short vertex[4] = {
			this->globalx, this->globaly, (short)this->width, (short)this->height
		};
		glGenVertexArrays(1, &this->vao);
		if (this->vao==0) printf("VAO_NULL");
		glBindVertexArray(this->vao);
		
		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, 4*2, vertex, GL_STATIC_DRAW);
		glVertexAttribPointer(ShaderBitmap::prog->position, 4, GL_SHORT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(ShaderBitmap::prog->position);
	}
	glBindVertexArray(this->vao);
	//printf("color %i %i %i %i\n", backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	glUniform4f(ShaderFPrimitiv::prog->fillColor, 
			backgroundColor.r/255.0, 
			backgroundColor.g/255.0, 
			backgroundColor.b/255.0, 
			backgroundColor.a/255.0);
	glDrawArrays(GL_POINTS, 0, 1);
	return true;
}