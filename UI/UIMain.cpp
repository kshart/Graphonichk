#include "../grBaseTypes.h"
#include "UIMain.h"

using namespace std;
using namespace Graphonichk;


UIWorkspace::UIWorkspace(unsigned short width, unsigned short height) {
	this->UIWorkspace::width = width;
	this->UIWorkspace::height = height;
	this->root = new UIUnitDirectory(width, height);
	//this->root->resize(width, height);
}
void UIWorkspace::getUIUnits() {
	//vector<UIUnit*> arr;
	//this->root->getChild(&arr);
	//printf("getUIUnits arrSize=%i\n", arr.size());
}

UIUnitDirectory::UIUnitDirectory(unsigned short w, unsigned short h, ShapeRect *sh) :UIUnit(w, h, sh) {
	this->separationStyle = false;
}
bool UIUnitDirectory::addChild(UIUnit* unit, unsigned short pos) {
	//UIUnitDirectory *obj = dynamic_cast<UIUnitDirectory*>(unit);
	unit->parent = this;
	/*if (obj==NULL) {
		if (this->child.empty()) {
			unit->x = 0;
			unit->y = 0;
		}else if (this->separationStyle == UIUnitDirectory::SEPARATION_HEIGHT) {
			unit->x = 0;
			unit->y = (this->child.back())->y+(this->child.back())->height;
		}else{
			unit->x = (this->child.back())->x+(this->child.back())->width;
			unit->y = 0;
		}
	}else{*/
		/*if (this->child.empty()) {
			unit->x = 0;
			unit->y = 0;a
		}else if (this->separationStyle == UIUnitDirectory::SEPARATION_HEIGHT) {
			unit->x = 0;
			unit->y = (this->child.back())->y+(this->child.back())->height;
		}else{
			unit->x = (this->child.back())->x+(this->child.back())->width;
			unit->y = 0;
		}
		if (this->separationStyle == UIUnitDirectory::SEPARATION_HEIGHT) {
			if (unit->y > this->scaledHeight) {
				unit->scaledWidth = 0;
				unit->scaledHeight = 0;
			}else if ( unit->y+unit->height > this->scaledHeight) {
				unit->scaledWidth = this->scaledWidth;
				unit->scaledHeight = this->scaledHeight-unit->y;
			}else{
				unit->scaledWidth = this->scaledWidth;
				unit->scaledHeight = unit->height;
			}
		}else{
			if (unit->x > this->scaledWidth) {
				unit->scaledWidth = 0;
				unit->scaledHeight = 0;
			}else if ( unit->x+unit->width > this->scaledWidth) {
				unit->scaledWidth = this->scaledWidth-unit->x;
				unit->scaledHeight = this->scaledHeight;
			}else{
				unit->scaledWidth = unit->width;
				unit->scaledHeight = this->scaledHeight;
			}
		}
	//}
	unit->updateGlobalPosition();*/
	this->child.push_back(unit);
	return true;
}
void UIUnitDirectory::drag(unsigned short x, unsigned short y) {
	this->UIUnit::drag(x, y);
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->updateGlobalPosition();
	}
}
void UIUnitDirectory::resize(unsigned short w, unsigned short h) {
	UIUnit *unit, *lastUnit;
	this->scaledWidth = w;
	this->scaledHeight = h;
	if (this->child.empty()) return;
	unit = this->child[0];
	unit->drag(0, 0);
	if (this->child.size()==1) {
		unit->resize(this->scaledWidth, this->scaledHeight);
		return;
	}
	if (this->separationStyle==SEPARATION::SEPARATION_HEIGHT) {
		if (unit->height > this->scaledHeight) {
			unit->resize(this->scaledWidth, this->scaledHeight);
			return;
		}else{
			unit->resize(this->scaledWidth, unit->height);
			for(int i=0; i<this->child.size()-2; i++) {
				lastUnit = this->child[i];
				unit = this->child[i+1];
				unit->drag(0, lastUnit->y+lastUnit->height);
				if (unit->y+unit->height > this->scaledHeight) {
					unit->resize(this->scaledWidth, this->scaledHeight-unit->y);
					return;
				}else{
					unit->resize(this->scaledWidth, unit->height);
				}
			}
			lastUnit = this->child[this->child.size()-2];
			unit = this->child.back();
			unit->drag(0, lastUnit->y+lastUnit->height);
			unit->resize(this->scaledWidth, this->scaledHeight-unit->y);
		}
	}else{
		if (unit->width > this->scaledWidth) {
			unit->resize(this->scaledWidth, this->scaledHeight);
			return;
		}else{
			unit->resize(unit->width, this->scaledHeight);
			for(int i=0; i<this->child.size()-2; i++) {
				lastUnit = this->child[i];
				unit = this->child[i+1];
				unit->drag(lastUnit->x+lastUnit->width, 0);
				if (unit->x+unit->width > this->scaledWidth) {
					unit->resize(this->scaledWidth-unit->x, this->scaledHeight);
					return;
				}else{
					unit->resize(unit->width, this->scaledHeight);
				}
			}
			lastUnit = this->child[this->child.size()-2];
			unit = this->child.back();
			unit->drag(lastUnit->x+lastUnit->width, 0);
			unit->resize(this->scaledWidth-unit->x, this->scaledHeight);
		}
	}
}
ShapeGroupRect* UIUnitDirectory::getRect() {
	ShapeGroupRect *dir = new ShapeGroupRect();
	this->resize(this->width, this->height);
	
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->getRect(dir);
	}
	//rect = new FRect(this->scaledWidth, this->scaledHeight, 0xFF00FFFF);
	//vec->push_back(rect);
	return dir;
}
void UIUnitDirectory::getRect(ShapeGroupRect *dir) {
	//FRect *rect;
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->getRect(dir);
	}
	//rect = new FRect(this->scaledWidth, this->scaledHeight, 0xFF00FFFF);
	//vec->push_back(rect);
}
void UIUnitDirectory::trace(unsigned int tab) {
	printf( "%*.*c", 3*tab, 3*tab, ' ' );
	printf("<UIObject x='%i'\ty='%i'\tgx='%i'\tgy='%i'\tw='%i'\th='%i'\tsw='%i'\tsh='%i'>\n", this->x, this->y, this->globalx, this->globaly, this->width, this->height, this->scaledWidth, this->scaledHeight);
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->trace(tab+1);
	}
	printf( "%*.*c", 3*tab, 3*tab, ' ' );
	printf("</UIObject>\n");
}


