
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


UIUnit::UIUnit() :Shape(Shape::TYPE_NULL) {
	
}
void UIUnit::trace() {
	printf("<UIUnit x='%i' y='%i' width='%i' height='%i'/>\n", this->x, this->y, this->width, this->height);
}