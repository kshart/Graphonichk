/* 
 * File:   grSVG.h
 * Author: Артем
 *
 * Created on 18 Февраль 2014 г., 23:31
 */

#ifndef GRSVG_H
#define	GRSVG_H

#include "grBaseTypes.h"
#include <GL/gl.h>
#include "SVG.h"
namespace Graphonichk {
	class ImageSVG :public ShapeRect {
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