UIUnit::UIUnit(unsigned short w, unsigned short h, ShapeRect *sh) {
	this->scaledWidth = this->width = w;
	this->scaledHeight = this->height = h;
	this->x = this->y = this->globalx = this->globaly;
}
void UIUnit::updateGlobalPosition() {
	if (this->parent==NULL) return;
	this->globalx = this->parent->globalx + this->x;
	this->globaly = this->parent->globaly + this->y;
}
void UIUnit::drag(unsigned short x, unsigned short y) {
	this->x = x;
	this->y = y;
	if (this->parent==NULL) return;
	this->globalx = this->parent->globalx+x;
	this->globaly = this->parent->globaly+y;
}
void UIUnit::resize(unsigned short w, unsigned short h) {
	this->scaledWidth = w;
	this->scaledHeight = h;
}
ShapeGroupRect* UIUnit::getRect() {
	ShapeGroupRect *dir = new ShapeGroupRect();
	FRect *rect = new FRect(this->scaledWidth-2, this->scaledHeight-2, 0xFF00FFFF);
	rect->drag(this->globalx+1, this->globaly+1);
	dir->addChild(rect);
	return dir;
}
void UIUnit::getRect(ShapeGroupRect *dir) {
	FRect *rect = new FRect(this->scaledWidth-2, this->scaledHeight-2, 0xFF686868);
	rect->drag(this->globalx+1, this->globaly+1);
	dir->addChild(rect);
}
void UIUnit::trace(unsigned int tab) {
	printf( "%*.*c", 3*tab, 3*tab, ' ' );
	printf("<UIUnit x='%i'\ty='%i'\tgx='%i'\tgy='%i'\tw='%i'\th='%i'\tsw='%i'\tsh='%i'/>\n", this->x, this->y, this->globalx, this->globaly, this->width, this->height, this->scaledWidth, this->scaledHeight);
}



void ScrollBarHmouseDown (const EventMouseShape *e) {
	UIScrollBarH *sb = (UIScrollBarH*)(e->shape);
	printf("paintMouseDown\n");
	short mousex, mousey;
	mousex = e->localx - sb->bar->width/2;
	mousey = e->localy - sb->bar->height/2;
	if (mousey<0) mousey = 0;
	if ( mousey>(sb->height - sb->bar->height) ) mousey = sb->height - sb->bar->height;
	sb->position = (float)mousey/(sb->height - sb->bar->height);
	sb->bar->drag(0, mousey);
	sb->scrollStarted = true;
}
void ScrollBarHmouseUp (const EventMouse *e) {
	UIScrollBarH *sb = (UIScrollBarH*)(e->obj);
	short mousex, mousey;
	if (sb->scrollStarted) {
		mousex = e->x - sb->globalx - sb->bar->width/2;
		mousey = e->y - sb->globaly - sb->bar->height/2;
		
		if (mousey<0) mousey = 0;
		if ( mousey>(sb->height - sb->bar->height) ) mousey = sb->height - sb->bar->height;
		sb->position = (float)mousey/(sb->height - sb->bar->height);
		sb->bar->drag(0, mousey);
		sb->scrollStarted = false;
	}
}
void ScrollBarHmouseMove (const EventMouse *e) {
	UIScrollBarH *sb = (UIScrollBarH*)(e->obj);
	short mousex, mousey;
	float n;
	if (sb->scrollStarted) {
		mousex = e->x - sb->globalx - sb->bar->width/2;
		mousey = e->y - sb->globaly - sb->bar->height/2;
		if (mousey<0) mousey = 0;
		if ( mousey>(sb->height - sb->bar->height) ) mousey = sb->height - sb->bar->height;
		n = (float)mousey/(sb->height - sb->bar->height);
		sb->bar->drag(0, mousey);
		if (sb->position == n) return;
		sb->position = n;
		if (sb->chengePosition!=NULL) sb->chengePosition(sb->position, sb->obj);
	}
	//sb->scrollStarted = false;
}
void ScrollBarWmouseDown (const EventMouseShape *e) {
	UIScrollBarH *sb = (UIScrollBarH*)(e->shape);
	short mousex, mousey;
	printf("paintMouseDown\n");
	mousex = e->localx - sb->bar->width/2;
	mousey = e->localy - sb->bar->height/2;
	if (mousey<0) mousey = 0;
	if ( mousex>(sb->width - sb->bar->width) ) mousex = sb->width - sb->bar->width;
	sb->position = (float)mousex/(sb->width - sb->bar->width);
	sb->bar->drag(mousex, 0);
	sb->scrollStarted = true;
}
void ScrollBarWmouseUp (const EventMouse *e) {
	UIScrollBarH *sb = (UIScrollBarH*)(e->obj);
	short mousex, mousey;
	if (sb->scrollStarted) {
		mousex = e->x - sb->globalx - sb->bar->width/2;
		mousey = e->y - sb->globaly - sb->bar->height/2;
		
		if (mousex<0) mousex = 0;
		if ( mousex>(sb->width - sb->bar->width) ) mousex = sb->width - sb->bar->width;
		sb->position = (float)mousex/(sb->width - sb->bar->width);
		sb->bar->drag(mousex, 0);
		sb->scrollStarted = false;
	}
}
void ScrollBarWmouseMove (const EventMouse *e) {
	UIScrollBarH *sb = (UIScrollBarH*)(e->obj);
	short mousex, mousey;
	if (sb->scrollStarted) {
		mousex = e->x - sb->globalx - sb->bar->width/2;
		mousey = e->y - sb->globaly - sb->bar->height/2;
		if (mousex<0) mousex = 0;
		if ( mousex>(sb->width - sb->bar->width) ) mousex = sb->width - sb->bar->width;
		sb->position = (float)mousex/(sb->width - sb->bar->width);
		sb->bar->drag(mousex, 0);
		
	}
	//sb->scrollStarted = false;
}

