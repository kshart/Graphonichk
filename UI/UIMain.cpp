
#include <vector>

#include "../grBaseTypes.h"
#include "UIMain.h"

using namespace std;
using namespace grEngine;


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

UIUnitDirectory::UIUnitDirectory(unsigned short w, unsigned short h, Shape *sh) :UIUnit(w, h, sh) {
	this->separationStyle = false;
}
void UIUnitDirectory::addChild(UIUnit* unit, unsigned short pos) {
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
Directory* UIUnitDirectory::getRect() {
	Directory *dir = new Directory();
	this->resize(this->width, this->height);
	
	for(int i=0; i<this->child.size(); i++) {
		this->child[i]->getRect(dir);
	}
	//rect = new FRect(this->scaledWidth, this->scaledHeight, 0xFF00FFFF);
	//vec->push_back(rect);
	return dir;
}
void UIUnitDirectory::getRect(Directory *dir) {
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

UIUnit::UIUnit(unsigned short w, unsigned short h, Shape *sh) {
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
Directory* UIUnit::getRect() {
	Directory *dir = new Directory();
	FRect *rect = new FRect(this->scaledWidth-2, this->scaledHeight-2, 0xFF00FFFF);
	rect->drag(this->globalx+1, this->globaly+1);
	dir->addChild(rect);
	return dir;
}
void UIUnit::getRect(Directory *dir) {
	FRect *rect = new FRect(this->scaledWidth-2, this->scaledHeight-2, 0xFF00FFFF);
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
	if (sb->scrollStarted) {
		mousex = e->x - sb->globalx - sb->bar->width/2;
		mousey = e->y - sb->globaly - sb->bar->height/2;
		if (mousey<0) mousey = 0;
		if ( mousey>(sb->height - sb->bar->height) ) mousey = sb->height - sb->bar->height;
		sb->position = (float)mousey/(sb->height - sb->bar->height);
		sb->bar->drag(0, mousey);
		
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

UIScrollBarH::UIScrollBarH(unsigned short w, unsigned short h) :Directory(UIScrollBarH::CRC32) {
	this->pad = new FRect(w, h, 0xFF0000FF);
	this->bar = new FRect(w, 60, 0xFFFF0000);
	this->addChild(this->pad);
	this->addChild(this->bar);
	this->width = w;
	this->height = h;
	
	this->addEventHandler(EventMouseShape::MOUSE_DOWN, ScrollBarHmouseDown);
	root.window->events.mouse.addEventHandler(EventMouse::MOUSE_MOVE, ScrollBarHmouseMove, this);
	root.window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, ScrollBarHmouseUp, this);
	root.window->renderComplete = false;
}
UIScrollBarW::UIScrollBarW(unsigned short w, unsigned short h) :Directory(UIScrollBarW::CRC32) {
	this->pad = new FRect(w, h, 0xFF0000FF);
	this->bar = new FRect(60, h, 0xFFFF0000);
	this->addChild(this->pad);
	this->addChild(this->bar);
	this->width = w;
	this->height = h;
	
	this->addEventHandler(EventMouseShape::MOUSE_DOWN, ScrollBarWmouseDown);
	root.window->events.mouse.addEventHandler(EventMouse::MOUSE_MOVE, ScrollBarWmouseMove, this);
	root.window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, ScrollBarWmouseUp, this);
	root.window->renderComplete = false;
}

void ButtonMouseUp (const EventMouse *e) {
	UIButton *sb = (UIButton*)(e->obj);
	sb->press = false;
	if (sb->mouseEventRollOver) {
		sb->status = UIButton::ROLLOVER;
	}else{
		sb->status = UIButton::NORMAL;
	}
	root.window->renderComplete = false;
}

UIButton::UIButton(int crc32, unsigned short w, unsigned short h, Shape* shPressed, Shape* shNormal, Shape* shRollOver) :Shape(crc32) {
	this->mouseEventActive = true;
	this->width = w;
	this->height = h;
	this->press = false;
	this->status = UIButton::NORMAL;
	this->shapePressed = shPressed;
	this->shapeNormal = shNormal;
	this->shapeRollOver = shRollOver;
	root.window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, ButtonMouseUp, this);
}
UIButton::UIButton(unsigned short w, unsigned short h) :Shape(UIButton::CRC32) {
	this->mouseEventActive = true;
	this->width = w;
	this->height = h;
	this->press = false;
	this->status = UIButton::NORMAL;
	this->shapePressed = new FRect(w, h, 0xFF16A085);
	this->shapeNormal = new FRect(w, h, 0xFF1ABC9C);
	this->shapeRollOver = new FRect(w, h, 0xFF48C9B0);
	((FRect*)this->shapePressed)->radius = 20;
	((FRect*)this->shapeNormal)->radius = 20;
	((FRect*)this->shapeRollOver)->radius = 20;
	root.window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, ButtonMouseUp, this);
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
	root.window->renderComplete = false;
}
int UIButton::callEvent(EventMouseShape* event) {
	//event->shape = this;
	//event->localx = event->globalx - this->globalx;
	//event->localy = event->globaly - this->globaly;
	switch (event->type) {
		case EventMouseShape::MOUSE_DOWN:
			this->press = true;
			this->status = UIButton::PRESS;
			root.window->renderComplete = false;
			return true;
		case EventMouseShape::MOUSE_ROLL_OUT:
			this->status = UIButton::NORMAL;
			root.window->renderComplete = false;
			return true;
		case EventMouseShape::MOUSE_ROLL_OVER:
			if (this->press) {
				this->status = UIButton::PRESS;
			}else{
				this->status = UIButton::ROLLOVER;
			}
			root.window->renderComplete = false;
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
		root.window->renderComplete = false;	
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
		root.window->renderComplete = false;
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
		root.window->renderComplete = false;	
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
		root.window->renderComplete = false;	
	}
}

UICheckbox::UICheckbox(int crc32, unsigned short w, unsigned short h, Shape* shUnchkPressed, Shape* shUnchkNormal, Shape* shUnchkRollOver, Shape* shUnchkDisable, 
		Shape* shChkPressed, Shape* shChkNormal, Shape* shChkRollOver, Shape* shChkDisable) :Shape(crc32) {
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
	root.window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, UICheckboxMouseUp, this);
}
UICheckbox::UICheckbox(unsigned short w, unsigned short h) :Shape(UIButton::CRC32) {
	Texture *tex1 = new Texture("FlatUICheckBox.png");
	Texture *tex2 = new Texture("FlatUICheckBoxRollOver.png");
	Texture *tex3 = new Texture("FlatUICheckBoxCheck.png");
	this->width = w;
	this->height = h;
	this->press = false;
	this->checked = false;
	this->status = UICheckbox::UNCHECKED_NORMAL;
	this->shUnchkPressed = new Bitmap(tex2);
	this->shUnchkNormal = new Bitmap(tex1);
	this->shUnchkRollOver = new Bitmap(tex2);
	this->shUnchkDisable = new FRect(w, h, 0xFFAAAAAA);
	this->shChkPressed = new Bitmap(tex3);
	this->shChkNormal = new Bitmap(tex3);
	this->shChkRollOver = new Bitmap(tex3);
	this->shChkDisable = new FRect(w, h, 0xFF444444);
	this->addEventHandler(EventMouseShape::MOUSE_DOWN, UICheckboxMouseDown);
	this->addEventHandler(EventMouseShape::MOUSE_ROLL_OUT, UICheckboxMouseOut);
	this->addEventHandler(EventMouseShape::MOUSE_ROLL_OVER, UICheckboxMouseOver);
	root.window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, UICheckboxMouseUp, this);
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
	root.window->renderComplete = false;
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


