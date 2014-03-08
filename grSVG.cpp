/* 
 * File:   grSVG.cpp
 * Author: Артем
 * 
 * Created on 18 Февраль 2014 г., 23:31
 */
#include <libxml/tree.h>
#include <libxml/parser.h>

#include "grSVG.h"
using namespace Graphonichk;
ImageSVG::ImageSVG(const char *filename) :ShapeRect(0) {
	
	
	xmlDocPtr doc;
	xmlAttr *attr;
	xmlNodePtr node;
	char *name, *value, *attrName, *attrValue;
    doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse document\n");
		return;
    }
	node = xmlDocGetRootElement(doc);
	
	
	printf("xml name=%s \n", node->name);
	attr = node->properties;
	
	while(attr != NULL) {
		name = (char*)attr->name;
		value = (char*)xmlNodeListGetString(node->doc, attr->children, 1);
		if ( strcmp("width", name)==0 ) {
			SVG::Length w = SVG::DataTypes::getLength(value);
			this->width = SVG::DataTypes::getPixels(w);
			printf("width=%s %f ", value, w.value);
		}else if ( strcmp("height", name)==0 ) {
			SVG::Length h = SVG::DataTypes::getLength(value);
			this->height = SVG::DataTypes::getPixels(h);
			printf("height=%s %f ", value, h.value);
		}else{
			printf("%s=%s ", name, value);
		}
		printf("\n");
		attr = attr->next;
		
	}
	ViewMatrix vm(0, this->width, 0, this->height, -1, 1);
	this->viewMatrix = vm;
	
	
	
	
	node = node->xmlChildrenNode;
	while(node != NULL) {
		name = (char*)node->name;
		if ( strcmp("rect", name)==0 ) {
			SVG::BasicShapeRect *rect = new SVG::BasicShapeRect();
			attr = node->properties;
			while(attr != NULL) {
				attrName = (char*)attr->name;
				attrValue = (char*)xmlNodeListGetString(node->doc, attr->children, 1);
				printf("|%s %s|", attrName, attrValue);
				if ( strcmp("width", attrName)==0 ) {
					rect->width = SVG::DataTypes::getLength(attrValue);
				}else if ( strcmp("height", attrName)==0 ) {
					rect->height = SVG::DataTypes::getLength(attrValue);
				}else if ( strcmp("x", attrName)==0 ) {
					rect->x = SVG::DataTypes::getLength(attrValue);
				}else if ( strcmp("y", attrName)==0 ) {
					rect->y = SVG::DataTypes::getLength(attrValue);
				}else{
				}
				attr = attr->next;
			}
			printf("rect w=%f h=%f x=%f y=%f\n", rect->width.value, rect->height.value, rect->x.value, rect->y.value);
			this->root.child.push_back(rect);
		}else if ( strcmp("circle", name)==0 ) {
			SVG::BasicShapeCircle *circle = new SVG::BasicShapeCircle();
			circle->cx.value = 100;
			circle->cy.value = 300;
			circle->r.value = 50;
			this->root.child.push_back(circle);
		}else if ( strcmp("ellipse", name)==0 ) {
			SVG::BasicShapeEllipse *ellipse = new SVG::BasicShapeEllipse();
			ellipse->cx.value = 200;
			ellipse->cy.value = 200;
			ellipse->rx.value = 50;
			ellipse->ry.value = 100;
			this->root.child.push_back(ellipse);
		}else{
			printf("%s=%s ", name, value);
		}
		node = node->next;
	}
	
    xmlFreeDoc(doc);
	
	/*SVG::Group *gr1 = new SVG::Group();
	
	
	SVG::BasicShapeLine *line = new SVG::BasicShapeLine();
	line->x1.value = 50;
	line->y1.value = 50;
	line->x2.value = 100;
	line->y2.value = 400;
	
	gr1->child.push_back(ellipse);
	gr1->child.push_back(rect);
	gr1->child.push_back(circle);
	gr1->child.push_back(line);
	gr1->matrix.translate(100, 100);
	gr1->matrix.rotate(45);
	gr1->matrix.rotate(-45);
	gr1->matrix.translate(-100, -100);
	
	SVG::BasicShapePolyline *polyline = new SVG::BasicShapePolyline();
	polyline->length = 4;
	polyline->points = new SVG::BasicPoint[4];
	polyline->points[0].x.value = 50;
	polyline->points[0].y.value = 0;
	polyline->points[1].x.value = 100;
	polyline->points[1].y.value = 50;
	polyline->points[2].x.value = 150;
	polyline->points[2].y.value = 0;
	polyline->points[3].x.value = 200;
	polyline->points[3].y.value = 50;
	
	SVG::BasicShapePolygon *polygon = new SVG::BasicShapePolygon();
	polygon->length = 10;
	polygon->points = new SVG::BasicPoint[10];
	polygon->points[0].x.value = 350;
	polygon->points[0].y.value = 75;
	polygon->points[1].x.value = 379;
	polygon->points[1].y.value = 161;
	polygon->points[2].x.value = 469;
	polygon->points[2].y.value = 161;
	polygon->points[3].x.value = 397;
	polygon->points[3].y.value = 215;
	polygon->points[4].x.value = 423;
	polygon->points[4].y.value = 301;
	polygon->points[5].x.value = 350;
	polygon->points[5].y.value = 250;
	polygon->points[6].x.value = 277;
	polygon->points[6].y.value = 301;
	polygon->points[7].x.value = 303;
	polygon->points[7].y.value = 215;
	polygon->points[8].x.value = 231;
	polygon->points[8].y.value = 161;
	polygon->points[9].x.value = 321;
	polygon->points[9].y.value = 161;
	this->root.child.push_back(gr1);
	this->root.child.push_back(polyline);
	this->root.child.push_back(polygon);*/
}
int ImageSVG::renderGLComptAll() {
	OpenGL::pushViewport();
	OpenGL::pushViewMatrix();
	OpenGL::setViewport(this->globalx+this->offsetPos.x, 
			Windows::window->height-this->globaly+this->offsetPos.y-this->height,
			this->width, this->height);
	OpenGL::setViewMatrix(this->viewMatrix);
	
	this->root.renderGLComptAll();
	
	
	OpenGL::popViewMatrix();
	OpenGL::popViewport();
	
	glLineWidth(1);
	glColor4ub(0xFF,0,0,0xFF);
	glBegin(GL_LINE_STRIP);// <editor-fold defaultstate="collapsed" desc="GL_LINE_STRIP">
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y );
		glVertex2s( this->globalx+this->offsetPos.x+this->width, this->globaly+this->offsetPos.y );
		glVertex2s( this->globalx+this->offsetPos.x+this->width, this->globaly+this->offsetPos.y+this->height );
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y+this->height );
		glVertex2s( this->globalx+this->offsetPos.x, this->globaly+this->offsetPos.y );
	glEnd();// </editor-fold>
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