UIScrollBarH::UIScrollBarH(unsigned short w, unsigned short h) :ShapeGroupRect(UIScrollBarH::CRC32) {
	this->pad = new FRect(w, h, 0xFF0000FF);
	this->bar = new FRect(w, 60, 0xFFFF0000);
	this->addChild(this->pad);
	this->addChild(this->bar);
	this->scrollStarted = false;
	this->width = w;
	this->height = h;
	this->chengePosition = NULL;
	this->addEventHandler(EventMouseShape::MOUSE_DOWN, ScrollBarHmouseDown);
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_MOVE, ScrollBarHmouseMove, this);
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, ScrollBarHmouseUp, this);
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}
UIScrollBarW::UIScrollBarW(unsigned short w, unsigned short h) :ShapeGroupRect(UIScrollBarW::CRC32) {
	this->pad = new FRect(w, h, 0xFF0000FF);
	this->bar = new FRect(60, h, 0xFFFF0000);
	this->addChild(this->pad);
	this->addChild(this->bar);
	this->scrollStarted = false;
	this->width = w;
	this->height = h;
	
	this->addEventHandler(EventMouseShape::MOUSE_DOWN, ScrollBarWmouseDown);
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_MOVE, ScrollBarWmouseMove, this);
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, ScrollBarWmouseUp, this);
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}



void ButtonMouseUp (const EventMouse *e) {
	UIButton *sb = (UIButton*)(e->obj);
	sb->press = false;
	if (sb->mouseEventRollOver) {
		sb->status = UIButton::ROLLOVER;
	}else{
		sb->status = UIButton::NORMAL;
	}
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}

UIButton::UIButton(int crc32, unsigned short w, unsigned short h, ShapeRect* shPressed, ShapeRect* shNormal, ShapeRect* shRollOver) :ShapeRect(crc32) {
	this->mouseEventActive = true;
	this->width = w;
	this->height = h;
	this->press = false;
	this->status = UIButton::NORMAL;
	this->shapePressed = shPressed;
	this->shapeNormal = shNormal;
	this->shapeRollOver = shRollOver;
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, ButtonMouseUp, this);
}
UIButton::UIButton(unsigned short w, unsigned short h) :ShapeRect(UIButton::CRC32) {
	this->mouseEventActive = true;
	this->width = w;
	this->height = h;
	this->press = false;
	this->status = UIButton::NORMAL;
	this->shapePressed = new FRect(w, h, 0xFF16A085);
	this->shapeNormal = new FRect(w, h, 0xFF1ABC9C);
	this->shapeRollOver = new FRect(w, h, 0xFF48C9B0);
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, ButtonMouseUp, this);
}
void UIButton::updateGlobalPosition() {
	if (this->parent!=NULL) {
		this->globalx = parent->globalx+this->x;
		this->globaly = parent->globaly+this->y;
		this->shapePressed->globalx = this->globalx+shapePressed->x;
		this->shapePressed->globaly = this->globaly+shapePressed->y;
		this->shapeNormal->globalx = this->globalx+shapeNormal->x;
		this->shapeNormal->globaly = this->globaly+shapeNormal->y;
		this->shapeRollOver->globalx = this->globalx+shapeRollOver->x;
		this->shapeRollOver->globaly = this->globaly+shapeRollOver->y;
	}
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}
int UIButton::callEvent(EventMouseShape* event) {
	//event->shape = this;
	//event->localx = event->globalx - this->globalx;
	//event->localy = event->globaly - this->globaly;
	switch (event->type) {
		case EventMouseShape::MOUSE_DOWN:
			this->press = true;
			this->status = UIButton::PRESS;
			#ifdef REDRAWN_BY_THE_ACTION
			Windows::window->renderComplete = false;
			#endif
			return true;
		case EventMouseShape::MOUSE_ROLL_OUT:
			this->status = UIButton::NORMAL;
			#ifdef REDRAWN_BY_THE_ACTION
			Windows::window->renderComplete = false;
			#endif
			return true;
		case EventMouseShape::MOUSE_ROLL_OVER:
			if (this->press) {
				this->status = UIButton::PRESS;
			}else{
				this->status = UIButton::ROLLOVER;
			}
			#ifdef REDRAWN_BY_THE_ACTION
			Windows::window->renderComplete = false;
			#endif
			return true;
	}
}
int UIButton::renderGLComptAll() {
	switch(this->status) {
		case UIButton::NORMAL:
			this->shapeNormal->renderGLComptAll();
			return true;
		case UIButton::PRESS:
			this->shapePressed->renderGLComptAll();
			return true;
		case UIButton::ROLLOVER:
			this->shapeRollOver->renderGLComptAll();
			return true;
	}
}
int UIButton::renderGL400() {
	switch(this->status) {
		case UIButton::NORMAL:
			this->shapeNormal->renderGL400();
			return true;
		case UIButton::PRESS:
			this->shapePressed->renderGL400();
			return true;
		case UIButton::ROLLOVER:
			this->shapeRollOver->renderGL400();
			return true;
	}
}
int UIButton::renderGL330() {
	switch(this->status) {
		case UIButton::NORMAL:
			this->shapeNormal->renderGL330();
			return true;
		case UIButton::PRESS:
			this->shapePressed->renderGL330();
			return true;
		case UIButton::ROLLOVER:
			this->shapeRollOver->renderGL330();
			return true;
	}
}
int UIButton::renderGL210() {
	switch(this->status) {
		case UIButton::NORMAL:
			this->shapeNormal->renderGL210();
			return true;
		case UIButton::PRESS:
			this->shapePressed->renderGL210();
			return true;
		case UIButton::ROLLOVER:
			this->shapeRollOver->renderGL210();
			return true;
	}
}



