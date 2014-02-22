/* 
 * File:   grSVG.cpp
 * Author: Артем
 * 
 * Created on 18 Февраль 2014 г., 23:31
 */

#include "grSVG.h"
using namespace Graphonichk;
ImageSVG::ImageSVG() :ShapeRect(0) {
	SVG::BasicShapeCircle *rect = new SVG::BasicShapeCircle();
	rect->cx.value = 100;
	rect->cy.value = 100;
	rect->r.value = 100;
	this->root.child.push_back(rect);
}

int ImageSVG::renderGLComptAll() {
	this->root.renderGLComptAll();
}
int ImageSVG::renderGL400() {
	this->root.renderGL400();
}
int ImageSVG::renderGL330() {
	this->root.renderGL330();
}
int ImageSVG::renderGL210() {
	this->root.renderGL210();
}