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
			printf("width=%s %i ", value, this->width);
		}else if ( strcmp("height", name)==0 ) {
			SVG::Length h = SVG::DataTypes::getLength(value);
			this->height = SVG::DataTypes::getPixels(h);
			printf("height=%s %i ", value, this->height);
		}else{
			printf("%s=%s ", name, value);
		}
		printf("\n");
		attr = attr->next;
		
	}
	this->viewMatrix = ViewMatrixOrtho(0, this->width, 0, this->height, -1, 1);
	
	ImageSVG::loadGroup(node, &this->root);
	/*node = node->xmlChildrenNode;
	while(node != NULL) {
		name = (char*)node->name;
		if ( strcmp("rect", name)==0 ) {
			ImageSVG::loadShapeRect(node, &this->root);
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
	}*/
	
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
void ImageSVG::loadGroup(xmlNodePtr node, SVG::Group* group) {
	xmlNodePtr child;
	char *name, *value, *attrName, *attrValue;
	xmlAttr *attr = node->properties;
	
	while(attr != NULL) {
		name = (char*)attr->name;
		value = (char*)xmlNodeListGetString(node->doc, attr->children, 1);
		if ( strcmp("transform", name)==0 ) {
			group->matrix = SVG::DataTypes::getTransformMatrix(value);
			printf("transform=%s ", value);
		}
		printf("\n");
		attr = attr->next;
	}
	
	child = node->xmlChildrenNode;
	while(child != NULL) {
		name = (char*)child->name;
		if ( strcmp("rect", name)==0 ) {
			ImageSVG::loadShapeRect(child, group);
		}else if ( strcmp("circle", name)==0 ) {
			ImageSVG::loadShapeCircle(child, group);
		}else if ( strcmp("ellipse", name)==0 ) {
			ImageSVG::loadShapeEllipse(child, group);
		}else if ( strcmp("line", name)==0 ) {
			ImageSVG::loadShapeLine(child, group);
		}else if ( strcmp("polyline", name)==0 ) {
			ImageSVG::loadShapePolyline(child, group);
		}else if ( strcmp("polygon", name)==0 ) {
			ImageSVG::loadShapePolygon(child, group);
		}else if ( name[0]=='g' ) {
			SVG::Group *newgr = new SVG::Group();
			group->child.push_back(newgr);
			ImageSVG::loadGroup(child, newgr);
		}else{
			printf("%s=%s \n", name, value);
		}
		child = child->next;
	}
	
}
void ImageSVG::loadShapeRect(xmlNodePtr node, SVG::Group* group) {
	SVG::BasicShapeRect *rect = new SVG::BasicShapeRect();
	xmlAttr *attr;
	char *name, *value, *attrName, *attrValue;
	attr = node->properties;
	while(attr != NULL) {
		attrName = (char*)attr->name;
		attrValue = (char*)xmlNodeListGetString(node->doc, attr->children, 1);
		if ( strcmp("width", attrName)==0 ) {
			rect->width = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("height", attrName)==0 ) {
			rect->height = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("x", attrName)==0 ) {
			rect->x = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("y", attrName)==0 ) {
			rect->y = SVG::DataTypes::getLength(attrValue);
		}else{
		}//getColor
		attr = attr->next;
	}
	group->child.push_back(rect);
}
void ImageSVG::loadShapeCircle(xmlNodePtr node, SVG::Group* group) {
	SVG::BasicShapeCircle *circle = new SVG::BasicShapeCircle();
	xmlAttr *attr;
	char *name, *value, *attrName, *attrValue;
	attr = node->properties;
	while(attr != NULL) {
		attrName = (char*)attr->name;
		attrValue = (char*)xmlNodeListGetString(node->doc, attr->children, 1);
		if ( strcmp("cx", attrName)==0 ) {
			circle->cx = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("cy", attrName)==0 ) {
			circle->cy = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("r", attrName)==0 ) {
			circle->r = SVG::DataTypes::getLength(attrValue);
		}else{
		}
		attr = attr->next;
	}
	group->child.push_back(circle);
}
void ImageSVG::loadShapeEllipse(xmlNodePtr node, SVG::Group* group) {
	SVG::BasicShapeEllipse *ellipse = new SVG::BasicShapeEllipse();
	xmlAttr *attr;
	char *name, *value, *attrName, *attrValue;
	attr = node->properties;
	while(attr != NULL) {
		attrName = (char*)attr->name;
		attrValue = (char*)xmlNodeListGetString(node->doc, attr->children, 1);
		if ( strcmp("cx", attrName)==0 ) {
			ellipse->cx = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("cy", attrName)==0 ) {
			ellipse->cy = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("rx", attrName)==0 ) {
			ellipse->rx = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("ry", attrName)==0 ) {
			ellipse->ry = SVG::DataTypes::getLength(attrValue);
		}else{
		}
		attr = attr->next;
	}
	group->child.push_back(ellipse);
}
void ImageSVG::loadShapeLine(xmlNodePtr node, SVG::Group* group) {
	SVG::BasicShapeLine *line = new SVG::BasicShapeLine();
	xmlAttr *attr;
	char *name, *value, *attrName, *attrValue;
	attr = node->properties;
	while(attr != NULL) {
		attrName = (char*)attr->name;
		attrValue = (char*)xmlNodeListGetString(node->doc, attr->children, 1);
		if ( strcmp("x1", attrName)==0 ) {
			line->x1 = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("x2", attrName)==0 ) {
			line->x2 = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("y1", attrName)==0 ) {
			line->y1 = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("y2", attrName)==0 ) {
			line->y2 = SVG::DataTypes::getLength(attrValue);
		}else{
		}
		attr = attr->next;
	}
	group->child.push_back(line);
}
void ImageSVG::loadShapePolyline(xmlNodePtr node, SVG::Group* group) {
	SVG::BasicShapePolyline *polyline = new SVG::BasicShapePolyline();
	polyline->length = 0;
	/*xmlAttr *attr;
	char *name, *value, *attrName, *attrValue;
	attr = node->properties;
	while(attr != NULL) {
		attrName = (char*)attr->name;
		attrValue = (char*)xmlNodeListGetString(node->doc, attr->children, 1);
		if ( strcmp("x1", attrName)==0 ) {
			line->x1 = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("x2", attrName)==0 ) {
			line->x2 = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("y1", attrName)==0 ) {
			line->y1 = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("y2", attrName)==0 ) {
			line->y2 = SVG::DataTypes::getLength(attrValue);
		}else{
		}
		attr = attr->next;
	}*/
	group->child.push_back(polyline);
}
void ImageSVG::loadShapePolygon(xmlNodePtr node, SVG::Group* group) {
	SVG::BasicShapePolygon *polygon = new SVG::BasicShapePolygon();
	polygon->length = 0;
	/*xmlAttr *attr;
	char *name, *value, *attrName, *attrValue;
	attr = node->properties;
	while(attr != NULL) {
		attrName = (char*)attr->name;
		attrValue = (char*)xmlNodeListGetString(node->doc, attr->children, 1);
		if ( strcmp("x1", attrName)==0 ) {
			line->x1 = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("x2", attrName)==0 ) {
			line->x2 = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("y1", attrName)==0 ) {
			line->y1 = SVG::DataTypes::getLength(attrValue);
		}else if ( strcmp("y2", attrName)==0 ) {
			line->y2 = SVG::DataTypes::getLength(attrValue);
		}else{
		}
		attr = attr->next;
	}*/
	group->child.push_back(polygon);
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
	return true;
}
int ImageSVG::renderGL400() {
	OpenGL::pushViewport();
	OpenGL::pushViewMatrix();
	OpenGL::setViewport(this->globalx+this->offsetPos.x, 
			Windows::window->height-this->globaly+this->offsetPos.y-this->height,
			this->width, this->height);
	OpenGL::setViewMatrix(this->viewMatrix);
	
	this->root.renderGL400();
	
	OpenGL::popViewMatrix();
	OpenGL::popViewport();
	return true;
}
int ImageSVG::renderGL330() {
	OpenGL::pushViewport();
	OpenGL::pushViewMatrix();
	OpenGL::setViewport(this->globalx+this->offsetPos.x, 
			Windows::window->height-this->globaly+this->offsetPos.y-this->height,
			this->width, this->height);
	OpenGL::setViewMatrix(this->viewMatrix);
	
	this->root.renderGL330();
	
	OpenGL::popViewMatrix();
	OpenGL::popViewport();
	return true;
}
int ImageSVG::renderGL210() {
	OpenGL::pushViewport();
	OpenGL::pushViewMatrix();
	OpenGL::setViewport(this->globalx+this->offsetPos.x, 
			Windows::window->height-this->globaly+this->offsetPos.y-this->height,
			this->width, this->height);
	OpenGL::setViewMatrix(this->viewMatrix);
	
	this->root.renderGL210();
	
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
	return true;
}