void UICheckboxMouseDown (const EventMouseShape *e) {
	UICheckbox *sb = (UICheckbox*)(e->shape);
	if (sb->status!=UICheckbox::CHECKED_DISABLE && sb->status!=UICheckbox::UNCHECKED_DISABLE) {
		sb->press = true;
		if (sb->checked) {
			sb->status = UICheckbox::CHECKED_PRESS;
		}else{
			sb->status = UICheckbox::UNCHECKED_PRESS;
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}
void UICheckboxMouseUp (const EventMouse *e) {
	UICheckbox *sb = (UICheckbox*)(e->obj);
	short localx, localy;
	if (sb->press) {
		sb->press = false;
		localx = e->x - sb->globalx;
		localy = e->y - sb->globaly;
		if (localx>0 && localx<sb->width && localy>0 && localy<sb->height) {
			if (sb->checked) {
				sb->checked = false;
				sb->status = UICheckbox::UNCHECKED_NORMAL;
			}else{
				sb->checked = true;
				sb->status = UICheckbox::CHECKED_NORMAL;
			}
		}else{
			if (sb->checked) {
				sb->status = UICheckbox::CHECKED_NORMAL;
			}else{
				sb->status = UICheckbox::UNCHECKED_NORMAL;
			}
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}
void UICheckboxMouseOver (const EventMouseShape *e) {
	UICheckbox *sb = (UICheckbox*)(e->shape);
	if (sb->status!=UICheckbox::CHECKED_DISABLE && sb->status!=UICheckbox::UNCHECKED_DISABLE) {
		if (sb->checked) {
			if (sb->press) {
				sb->status = UICheckbox::CHECKED_PRESS;
			}else{
				sb->status = UICheckbox::CHECKED_ROLLOVER;
			}
		}else{
			if (sb->press) {
				sb->status = UICheckbox::UNCHECKED_PRESS;
			}else{
				sb->status = UICheckbox::UNCHECKED_ROLLOVER;
			}
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}
void UICheckboxMouseOut (const EventMouseShape *e) {
	UICheckbox *sb = (UICheckbox*)(e->shape);
	if (sb->status!=UICheckbox::CHECKED_DISABLE && sb->status!=UICheckbox::UNCHECKED_DISABLE) {
		if (sb->checked) {
			sb->status = UICheckbox::CHECKED_NORMAL;
		}else{
			sb->status = UICheckbox::UNCHECKED_NORMAL;
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}

UICheckbox::UICheckbox(int crc32, unsigned short w, unsigned short h, ShapeRect* shUnchkPressed, ShapeRect* shUnchkNormal, ShapeRect* shUnchkRollOver, ShapeRect* shUnchkDisable, 
		ShapeRect* shChkPressed, ShapeRect* shChkNormal, ShapeRect* shChkRollOver, ShapeRect* shChkDisable) :ShapeRect(crc32) {
	this->width = w;
	this->height = h;
	this->press = false;
	this->checked = false;
	this->status = UICheckbox::UNCHECKED_NORMAL;
	this->shUnchkPressed = shUnchkPressed;
	this->shUnchkNormal = shUnchkNormal;
	this->shUnchkRollOver = shUnchkRollOver;
	this->shUnchkDisable = shUnchkDisable;
	this->shChkPressed = shChkPressed;
	this->shChkNormal = shChkNormal;
	this->shChkRollOver = shChkRollOver;
	this->shChkDisable = shChkDisable;
	this->addEventHandler(EventMouseShape::MOUSE_DOWN, UICheckboxMouseDown);
	this->addEventHandler(EventMouseShape::MOUSE_ROLL_OUT, UICheckboxMouseOut);
	this->addEventHandler(EventMouseShape::MOUSE_ROLL_OVER, UICheckboxMouseOver);
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, UICheckboxMouseUp, this);
}
UICheckbox::UICheckbox(unsigned short w, unsigned short h) :ShapeRect(UIButton::CRC32) {
	this->width = w;
	this->height = h;
	this->press = false;
	this->checked = false;
	this->status = UICheckbox::UNCHECKED_NORMAL;
	this->shUnchkPressed =	new FRect(w, h, 0xFF00AAAA);
	this->shUnchkNormal =	new FRect(w, h, 0xFFAA00AA);
	this->shUnchkRollOver = new FRect(w, h, 0xFFFF0000);
	this->shUnchkDisable =	new FRect(w, h, 0xFF117700);
	this->shChkPressed =	new FRect(w, h, 0xFFFF0000);
	this->shChkNormal =		new FRect(w, h, 0xFF00FF00);
	this->shChkRollOver =	new FRect(w, h, 0xFF0000FF);
	this->shChkDisable =	new FRect(w, h, 0xFF117766);
	this->addEventHandler(EventMouseShape::MOUSE_DOWN, UICheckboxMouseDown);
	this->addEventHandler(EventMouseShape::MOUSE_ROLL_OUT, UICheckboxMouseOut);
	this->addEventHandler(EventMouseShape::MOUSE_ROLL_OVER, UICheckboxMouseOver);
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, UICheckboxMouseUp, this);
}
void UICheckbox::updateGlobalPosition() {
	if (this->parent!=NULL) {
		this->globalx = this->parent->globalx+this->x;
		this->globaly = this->parent->globaly+this->y;
		this->shUnchkPressed->globalx = this->globalx+shUnchkPressed->x;
		this->shUnchkPressed->globaly = this->globaly+shUnchkPressed->y;
		this->shUnchkNormal->globalx = this->globalx+shUnchkNormal->x;
		this->shUnchkNormal->globaly = this->globaly+shUnchkNormal->y;
		this->shUnchkRollOver->globalx = this->globalx+shUnchkRollOver->x;
		this->shUnchkRollOver->globaly = this->globaly+shUnchkRollOver->y;
		this->shUnchkDisable->globalx = this->globalx+shUnchkDisable->x;
		this->shUnchkDisable->globaly = this->globaly+shUnchkDisable->y;
		this->shChkPressed->globalx = this->globalx+shChkPressed->x;
		this->shChkPressed->globaly = this->globaly+shChkPressed->y;
		this->shChkNormal->globalx = this->globalx+shChkNormal->x;
		this->shChkNormal->globaly = this->globaly+shChkNormal->y;
		this->shChkRollOver->globalx = this->globalx+shChkRollOver->x;
		this->shChkRollOver->globaly = this->globaly+shChkRollOver->y;
		this->shChkDisable->globalx = this->globalx+shChkRollOver->x;
		this->shChkDisable->globaly = this->globaly+shChkRollOver->y;
	}
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}
int UICheckbox::renderGLComptAll() {
	switch(this->status) {
		case UICheckbox::UNCHECKED_PRESS:
			this->shUnchkPressed->renderGLComptAll();
			return true;
		case UICheckbox::UNCHECKED_NORMAL:
			this->shUnchkNormal->renderGLComptAll();
			return true;
		case UICheckbox::UNCHECKED_ROLLOVER:
			this->shUnchkRollOver->renderGLComptAll();
			return true;
		case UICheckbox::UNCHECKED_DISABLE:
			this->shUnchkDisable->renderGLComptAll();
			return true;
		case UICheckbox::CHECKED_PRESS:
			this->shChkPressed->renderGLComptAll();
			return true;
		case UICheckbox::CHECKED_NORMAL:
			this->shChkNormal->renderGLComptAll();
			return true;
		case UICheckbox::CHECKED_ROLLOVER:
			this->shChkRollOver->renderGLComptAll();
			return true;
		case UICheckbox::CHECKED_DISABLE:
			this->shChkDisable->renderGLComptAll();
			return true;
	}
}
int UICheckbox::renderGL400() {
	switch(this->status) {
		case UICheckbox::UNCHECKED_PRESS:
			this->shUnchkPressed->renderGL400();
			return true;
		case UICheckbox::UNCHECKED_NORMAL:
			this->shUnchkNormal->renderGL400();
			return true;
		case UICheckbox::UNCHECKED_ROLLOVER:
			this->shUnchkRollOver->renderGL400();
			return true;
		case UICheckbox::UNCHECKED_DISABLE:
			this->shUnchkDisable->renderGL400();
			return true;
		case UICheckbox::CHECKED_PRESS:
			this->shChkPressed->renderGL400();
			return true;
		case UICheckbox::CHECKED_NORMAL:
			this->shChkNormal->renderGL400();
			return true;
		case UICheckbox::CHECKED_ROLLOVER:
			this->shChkRollOver->renderGL400();
			return true;
		case UICheckbox::CHECKED_DISABLE:
			this->shChkDisable->renderGL400();
			return true;
	}
}
int UICheckbox::renderGL330() {
	switch(this->status) {
		case UICheckbox::UNCHECKED_PRESS:
			this->shUnchkPressed->renderGL330();
			return true;
		case UICheckbox::UNCHECKED_NORMAL:
			this->shUnchkNormal->renderGL330();
			return true;
		case UICheckbox::UNCHECKED_ROLLOVER:
			this->shUnchkRollOver->renderGL330();
			return true;
		case UICheckbox::UNCHECKED_DISABLE:
			this->shUnchkDisable->renderGL330();
			return true;
		case UICheckbox::CHECKED_PRESS:
			this->shChkPressed->renderGL330();
			return true;
		case UICheckbox::CHECKED_NORMAL:
			this->shChkNormal->renderGL330();
			return true;
		case UICheckbox::CHECKED_ROLLOVER:
			this->shChkRollOver->renderGL330();
			return true;
		case UICheckbox::CHECKED_DISABLE:
			this->shChkDisable->renderGL330();
			return true;
	}
}
int UICheckbox::renderGL210() {
	switch(this->status) {
		case UICheckbox::UNCHECKED_PRESS:
			this->shUnchkPressed->renderGL210();
			return true;
		case UICheckbox::UNCHECKED_NORMAL:
			this->shUnchkNormal->renderGL210();
			return true;
		case UICheckbox::UNCHECKED_ROLLOVER:
			this->shUnchkRollOver->renderGL210();
			return true;
		case UICheckbox::UNCHECKED_DISABLE:
			this->shUnchkDisable->renderGL210();
			return true;
		case UICheckbox::CHECKED_PRESS:
			this->shChkPressed->renderGL210();
			return true;
		case UICheckbox::CHECKED_NORMAL:
			this->shChkNormal->renderGL210();
			return true;
		case UICheckbox::CHECKED_ROLLOVER:
			this->shChkRollOver->renderGL210();
			return true;
		case UICheckbox::CHECKED_DISABLE:
			this->shChkDisable->renderGL210();
			return true;
	}
}



UIRadioButtonGroup::UIRadioButtonGroup() :ShapeGroupRect (UIRadioButtonGroup::CRC32) {
	
}
UIRadioButton *UIRadioButtonGroup::addRadioButton() {
	UIRadioButton *rb = new UIRadioButton(20, 20, this);
	this->addChild(rb);
	return rb;
}
bool UIRadioButtonGroup::setActiveButton(UIRadioButton* rb) {
	UIRadioButton *rb1;
	bool searchRB = false;
	for (int i=this->child.size()-1; i>=0; i--) {
		rb1 = dynamic_cast<UIRadioButton*>(this->child[i]);
		if (rb1!=NULL) {
			if (rb1 = rb) {
				searchRB = true;
				rb1->checked = true;
				rb1->status = UIRadioButton::CHECKED_NORMAL;
			}else{
				rb1->checked = false;
				rb1->status = UIRadioButton::UNCHECKED_NORMAL;
			}
		}
	}
	return searchRB;
}

void UIRadioButtonMouseDown (const EventMouseShape *e) {
	printf("UIRadioButtonMouseDown");
	UIRadioButton *sb = (UIRadioButton*)(e->shape);
	if (sb->status!=UIRadioButton::CHECKED_DISABLE && sb->status!=UIRadioButton::UNCHECKED_DISABLE) {
		sb->press = true;
		if (sb->checked) {
			sb->status = UIRadioButton::CHECKED_PRESS;
		}else{
			sb->status = UIRadioButton::UNCHECKED_PRESS;
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}
void UIRadioButtonMouseUp (const EventMouse *e) {
	UIRadioButton *sb = (UIRadioButton*)(e->obj);
	UIRadioButtonGroup *gr;
	short localx, localy;
	if (sb->press) {
		sb->press = false;
		localx = e->x - sb->globalx;
		localy = e->y - sb->globaly;
		if (localx>0 && localx<sb->width && localy>0 && localy<sb->height) {
			if (!sb->checked) {
				gr = sb->group;
				for (int i=gr->child.size()-1; i>=0; i--) {
					((UIRadioButton*)(gr->child[i]))->checked = false;
					((UIRadioButton*)(gr->child[i]))->status = UIRadioButton::UNCHECKED_NORMAL;
				}
				sb->checked = true;
				sb->status = UIRadioButton::CHECKED_NORMAL;
			}
		}else{
			if (sb->checked) {
				sb->status = UIRadioButton::CHECKED_NORMAL;
			}else{
				sb->status = UIRadioButton::UNCHECKED_NORMAL;
			}
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}
void UIRadioButtonMouseOver (const EventMouseShape *e) {
	UIRadioButton *sb = (UIRadioButton*)(e->shape);
	if (sb->status!=UIRadioButton::CHECKED_DISABLE && sb->status!=UIRadioButton::UNCHECKED_DISABLE) {
		if (sb->checked) {
			if (sb->press) {
				sb->status = UIRadioButton::CHECKED_PRESS;
			}else{
				sb->status = UIRadioButton::CHECKED_ROLLOVER;
			}
		}else{
			if (sb->press) {
				sb->status = UIRadioButton::UNCHECKED_PRESS;
			}else{
				sb->status = UIRadioButton::UNCHECKED_ROLLOVER;
			}
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif	
	}
}
void UIRadioButtonMouseOut (const EventMouseShape *e) {
	UIRadioButton *sb = (UIRadioButton*)(e->shape);
	if (sb->status!=UIRadioButton::CHECKED_DISABLE && sb->status!=UIRadioButton::UNCHECKED_DISABLE) {
		if (sb->checked) {
			sb->status = UIRadioButton::CHECKED_NORMAL;
		}else{
			sb->status = UIRadioButton::UNCHECKED_NORMAL;
		}
		#ifdef REDRAWN_BY_THE_ACTION
		Windows::window->renderComplete = false;
		#endif
	}
}

UIRadioButton::UIRadioButton(unsigned short w, unsigned short h, UIRadioButtonGroup *gr) :ShapeRect(UIRadioButton::CRC32) {
	this->width = w;
	this->height = h;
	this->group = gr;
	this->press = false;
	this->checked = false;
	this->status = UIRadioButton::UNCHECKED_NORMAL;
	this->shUnchkPressed =	new FRect(w, h, 0xFF00AAAA);
	this->shUnchkNormal =	new FRect(w, h, 0xFFAA00AA);
	this->shUnchkRollOver = new FRect(w, h, 0xFFFF0000);
	this->shUnchkDisable =	new FRect(w, h, 0xFF117700);
	this->shChkPressed =	new FRect(w, h, 0xFFFF0000);
	this->shChkNormal =		new FRect(w, h, 0xFF00FF00);
	this->shChkRollOver =	new FRect(w, h, 0xFF0000FF);
	this->shChkDisable =	new FRect(w, h, 0xFF117766);
	this->addEventHandler(EventMouseShape::MOUSE_DOWN, UIRadioButtonMouseDown);
	this->addEventHandler(EventMouseShape::MOUSE_ROLL_OUT, UIRadioButtonMouseOut);
	this->addEventHandler(EventMouseShape::MOUSE_ROLL_OVER, UIRadioButtonMouseOver);
	Windows::window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, UIRadioButtonMouseUp, this);
}
void UIRadioButton::updateGlobalPosition() {
	if (this->parent!=NULL) {
		this->globalx = this->parent->globalx+this->x;
		this->globaly = this->parent->globaly+this->y;
		this->shUnchkPressed->globalx = this->globalx+shUnchkPressed->x;
		this->shUnchkPressed->globaly = this->globaly+shUnchkPressed->y;
		this->shUnchkNormal->globalx = this->globalx+shUnchkNormal->x;
		this->shUnchkNormal->globaly = this->globaly+shUnchkNormal->y;
		this->shUnchkRollOver->globalx = this->globalx+shUnchkRollOver->x;
		this->shUnchkRollOver->globaly = this->globaly+shUnchkRollOver->y;
		this->shUnchkDisable->globalx = this->globalx+shUnchkDisable->x;
		this->shUnchkDisable->globaly = this->globaly+shUnchkDisable->y;
		this->shChkPressed->globalx = this->globalx+shChkPressed->x;
		this->shChkPressed->globaly = this->globaly+shChkPressed->y;
		this->shChkNormal->globalx = this->globalx+shChkNormal->x;
		this->shChkNormal->globaly = this->globaly+shChkNormal->y;
		this->shChkRollOver->globalx = this->globalx+shChkRollOver->x;
		this->shChkRollOver->globaly = this->globaly+shChkRollOver->y;
		this->shChkDisable->globalx = this->globalx+shChkRollOver->x;
		this->shChkDisable->globaly = this->globaly+shChkRollOver->y;
	}
	#ifdef REDRAWN_BY_THE_ACTION
	Windows::window->renderComplete = false;
	#endif
}
int UIRadioButton::renderGLComptAll() {
	switch(this->status) {
		case UIRadioButton::UNCHECKED_PRESS:
			this->shUnchkPressed->renderGLComptAll();
			return true;
		case UIRadioButton::UNCHECKED_NORMAL:
			this->shUnchkNormal->renderGLComptAll();
			return true;
		case UIRadioButton::UNCHECKED_ROLLOVER:
			this->shUnchkRollOver->renderGLComptAll();
			return true;
		case UIRadioButton::UNCHECKED_DISABLE:
			this->shUnchkDisable->renderGLComptAll();
			return true;
		case UIRadioButton::CHECKED_PRESS:
			this->shChkPressed->renderGLComptAll();
			return true;
		case UIRadioButton::CHECKED_NORMAL:
			this->shChkNormal->renderGLComptAll();
			return true;
		case UIRadioButton::CHECKED_ROLLOVER:
			this->shChkRollOver->renderGLComptAll();
			return true;
		case UIRadioButton::CHECKED_DISABLE:
			this->shChkDisable->renderGLComptAll();
			return true;
	}
}
int UIRadioButton::renderGL400() {
	switch(this->status) {
		case UIRadioButton::UNCHECKED_PRESS:
			this->shUnchkPressed->renderGL400();
			return true;
		case UIRadioButton::UNCHECKED_NORMAL:
			this->shUnchkNormal->renderGL400();
			return true;
		case UIRadioButton::UNCHECKED_ROLLOVER:
			this->shUnchkRollOver->renderGL400();
			return true;
		case UIRadioButton::UNCHECKED_DISABLE:
			this->shUnchkDisable->renderGL400();
			return true;
		case UIRadioButton::CHECKED_PRESS:
			this->shChkPressed->renderGL400();
			return true;
		case UIRadioButton::CHECKED_NORMAL:
			this->shChkNormal->renderGL400();
			return true;
		case UIRadioButton::CHECKED_ROLLOVER:
			this->shChkRollOver->renderGL400();
			return true;
		case UIRadioButton::CHECKED_DISABLE:
			this->shChkDisable->renderGL400();
			return true;
	}
}
int UIRadioButton::renderGL330() {
	switch(this->status) {
		case UIRadioButton::UNCHECKED_PRESS:
			this->shUnchkPressed->renderGL330();
			return true;
		case UIRadioButton::UNCHECKED_NORMAL:
			this->shUnchkNormal->renderGL330();
			return true;
		case UIRadioButton::UNCHECKED_ROLLOVER:
			this->shUnchkRollOver->renderGL330();
			return true;
		case UIRadioButton::UNCHECKED_DISABLE:
			this->shUnchkDisable->renderGL330();
			return true;
		case UIRadioButton::CHECKED_PRESS:
			this->shChkPressed->renderGL330();
			return true;
		case UIRadioButton::CHECKED_NORMAL:
			this->shChkNormal->renderGL330();
			return true;
		case UIRadioButton::CHECKED_ROLLOVER:
			this->shChkRollOver->renderGL330();
			return true;
		case UIRadioButton::CHECKED_DISABLE:
			this->shChkDisable->renderGL330();
			return true;
	}
}
int UIRadioButton::renderGL210() {
	switch(this->status) {
		case UIRadioButton::UNCHECKED_PRESS:
			this->shUnchkPressed->renderGL210();
			return true;
		case UIRadioButton::UNCHECKED_NORMAL:
			this->shUnchkNormal->renderGL210();
			return true;
		case UIRadioButton::UNCHECKED_ROLLOVER:
			this->shUnchkRollOver->renderGL210();
			return true;
		case UIRadioButton::UNCHECKED_DISABLE:
			this->shUnchkDisable->renderGL210();
			return true;
		case UIRadioButton::CHECKED_PRESS:
			this->shChkPressed->renderGL210();
			return true;
		case UIRadioButton::CHECKED_NORMAL:
			this->shChkNormal->renderGL210();
			return true;
		case UIRadioButton::CHECKED_ROLLOVER:
			this->shChkRollOver->renderGL210();
			return true;
		case UIRadioButton::CHECKED_DISABLE:
			this->shChkDisable->renderGL210();
			return true;
	}
}



UITable::UITable(unsigned short w, unsigned short h, vector<unsigned short> *columnsWidth) {
	this->cutTheRect = true;
	this->chengeRect = false;
	this->rowsHeight = 30;
	this->offsetPos.x = 0;
	this->offsetPos.y = 0;
	this->width = w;
	this->height = h;
	this->viewPosition = 0.0f;
	this->background = new FRect(w, h, 0xFF333333);
	if (columnsWidth==NULL) return;
	for(int i=0; i<columnsWidth->size(); i++) {
		this->columnsWidth.push_back(columnsWidth->at(i));
	}
}
void UITable::addRow(UITableRow* row) {
	this->rows.push_back(row);
	this->allRowsVisibleSize++;
	this->setPos(this->viewPosition);
}
void UITable::setPos(float n) {
	if (n<0 || n>1) return;
	this->viewPosition = n;
	this->updateRows();
}
void UITable::updateRows() {
	UITableRow *row;
	this->child.clear();
	this->addChild(this->background);
	FRect *r1;
	unsigned short startPos, stopPos;
	int xx=0, i=0, 
		listHeight = this->allRowsVisibleSize*this->rowsHeight, 
		listOffsetHeight = listHeight-this->height,
		offsetY;
	if (listOffsetHeight>0) {
		offsetY = (int)(this->viewPosition*(float)listOffsetHeight);
		startPos = (unsigned short)floor(offsetY/this->rowsHeight);
		stopPos = (unsigned short)ceil(  ((float)(offsetY+this->height))/((float)this->rowsHeight) );
	}else{
		offsetY = 0;
		startPos = 0;
		stopPos = this->allRowsVisibleSize;
	}
	if (startPos>this->allRowsVisibleSize) startPos=0;
	if (stopPos>this->allRowsVisibleSize) stopPos=this->allRowsVisibleSize;
	
	//(unsigned short)
	while (i<this->columnsWidth.size()) {
		r1 = new FRect(this->columnsWidth[i], this->height, 0xFF555555);
		r1->drag(xx, 0);
		xx+=this->columnsWidth[i]+this->columnsWidth[i+1];
		this->addChild(r1);
		i+=2;
	}
	//int y;
	for (int i=startPos; i<stopPos; i++) {
		row = this->rows[i];
		r1 = new FRect(this->width-4, this->rowsHeight-4, 0xFF222222);
		r1->drag(2, i*this->rowsHeight-offsetY+2);
		this->addChild(r1);
		row->columnsUpdate(0, i*this->rowsHeight-offsetY);
	}
}

UITableRow::UITableRow(UITable* table) {
	this->table = table;
}
void UITableRow::addCol(ShapeRect* sh) {
	this->columns.push_back(sh);
}
unsigned short UITableRow::columnsUpdate(unsigned short x, unsigned short y) {
	int offsetX = 0, halfRowsHeight = this->table->rowsHeight/2;
	if ( this->columns.size()  >  this->table->columnsWidth.size() ) {
		for(int i=0; i<this->table->columnsWidth.size(); i++) {
			this->columns[i]->drag( offsetX+this->table->columnsWidth[i]/2-this->columns[i]->width/2+x,   halfRowsHeight-( this->columns[i]->height/2 )+y );
			this->table->addChild(this->columns[i]);
			offsetX+=this->table->columnsWidth[i];
		}
	}else{
		for(int i=0; i<this->columns.size(); i++) {
			this->columns[i]->drag( offsetX+this->table->columnsWidth[i]/2-this->columns[i]->width/2+x,   halfRowsHeight-( this->columns[i]->height/2 )+y );
			this->table->addChild(this->columns[i]);
			offsetX+=this->table->columnsWidth[i];
		}
	}
	return 1;
}



UITableDirectory::UITableDirectory(unsigned short w, unsigned short h, vector<unsigned short>* columnsWidth) {
	this->chengeRect = false;
	this->rowsHeight = 30;
	this->width = w;
	this->height = h;
	this->viewPosition = 0.0f;
	this->background = new FRect(w, h, 0xFF333333);
	if (columnsWidth==NULL) return;
	for(int i=0; i<columnsWidth->size(); i++) {
		this->columnsWidth.push_back(columnsWidth->at(i));
	}
}
void UITableDirectory::addRow(UITableDirectoryRow* row) {
	this->rows.push_back(row);
	//this->allRowsSize += row->allChildSize;
	this->setPos(this->viewPosition);
}
void UITableDirectory::setPos(float n) {
	if (n<0 || n>1) return;
	UITableDirectoryRow *row;
	this->child.clear();
	this->addChild(this->background);
	FRect *r1;
	unsigned short startPos, stopPos;
	int xx=0, i=0, 
		listHeight = this->rows.size()*this->rowsHeight, 
		listOffsetHeight = listHeight-this->height,
		offsetY;
	if (listOffsetHeight>0) {
		offsetY = (int)(n*(float)listOffsetHeight);
		startPos = (unsigned short)floor(offsetY/this->rowsHeight);
		stopPos = (unsigned short)ceil(  ((float)(offsetY+this->height))/((float)this->rowsHeight) );
	}else{
		offsetY = 0;
		startPos = 0;
		stopPos = this->rows.size();
	}
	if (startPos>this->rows.size()) startPos=0;
	if (stopPos>this->rows.size()) stopPos=this->rows.size();
	while (i<this->columnsWidth.size()) {
		r1 = new FRect(this->columnsWidth[i], this->height, 0xFF555555);
		r1->drag(xx, 0);
		xx+=this->columnsWidth[i]+this->columnsWidth[i+1];
		this->addChild(r1);
		i+=2;
	}
	/*for (int y=startPos; y<stopPos; y++) {
		row = this->rows[y];
		row->render(y*this->rowsHeight-offsetY);
	}*/
}

UITableDirectoryRow::UITableDirectoryRow(UITableDirectory* table) {
	this->table = table;
	this->allChildCount = 0;
	this->allChildVisibleCount = 0;
	this->parentDepth = 0;
}
void UITableDirectoryRow::show() {
	
}
void UITableDirectoryRow::hide() {
	
}
void UITableDirectoryRow::addDirectoryRow(UITableDirectoryRow* dir) {
	dir->parent = this;
	this->child.push_back(dir);
	dir->updateParentDepth(this->parentDepth+1);
	UITableDirectoryRow *d1 = dir;
	while(d1->parent!=NULL) {
		d1 = d1->parent;
		//d1->allChildSize += dir->allChildSize+1;
	}
}
void UITableDirectoryRow::addCol(ShapeRect* sh) {
	this->columns.push_back(sh);
}
void UITableDirectoryRow::updateParentDepth(unsigned short i) {
	this->parentDepth = i;
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->updateParentDepth(i+1);
	}
}
int UITableDirectoryRow::render(int y) {
	return 1;
}