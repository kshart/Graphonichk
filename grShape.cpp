#include "grMain.h"
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
void ShapeBasic::trace() {
	fputs("<ShapeBasic empty/>\n", ioshp);
	fputs("ShapeBasic::trace\n", iovir);
}
int ShapeBasic::renderGL400() {
	fputs("ShapeBasic::renderGL400\n", iovir);
	return false;
}
int ShapeBasic::renderGL330() {
	fputs("ShapeBasic::renderGL330\n", iovir);
	return false;
}
int ShapeBasic::renderGL210() {
	fputs("ShapeBasic::renderGL210\n", iovir);
	return false;
}
int ShapeBasic::renderGL100() {
	fputs("ShapeBasic::renderGL100\n", iovir);
	return false;
}

ShapeGroupBasic::ShapeGroupBasic(int crc32) :ShapeBasic(crc32) {
	
}
ShapeGroupBasic::ShapeGroupBasic() :ShapeBasic(1) {
	
}
void ShapeGroupBasic::trace() {
	printf("<ShapeGroupBasic empty/>\n");
	fputs("ShapeGroupBasic::trace\n", iovir);
}
int ShapeGroupBasic::renderGL400() {
	fputs("ShapeGroupBasic::renderGL400\n", iovir);
	return false;
}
int ShapeGroupBasic::renderGL330() {
	fputs("ShapeGroupBasic::renderGL330\n", iovir);
	return false;
}
int ShapeGroupBasic::renderGL210() {
	fputs("ShapeGroupBasic::renderGL210\n", iovir);
	return false;
}
int ShapeGroupBasic::renderGL100() {
	fputs("ShapeGroupBasic::renderGL100\n", iovir);
	return false;
}

ShapeGroupMatrix2D::ShapeGroupMatrix2D() {
}
ShapeGroupMatrix2D::ShapeGroupMatrix2D(int crc32) {
}
int ShapeGroupMatrix2D::renderGL400() {
	fputs("ShapeGroupMatrix2D::renderGL400\n", iovir);
	return false;
}
int ShapeGroupMatrix2D::renderGL330() {
	fputs("ShapeGroupMatrix2D::renderGL330\n", iovir);
	return false;
}
int ShapeGroupMatrix2D::renderGL210() {
	fputs("ShapeGroupMatrix2D::renderGL210\n", iovir);
	return false;
}
int ShapeGroupMatrix2D::renderGL100() {
	float mat[16] = {
		this->matrix.a,	this->matrix.d,	0,	0,
		this->matrix.b,	this->matrix.e,	0,	0,
		0,				0,				1,	0,
		this->matrix.c,	this->matrix.f,	1,	1};
	glPushMatrix();
	glMultMatrixf(mat);
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->renderGL100();
	}
	glPopMatrix();
	return true;
}

ShapeRectGateMatrix2D::ShapeRectGateMatrix2D() :ShapeRect(0) {
}
int ShapeRectGateMatrix2D::renderGL100() {
	glPushMatrix();
	OpenGL::pushViewport();
	OpenGL::setViewport(this->getGlobalX(), this->getGlobalY(), this->getWidth(), this->getHeight());
	glMultMatrixf(this->view.a);
	this->group.renderGL100();
	OpenGL::popViewport();
	glPopMatrix();
	return true;
}
int ShapeRectGateMatrix2D::renderGL400() {
	fputs("ShapeRectGateMatrix2D::renderGL400\n", iovir);
	return false;
}
int ShapeRectGateMatrix2D::renderGL330() {
	fputs("ShapeRectGateMatrix2D::renderGL330\n", iovir);
	return false;
}
int ShapeRectGateMatrix2D::renderGL210() {
	fputs("ShapeRectGateMatrix2D::renderGL210\n", iovir);
	return false;
}