UIRadioButtonGroup::UIRadioButtonGroup() :Directory(UIRadioButtonGroup::CRC32) {
	
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
		root.window->renderComplete = false;	
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
		root.window->renderComplete = false;
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
		root.window->renderComplete = false;	
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
		root.window->renderComplete = false;	
	}
}


UIRadioButton::UIRadioButton(unsigned short w, unsigned short h, UIRadioButtonGroup *gr) :Shape(UIRadioButton::CRC32) {
	Texture *tex1 = new Texture("FlatUIRadioButton.png");
	Texture *tex2 = new Texture("FlatUIRadioButtonRollOver.png");
	Texture *tex3 = new Texture("FlatUIRadioButtonCheck.png");
	this->width = w;
	this->height = h;
	this->group = gr;
	this->press = false;
	this->checked = false;
	this->status = UIRadioButton::UNCHECKED_NORMAL;
	this->shUnchkPressed = new Bitmap(tex2);
	this->shUnchkNormal = new Bitmap(tex1);
	this->shUnchkRollOver = new Bitmap(tex2);
	this->shUnchkDisable = new FRect(w, h, 0xFFAAAAAA);
	this->shChkPressed = new Bitmap(tex3);
	this->shChkNormal = new Bitmap(tex3);
	this->shChkRollOver = new Bitmap(tex3);
	this->shChkDisable = new FRect(w, h, 0xFF444444);
	this->addEventHandler(EventMouseShape::MOUSE_DOWN, UIRadioButtonMouseDown);
	this->addEventHandler(EventMouseShape::MOUSE_ROLL_OUT, UIRadioButtonMouseOut);
	this->addEventHandler(EventMouseShape::MOUSE_ROLL_OVER, UIRadioButtonMouseOver);
	root.window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, UIRadioButtonMouseUp, this);
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
	root.window->renderComplete = false;
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