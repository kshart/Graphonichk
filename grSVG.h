/* 
 * File:   grSVG.h
 * Author: Артем
 *
 * Created on 18 Февраль 2014 г., 23:31
 */

#ifndef GRSVG_H
#define	GRSVG_H

#include "grMain.h"

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
		ImageSVG();
		ImageSVG(const char *filename);
		int renderGL330() override;
		int renderGL100() override;
		SVG::Group root;
		Matrix3D viewMatrix;
	};
}

#endif	/* GRSVG_H */