ShapeRect::ShapeRect(int crc32) :ShapeBasic(crc32), parent(nullptr) {
	this->mouseEventActive = false;
	this->mouseEventRollOver = false;
	this->visible = true;
	this->local.x = this->local.y = 0;
	this->global.x = this->global.y = 0;
	this->width = this->height = 0;
	this->offset.x = this->offset.y = 0;
	this->vao = 0;
	this->vboRect = 0;
	ShapeRectTask::task.addTask(this);
}
void ShapeRect::setPosition(short x, short y) {
	this->local.x = x;
	this->local.y = y;
	if (this->parent!=NULL) {
		this->global.x = this->parent->global.x+x;
		this->global.y = this->parent->global.y+y;
		this->parent->updateRect();
	}
	ShapeRectTask::task.addTask(this);
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}
void ShapeRect::setOffset(short x, short y) {
	this->offset.x = x;
	this->offset.y = y;
	if (this->parent!=nullptr) this->parent->updateRect();
	ShapeRectTask::task.addTask(this);
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}
void ShapeRect::setBox(short x, short y, short w, short h) {
	this->local.x = x;
	this->local.y = y;
	this->width = w;
	this->height = h;
	if (this->parent!=nullptr) this->parent->updateRect();
	ShapeRectTask::task.addTask(this);
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}
void ShapeRect::setRect(short w, short h) {
	this->width = w;
	this->height = h;
	if (this->parent!=nullptr) this->parent->updateRect();
	ShapeRectTask::task.addTask(this);
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}
void ShapeRect::setVisible(bool vis) {
	this->visible = vis;
	if (this->parent!=nullptr) this->parent->updateRect();
	
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}
short ShapeRect::getGlobalX() const {
	return this->global.x;
}
short ShapeRect::getGlobalY() const {
	return this->global.y;
}
svec2 ShapeRect::getGlobalPosition() const {
	return this->global;
}
short ShapeRect::getX() const {
	return this->local.x;
}
short ShapeRect::getY() const {
	return this->local.y;
}
svec2 ShapeRect::getLocalPosition() const {
	return this->local;
}
short ShapeRect::getOffsetX() const {
	return this->offset.x;
}
short ShapeRect::getOffsetY() const {
	return this->offset.y;
}
svec2 ShapeRect::getOffsetPosition() const {
	return this->offset;
}
short ShapeRect::getWidth() const {
	return (short)this->width;
}
short ShapeRect::getHeight() const {
	return (short)this->height;
}
svec2 ShapeRect::getRect() const {
	svec2 v;
	v.x = this->width;
	v.y = this->height;
	return v;
}
svec4 ShapeRect::getBox() const {
	return svec4(global.x+offset.x, global.y+offset.y, width, height);
}
void ShapeRect::trace() {
	printf("ShapeRect a='%i' x=%i, y=%i, gx=%i, gy=%i, w=%i, h=%i\n", this->mouseEventActive, this->local.x, this->local.y, this->global.x, this->global.y, this->width, this->height);
}
void ShapeRect::updateGlobalPosition() {
	if (this->parent==NULL) {
		this->global.x = this->global.y = 0;
	}else{
		this->global.x = this->parent->global.x + this->local.x;
		this->global.y = this->parent->global.y + this->local.y;
	}
	ShapeRectTask::task.addTask(this);
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}
int ShapeRect::saveAsXML(FILE* str, unsigned short tab) {
	for (int i=0; i<tab; i++) fprintf(str, "\t");
	fprintf(str, "<ShapeRect name='%s' crc32='%i' x='%i' y='%i' width='%i' height='%i' offsetX='%i' offsetY='%i'/>\n",
			this->name.c_str(), this->crc32, this->local.x, this->local.y, this->width, this->height, this->offset.x, this->offset.y);
	return true;
}
ShapeRect* ShapeRect::globalHitTest(short x, short y) {
	if ( x>this->global.x+this->offset.x &&
		 y>this->global.y+this->offset.y &&
		 x<this->global.x+this->offset.x+this->width &&
		 y<this->global.y+this->offset.y+this->height) {
		return this;
	}
	return NULL;
}
int ShapeRect::callEvent(EventMouseShape* event) {
	event->shape = this;
	event->localx = event->globalx-this->global.x;
	event->localy = event->globaly-this->global.y;
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
	while (sh->parent != nullptr) {
		sh = sh->parent;
		sh->mouseEventActive = true;
	}
}

