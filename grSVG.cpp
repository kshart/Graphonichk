#include "grSVG.h"

using namespace Graphonichk;
ImageSVG::ImageSVG() :ShapeRect(0)  {
	
}
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
	
	svec2 size;
	while(attr != NULL) {
		name = (char*)attr->name;
		value = (char*)xmlNodeListGetString(node->doc, attr->children, 1);
		if ( strcmp("width", name)==0 ) {
			SVG::Length w = SVG::DataTypes::getLength(value);
			size.x = w.getPixel();
		}else if ( strcmp("height", name)==0 ) {
			SVG::Length h = SVG::DataTypes::getLength(value);
			size.y = h.getPixel();
		}else{
			printf("%s=%s ", name, value);
		}
		printf("\n");
		attr = attr->next;
		
	}
	this->setRect(size.x, size.y);
	this->viewMatrix = Matrix3D::ViewOrtho(0, size.x, 0, size.y, -1, 1);
	
	ImageSVG::loadGroup(node, &this->root);
    xmlFreeDoc(doc);
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
int ImageSVG::renderGL100() {
	OpenGL::pushViewport();
	OpenGL::pushViewMatrix();
	OpenGL::setViewport(this->getGlobalX()+this->getOffsetX(), 
			Windows::window->height-this->getGlobalY()+this->getOffsetY()-this->getHeight(),
			this->getWidth(), this->getHeight());
	OpenGL::setViewMatrix(this->viewMatrix);
	
	this->root.renderGL100();
	
	OpenGL::popViewMatrix();
	OpenGL::popViewport();
	
	glLineWidth(1);
	glColor4ub(0xFF,0,0,0xFF);
	glBegin(GL_LINE_STRIP);// <editor-fold defaultstate="collapsed" desc="GL_LINE_STRIP">
		glVertex2s( this->getGlobalX()+this->getOffsetX(),					this->getGlobalY()+this->getOffsetY() );
		glVertex2s( this->getGlobalX()+this->getOffsetX()+this->getWidth(), this->getGlobalY()+this->getOffsetY() );
		glVertex2s( this->getGlobalX()+this->getOffsetX()+this->getWidth(), this->getGlobalY()+this->getOffsetY()+this->getHeight() );
		glVertex2s( this->getGlobalX()+this->getOffsetX(),					this->getGlobalY()+this->getOffsetY()+this->getHeight() );
		glVertex2s( this->getGlobalX()+this->getOffsetX(),					this->getGlobalY()+this->getOffsetY() );
	glEnd();// </editor-fold>
	return true;
}
int ImageSVG::renderGL330() {
	OpenGL::pushViewport();
	OpenGL::pushViewMatrix();
	OpenGL::setViewport(this->getGlobalX()+this->getOffsetX(), 
			Windows::window->height-this->getGlobalY()+this->getOffsetY()-this->getHeight(),
			this->getWidth(), this->getHeight());
	OpenGL::setViewMatrix(this->viewMatrix);
	
	//glEnable(GL_MULTISAMPLE);
	
	this->root.renderGL330();
	
	//glDisable(GL_MULTISAMPLE);
	
	OpenGL::popViewMatrix();
	OpenGL::popViewport();
	return true;
}
