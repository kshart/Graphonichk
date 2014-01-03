
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
	ScrollBarH *sb = (ScrollBarH*)(e->shape);
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
	ScrollBarH *sb = (ScrollBarH*)(e->obj);
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
	ScrollBarH *sb = (ScrollBarH*)(e->obj);
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
	ScrollBarH *sb = (ScrollBarH*)(e->shape);
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
	ScrollBarH *sb = (ScrollBarH*)(e->obj);
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
	ScrollBarH *sb = (ScrollBarH*)(e->obj);
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

ScrollBarH::ScrollBarH(unsigned short w, unsigned short h) :Directory() {
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
ScrollBarW::ScrollBarW(unsigned short w, unsigned short h) :Directory() {
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


void ButtonMouseDown (const EventMouseShape *e) {
	printf("mouseDown\n");
	Button *sb = (Button*)(e->shape);
	sb->press = true;
	sb->status = Button::PRESS;
	root.window->renderComplete = false;
}
void ButtonMouseUp (const EventMouse *e) {
	Button *sb = (Button*)(e->obj);
	sb->press = false;
	sb->status = Button::NORMAL;
	root.window->renderComplete = false;
}
void ButtonMouseMove (const EventMouseShape *e) {
	Button *sb = (Button*)(e->shape);
	short mousex, mousey;
	//sb->scrollStarted = false;
}

Button::Button(unsigned short w, unsigned short h) :Shape(0) {
	this->width = w;
	this->height = h;
	this->press = false;
	this->status = Button::NORMAL;
	this->shapePressed = new FRect(w, h, 0xFFFF0000);
	this->shapeNormal = new FRect(w, h, 0xFF00FF00);
	this->shapeRollOver = new FRect(w, h, 0xFF0000FF);
	this->addEventHandler(EventMouseShape::MOUSE_DOWN, ButtonMouseDown);
	this->addEventHandler(EventMouseShape::MOUSE_MOVE, ButtonMouseMove);
	root.window->events.mouse.addEventHandler(EventMouse::MOUSE_UP, ButtonMouseUp, this);
}
int Button::renderGLComptAll() {
	switch(this->status) {
		case Button::NORMAL:
			this->shapeNormal->renderGLComptAll();
			return true;
		case Button::PRESS:
			this->shapePressed->renderGLComptAll();
			return true;
		case Button::ROLLOVER:
			this->shapeRollOver->renderGLComptAll();
			return true;
	}
}
int Button::renderGL400() {
	switch(this->status) {
		case Button::NORMAL:
			this->shapeNormal->renderGL400();
			return true;
		case Button::PRESS:
			this->shapePressed->renderGL400();
			return true;
		case Button::ROLLOVER:
			this->shapeRollOver->renderGL400();
			return true;
	}
}
int Button::renderGL330() {
	switch(this->status) {
		case Button::NORMAL:
			this->shapeNormal->renderGL330();
			return true;
		case Button::PRESS:
			this->shapePressed->renderGL330();
			return true;
		case Button::ROLLOVER:
			this->shapeRollOver->renderGL330();
			return true;
	}
}
int Button::renderGL210() {
	switch(this->status) {
		case Button::NORMAL:
			this->shapeNormal->renderGL210();
			return true;
		case Button::PRESS:
			this->shapePressed->renderGL210();
			return true;
		case Button::ROLLOVER:
			this->shapeRollOver->renderGL210();
			return true;
	}
}