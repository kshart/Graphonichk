
#include <vector>

#include "../grBaseTypes.h"
#include "UIMain.h"

using namespace std;
using namespace grEngine;
/*
UIWorkspace::UIWorkspace(unsigned short width, unsigned short height) {
	this->UIWorkspace::width = width;
	this->UIWorkspace::height = height;
	this->root = new UIObject(width, height);
	this->root->resize(width, height);
}
void UIWorkspace::getUIUnits() {
	vector<UIUnit*> arr;
	this->root->getChild(&arr);
	printf("getUIUnits arrSize=%i\n", arr.size());
}

UIObject::UIObject(unsigned short w, unsigned short h, UIObject *parent, UIUnit *unit) {
	this->parent = parent;
	this->unit = unit;
	this->separationStyle = false;
	this->x = this->y = 0;
	this->rectWidth = w;
	this->rectHeight = h;
	this->width = this->height = this->position = 0;
}
void UIObject::resize(unsigned short w, unsigned short h) {
	this->width = w;
	this->height = h;
	if (this->unit!=NULL) {
		this->unit->width = w;
		this->unit->height = h;
	}
}
void UIObject::addChild(UIUnit* unit, unsigned short size, unsigned short pos) {
	if (this->child.empty()) {
		if (this->unit == NULL) {
			unit->x = unit->y = 0;
			unit->width = this->width;
			unit->height = this->height;
			unit->y = 0;
			this->unit = unit;
		}else{
			UIObject *ui1 = new UIObject(this, this->unit), *ui2 = new UIObject(this, unit);
			if (this->separationStyle == SEPARATION_WIDTH) {
				//unit->width = size;
				ui1->resize(this->unit->width, this->height);
				ui2->resize(unit->width, this->height);
				ui1->x = 0;
				ui1->y = 0;
				ui2->x = this->unit->width;
				ui2->y = 0;
			}else{
				ui1->resize(this->width, this->unit->height);
				ui2->resize(this->width, unit->height);
				//unit->height = size;
			}
			this->child.push_back(ui1);
			this->child.push_back(ui2);
		}
		
	}else if (unit != NULL) {
		UIObject *ui1 = new UIObject(this, unit);
		if (pos == SHRT_MAX) {
			if (this->separationStyle == SEPARATION_WIDTH) {
				//unit->width = size;
				ui1->resize(0, this->height);
			}else{
				ui1->resize(this->width, 0);
				//unit->height = size;
			}
			this->child.push_back(new UIObject(this, unit));
		}else{

		}
	}
}
void UIObject::getChild(vector<UIUnit*> *arr) {
	printf("<UIObject x='%i' y='%i' w='%i' h='%i'/>\n", this->x, this->y, this->width, this->height);
	if (this->child.empty() && this->unit!=NULL) {
		arr->push_back(this->unit);
		//this->unit->trace();
	}else{
		for(int i=0; i<this->child.size(); i++) {
			this->child[i]->getChild(arr);
		}
	}
}*/


UIUnit::UIUnit() :Shape(0) {
	
}
void UIUnit::trace() {
	printf("<UIUnit x='%i' y='%i' width='%i' height='%i'/>\n", this->x, this->y, this->width, this->height);
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
	this->width = w;
	this->height = h;
	this->press = false;
	this->checked = false;
	this->status = UICheckbox::UNCHECKED_NORMAL;
	this->shUnchkPressed = new FRect(w, h, 0xFFFF0000);
	this->shUnchkNormal = new FRect(w, h, 0xFF00FF00);
	this->shUnchkRollOver = new FRect(w, h, 0xFF0000FF);
	this->shUnchkDisable = new FRect(w, h, 0xFFAAAAAA);
	this->shChkPressed = new FRect(w, h, 0xFFFF00FF);
	this->shChkNormal = new FRect(w, h, 0xFFFFFF00);
	this->shChkRollOver = new FRect(w, h, 0xFF00FFFF);
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
	UIRadioButton *rb = new UIRadioButton(40, 40, this);
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
	this->width = w;
	this->height = h;
	this->group = gr;
	this->press = false;
	this->checked = false;
	this->status = UIRadioButton::UNCHECKED_NORMAL;
	this->shUnchkPressed = new FRect(w, h, 0xFFFF0000);
	this->shUnchkNormal = new FRect(w, h, 0xFF00FF00);
	this->shUnchkRollOver = new FRect(w, h, 0xFF0000FF);
	this->shUnchkDisable = new FRect(w, h, 0xFFAAAAAA);
	this->shChkPressed = new FRect(w, h, 0xFFFF00FF);
	this->shChkNormal = new FRect(w, h, 0xFFFFFF00);
	this->shChkRollOver = new FRect(w, h, 0xFF00FFFF);
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