ShapeGroupRect::ShapeGroupRect() :ShapeRect(ShapeGroupRect::CRC32) {
	this->shapeCache = NULL;
	this->totalShape = this->totalDir = 0;
	this->parent = NULL;
	this->cutTheRect = false;
	this->chengeRect = true;
	this->offset.x = SHRT_MAX;
	this->offset.y = SHRT_MAX;
	this->addChildLock = CreateMutex(NULL, FALSE, NULL);
}
ShapeGroupRect::ShapeGroupRect(int crc32) :ShapeRect(crc32) {
	this->shapeCache = NULL;
	this->totalShape = this->totalDir = 0;
	this->parent = NULL;
	this->cutTheRect = false;
	this->chengeRect = true;
	this->addChildLock = CreateMutex(NULL, FALSE, NULL);
}
void ShapeGroupRect::trace() {
	printf("<ShapeGroupRect mouseActive='%i' pos='%i, %i' gpos='%i, %i' rect='%i, %i, %i, %i'>\n", this->mouseEventActive, this->local.x, this->local.y, this->global.x, this->global.y, this->offset.x, this->offset.y, this->width, this->height);
	for (int i=0; i<this->child.size(); i++) this->child[i]->trace(); 
	printf("</ShapeGroupRect>\n");
}
int ShapeGroupRect::renderGL400() {
	printf("ShapeGroupRect p\n");
	for (int i=0; i<this->child.size(); i++) {
		this->child[i]->trace();
		if (this->child[i]->visible)this->child[i]->renderGL400();
	}
	return true;
}
int ShapeGroupRect::renderGL330() {
	for (ShapeRect* &sh : this->child) {
		if (sh->visible) sh->renderGL330();
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
int ShapeGroupRect::renderGL100() {
	bool ctr = false;
	/*if (this->cutTheRect) {
		ctr = true;
		OpenGL::pushViewport();
		OpenGL::setViewport(this->globalx+this->offsetPos.x,
				Windows::window->height-this->globaly+this->offsetPos.y-this->height,
				this->width,
				this->height);
		OpenGL::pushViewMatrix();
		ViewMatrix vm = ViewMatrixOrtho(this->globalx+this->offsetPos.x, this->globalx+this->offsetPos.x+this->width, 
				this->globaly+this->offsetPos.y+this->height, this->globaly+this->offsetPos.y, -1, 1);
		OpenGL::setViewMatrix(vm);
	}*/
	if (this->bufferInit) {
		/*Texture *tex = this->bufferTexture;
		glEnable( GL_TEXTURE_2D );
		glBindTexture(GL_TEXTURE_2D, tex->GLID);
		glColor4ub(0xFF,0xFF,0xFF,0xFF);
		glBegin( GL_QUADS );// <editor-fold defaultstate="collapsed" desc="GL_QUADS">
			glTexCoord2d( 0.0, 0.0 );	glVertex2i(this->globalx+offsetPos.x, this->globaly+offsetPos.y );
			glTexCoord2d( 0.0, 1.0 );	glVertex2i(this->globalx+offsetPos.x, this->globaly+offsetPos.y+tex->height );
			glTexCoord2d( 1.0, 1.0 );	glVertex2i(this->globalx+offsetPos.x+tex->width, this->globaly+offsetPos.y+tex->height );
			glTexCoord2d( 1.0, 0.0 );	glVertex2i(this->globalx+offsetPos.x+tex->width, this->globaly+offsetPos.y );
		glEnd();// </editor-fold>
		glDisable( GL_TEXTURE_2D );*/
	}else if ( this->shapeCache != nullptr ) {
		#ifdef DEBUG
		printf("ShapeGroupRect shapeCache\n");
		#endif
		/*for (int i=0; i<this->shapeCache->size(); i++) {
			(this->shapeCache->at(i))->renderGL100();
		}*/
	}else{
		//WaitForSingleObject(this->addChildLock, INFINITE);
		for (ShapeRect* &sh : this->child) {
			if (sh->visible) sh->renderGL100();
		}
		//ReleaseMutex(this->addChildLock);
	}
	if (ctr) {
		//OpenGL::popViewport();
		//OpenGL::popViewMatrix();
	}
	glLineWidth(1);
	glColor4ub(0xFF,0,0,0xFF);
	glBegin(GL_LINE_STRIP);// <editor-fold defaultstate="collapsed" desc="GL_LINE_STRIP">
		glVertex2s( this->global.x+this->offset.x,				this->global.y+this->offset.y );
		glVertex2s( this->global.x+this->offset.x+this->width,	this->global.y+this->offset.y );
		glVertex2s( this->global.x+this->offset.x+this->width,	this->global.y+this->offset.y+this->height );
		glVertex2s( this->global.x+this->offset.x,				this->global.y+this->offset.y+this->height );
		glVertex2s( this->global.x+this->offset.x,				this->global.y+this->offset.y );
	glEnd();// </editor-fold>
	
	return false;
}
/*int ShapeGroupRect::saveAsXML(FILE* str, unsigned short tab) {
	for (int i=0; i<tab; i++) fprintf(str, "\t");
	fprintf(str, "<ShapeGroupRect name='%s' crc32='%i' childSize='%i' x='%i' y='%i' width='%i' height='%i' offsetX='%i' offsetY='%i'>\n",
			this->name.c_str(), this->crc32, this->child.size(), this->local.x, this->local.y, this->width, this->height, this->offset.x, this->offset.y);
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->saveAsXML(str, tab+1);
	}
	
	for (int i=0; i<tab; i++) fprintf(str, "\t");
	fprintf(str, "</ShapeGroupRect>\n");
}*/
void ShapeGroupRect::setPosition(short x, short y) {
	this->local.x = x;
	this->local.y = y;
	if ( this->parent!=nullptr || this==Windows::window->root) {
		this->updateGlobalPosition();
		if (this->visible) this->parent->updateRect();
	}
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}
void ShapeGroupRect::updateGlobalPosition() {
	if (this->parent==nullptr) {
		this->global.x = this->global.y = 0;
	}else{
		this->global.x = this->parent->global.x + this->local.x;
		this->global.y = this->parent->global.y + this->local.y;
	}
	for(ShapeRect* &sh : this->child) {
		sh->updateGlobalPosition();
	}
}
void ShapeGroupRect::updateRect() {
	//puts("ShapeGroupRect::updateRect");
	if (this->child.empty() || !this->chengeRect) return;
	svec2 newOffset, rect, offset, SHpos, SHoffset, SHrect;
	ShapeRect* sh;
	int i;
	for(i=0; i<this->child.size(); i++) if (this->child[i]->visible) break;
	if (i>=this->child.size()) return;
	
	sh = this->child[i];
	SHpos = sh->getLocalPosition();
	SHoffset = sh->getOffsetPosition();
	offset.x = SHpos.x+SHoffset.x;
	offset.y = SHpos.y+SHoffset.y;
	rect = sh->getRect();
	for(; i<this->child.size(); i++) {
		sh = this->child[i];
		if (!sh->visible) continue;
		SHpos = sh->getLocalPosition();
		SHoffset = sh->getOffsetPosition();
		SHrect = sh->getRect();
		if (SHpos.x+SHoffset.x < offset.x) {
			newOffset.x = SHpos.x+SHoffset.x;
			rect.x -= newOffset.x-offset.x;
			offset.x = newOffset.x;
		}
		if (SHpos.y+SHoffset.y < offset.y) {
			newOffset.y = SHpos.y+SHoffset.y;
			rect.y -= newOffset.y-offset.y;
			offset.y = newOffset.y;
		}
		if (SHpos.x+SHoffset.x+SHrect.x > offset.x+rect.x) {
			rect.x = SHpos.x+SHoffset.x+SHrect.x-offset.x;
		}
		if (SHpos.y+SHoffset.y+SHrect.y-offset.y > rect.y) {
			rect.y = SHpos.y+SHoffset.y+SHrect.y-offset.y;
		}
	}
	this->setOffset(offset.x, offset.y);
	this->setRect(rect.x, rect.y);
}
vector<ShapeRect*>* ShapeGroupRect::getChildShape() {
	
	vector<ShapeRect*>* arr = new vector<ShapeRect*>;
	for (ShapeRect* &sh : this->child) {
		if (dynamic_cast<ShapeGroupRect*>(sh) != nullptr) {
			((ShapeGroupRect*)sh)->getChildShape(arr);
		}else{
			arr->push_back(sh);
		}
	}
	return arr;
}
void ShapeGroupRect::getChildShape(vector<ShapeRect*>* arr) {
	ShapeGroupRect *dir;
	for (ShapeRect* &it : this->child) {
		dir = dynamic_cast<ShapeGroupRect*>(it);
		if (dir == nullptr) {
			arr->push_back( it );
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
	if (this->getOffsetX()==SHRT_MAX||this->getOffsetY()==SHRT_MAX) {
		if (sh->visible) this->updateRect();
	}else if (sh->visible) {
		this->updateRect();
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
	return true;
}
bool ShapeGroupRect::removeChild(const ShapeRect* sh) {
	for (auto it=this->child.begin(), end=this->child.end(); it!=end; ++it) {
		if ((*it)==sh){
			this->child.erase(it);
			return true;
		}
	}
	return false;
}
bool ShapeGroupRect::setChildDepth(ShapeRect* sh, unsigned short depth) {
	return false;
}
unsigned int ShapeGroupRect::getChildDepth(ShapeRect* sh) const {
	for (auto it=this->child.begin(), end=this->child.end(); it!=end; ++it) {
		if ( (*it)==sh ) return it-this->child.begin();
	}
	return INT_MAX;
}
ShapeRect* ShapeGroupRect::getChild(string str) const {
	return nullptr;
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
	EventMouseShape* eventRollOver = nullptr;
	event->shape = this;
	event->localx = event->globalx - this->getGlobalX();
	event->localy = event->globaly - this->getGlobalY();
	ShapeRect *sh;
	for(int i=0, s=this->eventList.size(); i<s; i++) {
		if (this->eventList[i].type == event->type) {
			this->eventList[i].fun(event);
		}
	}
	for(int i=this->child.size()-1; i>=0; i--) {
		if (this->child[i]->mouseEventActive) {
			sh = this->child[i];
			if (event->localx>sh->getX()+sh->getOffsetX() &&
				event->localy>sh->getY()+sh->getOffsetY() &&
				event->localx<sh->getX()+sh->getOffsetX()+sh->getWidth() &&
				event->localy<sh->getY()+sh->getOffsetY()+sh->getHeight()) {
				if (!sh->mouseEventRollOver) {
					sh->mouseEventRollOver = true;
					eventRollOver = new EventMouseShape( EventMouseShape::MOUSE_ROLL_OVER );
					memcpy(eventRollOver, event, sizeof(EventMouseShape));
					eventRollOver->type = EventMouseShape::MOUSE_ROLL_OVER;
					sh->callEvent(eventRollOver);
					delete eventRollOver;
				}
				sh->callEvent(event);
				break;
			}else if (sh->mouseEventRollOver) {
				sh->mouseEventRollOver = false;
				eventRollOver = new EventMouseShape( EventMouseShape::MOUSE_ROLL_OVER );
				memcpy(eventRollOver, event, sizeof(EventMouseShape));
				eventRollOver->type = EventMouseShape::MOUSE_ROLL_OUT;
				sh->callEvent(eventRollOver);
				delete eventRollOver;
			}
		}
	}
	return true;
}

ShapeMain::ShapeMain() :ShapeGroupRect(ShapeMain::CRC32) {
	this->frameBuffer.color = new Texture(this->width, this->height, GL_RGB, GL_UNSIGNED_BYTE);
	this->frameBuffer.depth = new Texture(this->width, this->height, GL_DEPTH_COMPONENT, GL_FLOAT);
	if (OpenGL::ver!=OpenGL::VER_CORE_100) Windows::window->eachFrame.addTask(new ShapeMainInitTask(this));
}
void ShapeMain::setRect(short w, short h) {
	this->ShapeGroupRect::setRect(w, h);
	this->frameBuffer.color->width = w;
	this->frameBuffer.color->height = h;
	this->frameBuffer.depth->width = w;
	this->frameBuffer.depth->height = h;
	if (this->frameBuffer.color->event == Texture::LOADED) {
		this->frameBuffer.color->event = Texture::TO_UPDATE;
		ADD_TEXTURE_TO_UPDATE_BUFFER(this->frameBuffer.color);
	}
	if (this->frameBuffer.depth->event == Texture::LOADED) {
		this->frameBuffer.depth->event = Texture::TO_UPDATE;
		ADD_TEXTURE_TO_UPDATE_BUFFER(this->frameBuffer.depth);
	}
}
int ShapeMain::renderGL330() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer.fbo);
	OpenGL::setViewport(0, 0, this->width, this->height);
	glClearColor( 0.9, 0.9, 0.9, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	//glFrontFace(GL_CW);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glHint(GL_FOG_HINT, GL_NICEST);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	//glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_BACK, GL_LINE);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glUseProgram(GLShader::glsl->shaderProgram);
	//printf("GLShader::glsl->shaderProgram %i\n", GLShader::glsl->shaderProgram);
	this->ShapeGroupRect::renderGL330();
	
	glDisable( GL_BLEND );
	
	
	OpenGL::setViewport(0, 0, this->width, this->height);
	glBindVertexArray(this->frameBuffer.vao);
	glBindTexture(GL_TEXTURE_2D, this->frameBuffer.color->GLID);
	if (!this->postEffects.child.empty()) {
		for (auto it=this->postEffects.child.begin(), end=this->postEffects.child.end(); it!=end; ++it) {
			(*it)->renderGL330();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	SET_SHADER(ShaderPost);
	glDrawArrays(GL_POINTS, 0, 1);	
	
	return true;
}
int ShapeMain::renderGL100() {
	OpenGL::setViewport(0, 0, this->width, this->height);
	OpenGL::clearViewMatrix();
	
	glClearColor( 0.9, 0.9, 0.9, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	this->ShapeGroupRect::renderGL100();
	
	glDisable( GL_BLEND );
	return true;
}


ShapePostEffect::ShapePostEffect(GLShader *shader) :ShapeBasic(ShapePostEffect::CRC32), shader(shader) {
	
}
int ShapePostEffect::renderGL330() {
	SET_SHADER_H(this->shader);
	glDrawArrays(GL_POINTS, 0, 1);	
}


ShapeRectTask ShapeRectTask::task;
int ShapeRectTask::processExecute() {
	this->performTasks();
	return false;
}
int ProcessingShapeRect::performTasks() {
	CRITICAL_SECTION_INTER(this->_accessPush);
	this->_queueIsUse = !this->_queueIsUse;
	CRITICAL_SECTION_LEAVE(this->_accessPush);
	if (this->_queueIsUse == 1) {
		while ( !this->_essentialTasks1.empty() ) {
			ShapeRect *shr = this->_essentialTasks1.front();
			if (shr->vao==0) {
				svec4 vertex = shr->getBox();
				glGenVertexArrays(1, &shr->vao);
				glBindVertexArray(shr->vao);
				if (shr->vao==0) {
					this->addTask(this->_essentialTasks1.front());
					continue;
				}
				glGenBuffers(1, &shr->vboRect);
				if (shr->vboRect==0) {
					this->addTask(this->_essentialTasks1.front());
					continue;
				}
				glBindBuffer(GL_ARRAY_BUFFER, shr->vboRect);
				glBufferData(GL_ARRAY_BUFFER, 4*sizeof(short), &vertex, GL_STATIC_DRAW);
				glVertexAttribPointer(ShaderShRect::POSITION, 4, GL_SHORT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(ShaderShRect::POSITION);
			}else{
				svec4 vertex = shr->getBox();
				glBindVertexArray(shr->vao);
				glBindBuffer(GL_ARRAY_BUFFER, shr->vboRect);
				glBufferSubData(GL_ARRAY_BUFFER, 0, 4*sizeof(short), &vertex);
			}
			this->_essentialTasks1.pop();
		}
	}else{
		while ( !this->_essentialTasks2.empty() ) {
			ShapeRect *shr = this->_essentialTasks2.front();
			if (shr->vao==0) {
				svec4 vertex = shr->getBox();
				glGenVertexArrays(1, &shr->vao);
				glBindVertexArray(shr->vao);
				if (shr->vao==0) {
					this->addTask(this->_essentialTasks2.front());
					continue;
				}
				glGenBuffers(1, &shr->vboRect);
				if (shr->vboRect==0) {
					this->addTask(this->_essentialTasks2.front());
					continue;
				}
				glBindBuffer(GL_ARRAY_BUFFER, shr->vboRect);
				glBufferData(GL_ARRAY_BUFFER, 4*sizeof(short), &vertex, GL_STATIC_DRAW);
				glVertexAttribPointer(ShaderShRect::POSITION, 4, GL_SHORT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(ShaderShRect::POSITION);
			}else{
				svec4 vertex = shr->getBox();
				glBindVertexArray(shr->vao);
				glBindBuffer(GL_ARRAY_BUFFER, shr->vboRect);
				glBufferSubData(GL_ARRAY_BUFFER, 0, 4*sizeof(short), &vertex);
			}
			this->_essentialTasks2.pop();
		}
	}
	return true;
}

Bitmap::Bitmap(Texture *tex) :ShapeRect(Bitmap::CRC32), tex(tex) {
	if (tex->event == Texture::LOADED) {
		this->setRect(tex->width, tex->height);
	}else{
		Bitmap::updateBuffer.push_back(this);
	}
}
void Bitmap::trace() {
	printf("<Bitmap a='%i' x='%i' y='%i' gx='%i' gy='%i' w='%i' h='%i' texId='%i'/>\n", 
			this->mouseEventActive, this->getX(), this->getY(), this->getGlobalX(), this->getGlobalY(), this->getWidth(), this->getHeight(), this->tex);
}
int Bitmap::renderGL330() {
	if (this->tex == nullptr) return false;
	SET_SHADER(ShaderBitmap);
	glBindVertexArray(this->vao);
	glBindTexture(GL_TEXTURE_2D, this->tex->GLID);
	glDrawArrays(GL_POINTS, 0, 1);
	return true;
}
int Bitmap::renderGL100() {
	svec2 global = this->getGlobalPosition();
	global += this->getOffsetPosition();
	glBindTexture(GL_TEXTURE_2D, tex->GLID);
	glColor4ub(0xFF,0xFF,0xFF,0xFF);
	glEnable( GL_TEXTURE_2D );
	glBegin( GL_QUADS );
		glTexCoord2d( 0.0, 0.0 );	glVertex2s(global.x,			global.y );
		glTexCoord2d( 0.0, 1.0 );	glVertex2s(global.x,			global.y+tex->height );
		glTexCoord2d( 1.0, 1.0 );	glVertex2s(global.x+tex->width, global.y+tex->height );
		glTexCoord2d( 1.0, 0.0 );	glVertex2s(global.x+tex->width, global.y );
	glEnd();
	glDisable( GL_TEXTURE_2D );
	return true;
}
void Bitmap::updateBitmaps() {
	if ( !Bitmap::updateBuffer.empty() ) {
		Bitmap *bmp;
		int countUpdateBMP = 0;
		for(int i=0; i<Bitmap::updateBuffer.size()-countUpdateBMP; i++) {
			bmp = Bitmap::updateBuffer[i];
			if (bmp->tex->GLID!=0 && bmp->tex->event==Texture::LOADED) {
				bmp->setRect(bmp->tex->width, bmp->tex->height);
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

BitmapAtlas::BitmapAtlas(Texture *tex, short id) :ShapeRect(BitmapAtlas::CRC32), tex(tex) {
	this->setRectID(id);
}
bool BitmapAtlas::setRectID(short id) {
	if (id >= this->tex->rects.size) return false;
	
	usvec4 vc = this->tex->rects[id];
	this->setRect(vc.z-vc.x, vc.w-vc.y);
	this->texRectID = id;
	//if (OpenGL::ver!=OpenGL::VER_CORE_100) Windows::window->eachFrame.addTask(new VBOUpdateTask(this->vboRectID, &this->texRectID, 2));
	return true;
}
void BitmapAtlas::trace() {
	printf("<BitmapAtlas local='%i %i' global='%i %i' rect='%i %i' GLID='%i' mouseEventActive='%i'/>\n", 
			this->mouseEventActive, this->local.x, this->local.y, this->global.x, this->global.y, this->width, this->height, this->tex->GLID);
}
int BitmapAtlas::renderGL330() {
	if (this->tex == nullptr) return false;
	SET_SHADER(ShaderBitmapAtlas);
	glUniform1i(ShaderBitmapAtlas::prog->rectID, this->texRectID);
	glBindVertexArray(this->vao);
	glBindTexture(GL_TEXTURE_2D, this->tex->GLID);
	glBindTexture(GL_TEXTURE_1D, this->tex->rectGLID);
	glDrawArrays(GL_POINTS, 0, 1);
	return true;
}
int BitmapAtlas::renderGL100() {
	svec2 global = this->global + this->offset;
	vec4 coord(
		(float)(tex->rects[texRectID].x)/tex->width,
		(float)(tex->rects[texRectID].y)/tex->height,
		(float)(tex->rects[texRectID].z)/tex->width,
		(float)(tex->rects[texRectID].w)/tex->height);
	glBindTexture(GL_TEXTURE_2D, tex->GLID);
	glColor4ub(0xFF,0xFF,0xFF,0xFF);
	glEnable( GL_TEXTURE_2D );
	glBegin( GL_QUADS );
		glTexCoord2f( coord.x, coord.y );	glVertex2s(global.x,		global.y );
		glTexCoord2f( coord.x, coord.w );	glVertex2s(global.x,		global.y+height );
		glTexCoord2f( coord.z, coord.w );	glVertex2s(global.x+width,	global.y+height );
		glTexCoord2f( coord.z, coord.y );	glVertex2s(global.x+width,	global.y );
	glEnd();
	glDisable( GL_TEXTURE_2D );
	return true;
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
int FPoint::renderGL100() {
	glPushMatrix();
	//glTranslatef();
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPEAT);
	glPointSize(this->radius);
	glBegin(GL_POINTS);
	  glColor3ub(this->color.r, this->color.g, this->color.b );
	  glVertex2s(this->getGlobalX(), this->getGlobalY());
	glEnd();
	glPopMatrix();
	return true;
}
FLines::FLines(Segment *sgs, short length, short lineWidth, argb color) :ShapeRect(FLines::CRC32), segments(length) {
	memcpy(this->segments.data, sgs, length*sizeof(Segment));
	this->lineWidth = lineWidth;
	this->color = color;
}
int FLines::renderGL100() {
	glLineWidth(this->lineWidth);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glBegin(GL_LINE_STRIP);
	glColor3ub(this->color.r, this->color.g, this->color.b );
	vec2 prev;
	vec2 p0, p1, q0, q1;
	vec2 p0p1Vec, p1p2Vec, vec;
	float frequency = 0.1;
	for (int i=0; i<this->segments.size; i++) {
		if (this->segments[i].type==Segment::TYPE::line) {
			prev.x = this->segments[i].p1.x;
			prev.y = this->segments[i].p1.y;
			glVertex2s(this->segments[i].p1.x, this->segments[i].p1.y);
		}else{
			p0.x = this->segments[i].p1.x;
			p0.y = this->segments[i].p1.y;
			p1.x = this->segments[i].p2.x;
			p1.y = this->segments[i].p2.y;
			p0p1Vec = p0-prev;
			p1p2Vec = p1-p0;
			for (float time = 0.0; time<1.0; time+=frequency) {
				q0 = p0p1Vec*time+prev;
				q1 = p1p2Vec*time+p0;
				vec = (q1-q0)*time+q0;

				glVertex2f(vec.x, vec.y);
				//glVertex2f(100, 200);
			}
			prev = p1;
		}
	}
	glEnd();
	return true;
}

FRect::FRect(short width, short height, uint32_t backgroundColor) :ShapeRect(FRect::CRC32) {
	this->setRect(width, height);
	this->backgroundColor.color = backgroundColor;
}
int FRect::renderGL100() {
	/*glPushMatrix();
	glBegin(GL_QUADS);
		glColor3ub(this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b );
		glVertex2s(this->globalx, this->globaly);
		glVertex2s(this->globalx+this->width, this->globaly);
		glVertex2s(this->globalx+this->width, this->globaly+this->height);
		glVertex2s(this->globalx, this->globaly+this->height);
	glEnd();
	glPopMatrix();*/
	return false;
}
int FRect::renderGL330() {
	//glUseProgram();
	puts("FRect::renderGL330");
	/*if (GLShader::shader->crc32!=ShaderFPrimitiv::CRC32) GLShader::setShader(ShaderFPrimitiv::prog);
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
	glDrawArrays(GL_POINTS, 0, 1);*/
	return true;
}

Scene3D::Scene3D():ShapeRect(1232123), model(nullptr) {
	this->viewMatrix = Matrix3D::ViewPerspective2(45.0, (float)Windows::window->width/(float)Windows::window->height, 1, 1000);
	this->setBox(0, 0, Windows::window->width, Windows::window->height);
}
int Scene3D::renderGL100() {
	OpenGL::pushViewport();
	OpenGL::pushViewMatrix();
	OpenGL::setViewport(this->getGlobalX()+this->getOffsetX(), 
			Windows::window->height-this->getGlobalY()+this->getOffsetY()-this->getHeight(),
			this->getWidth(), this->getHeight());
	OpenGL::setViewMatrix(this->viewMatrix);
	
	//this->root.renderGL100();
	
	OpenGL::popViewMatrix();
	OpenGL::popViewport();
	
	glLineWidth(1);
	glColor4ub(0xFF,0,0,0xFF);
	glBegin(GL_LINE_STRIP);// <editor-fold defaultstate="collapsed" desc="GL_LINE_STRIP">
		glVertex2s( this->getGlobalX()+this->getOffsetX(),					this->getGlobalY()+this->getOffsetY() );
		glVertex2s( this->getGlobalX()+this->getOffsetX()+this->getWidth(),	this->getGlobalY()+this->getOffsetY() );
		glVertex2s( this->getGlobalX()+this->getOffsetX()+this->getWidth(),	this->getGlobalY()+this->getOffsetY()+this->getHeight() );
		glVertex2s( this->getGlobalX()+this->getOffsetX(),					this->getGlobalY()+this->getOffsetY()+this->getHeight() );
		glVertex2s( this->getGlobalX()+this->getOffsetX(),					this->getGlobalY()+this->getOffsetY() );
	glEnd();// </editor-fold>
	return true;
}
int Scene3D::renderGL330() {
	OpenGL::pushViewport();
	OpenGL::pushViewMatrix();
	OpenGL::setViewport(this->getGlobalX()+this->getOffsetX(), 
			Windows::window->height-this->getGlobalY()+this->getOffsetY()-this->getHeight(),
			this->getWidth(), this->getHeight());
	OpenGL::setViewMatrix(this->viewMatrix);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	if (this->model!=nullptr && ShaderF3D::prog->shaderProgram!=0) {
		SET_SHADER(ShaderF3D);
		glUniform4f(ShaderF3D::prog->fillColor, 0, 0.4, 0, 1);
		glUniformMatrix4fv(ShaderF3D::prog->transformMatrix, 1, GL_FALSE, this->viewPosMatrix.a);
		glBindVertexArray(this->model->vao);
		glDrawElements(GL_TRIANGLES, this->model->polygon.size()*3, GL_UNSIGNED_INT, NULL);
	}
	/*
	float s = 1;
		float vertex[24][3] = {
			{-s, s, s}, { s, s, s}, { s,-s, s}, {-s,-s, s}, // front
			{ s, s,-s}, {-s, s,-s}, {-s,-s,-s}, { s,-s,-s}, // back
			{-s, s,-s}, { s, s,-s}, { s, s, s}, {-s, s, s}, // top
			{ s,-s,-s}, {-s,-s,-s}, {-s,-s, s}, { s,-s, s}, // bottom
			{-s, s,-s}, {-s, s, s}, {-s,-s, s}, {-s,-s,-s}, // left
			{ s, s, s}, { s, s,-s}, { s,-s,-s}, { s,-s, s}  // right
		};
		unsigned int index[36] = {
			0, 3, 1,  1, 3, 2, // back
			4, 7, 5,  5, 7, 6, // front
			8,11, 9,  9,11,10, // top
		   12,15,13, 13,15,14, // bottom
		   16,19,17, 17,19,18, // left
		   20,23,21, 21,23,22  // right
		};
		glGenVertexArrays(1, &this->vao);
		if (this->vao==0) printf("VAO_NULL");
		glBindVertexArray(this->vao);
		
		glGenBuffers(1, &this->vbo1);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo1);
		glBufferData(GL_ARRAY_BUFFER, 24*3*sizeof(float), vertex, GL_STATIC_DRAW);
		glVertexAttribPointer(ShaderF3D::prog->position, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(ShaderF3D::prog->position);
		
		glGenBuffers(1, &this->vbo2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo2);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36*sizeof(unsigned int), index, GL_STATIC_DRAW);*/
	//glClearColor( 0, 0, 1, 1 );
	//glClear( GL_COLOR_BUFFER_BIT );
	
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	OpenGL::popViewMatrix();
	OpenGL::popViewport();
	return true;
}