/* 
 * File:   grSVG.h
 * Author: Артем
 *
 * Created on 18 Февраль 2014 г., 23:31
 */

#ifndef GRSVG_H
#define	GRSVG_H

#include "grBaseTypes.h"

#include <libxml/tree.h>
#include <libxml/parser.h>
#include "SVG.h"
namespace Graphonichk {
	class ImageSVG :public ShapeRect {
	private:
		void loadGroup(xmlNodePtr node, SVG::Group *group);
		void loadShapeRect(xmlNodePtr node, SVG::Group *group);
		void loadShapeCircle(xmlNodePtr node, SVG::Group *group);
		void loadShapeEllipse(xmlNodePtr node, SVG::Group *group);
		void loadShapeLine(xmlNodePtr node, SVG::Group *group);
		void loadShapePolyline(xmlNodePtr node, SVG::Group *group);
		void loadShapePolygon(xmlNodePtr node, SVG::Group *group);
		void loadVisualProperties(xmlNodePtr node, SVG::Group *group);
	public:
		ImageSVG(const char *filename);
		int renderGLComptAll();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		SVG::Group root;
		ViewMatrix viewMatrix;
	};
}

#endif	/* GRSVG_H */

