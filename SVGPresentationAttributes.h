/* 
 * File:   SVGPresentationAttributes.h
 * Author: Артем
 *
 * Created on 19 Февраль 2014 г., 12:22
 */

#ifndef SVGPRESENTATIONATTRIBUTES_H
#define	SVGPRESENTATIONATTRIBUTES_H

#include "grSVG.h"
#include "SVG.h"
using namespace Graphonichk;
namespace SVG {
	typedef int SVGShape;
	typedef struct {
		enum ALIGNMENTBASELINE {
			_auto, 
			baseline, 
			beforeEdge, 
			textBeforeEdge, 
			middle, 
			central, 
			afterEdge, 
			textAfterEdge, 
			ideographic, 
			alphabetic, 
			hanging, 
			mathematical, 
			inherit=0
		};
		ALIGNMENTBASELINE value;
	} PresentationAttributesAlignmentBaseline;
	typedef struct {
		enum BASELINESHIFT {
			baseline, 
			sub, 
			super, 
			_percentage, 
			_length,
			inherit=0
		};
		BASELINESHIFT value;
		union {
			Percentage percentage;
			Length length;
		};
	} PresentationAttributesBaselineShift;
	typedef struct {
		enum VISIBILITY {
			visible,
			hidden,
			collapse,
			inherit=0
		};
		VISIBILITY value;
	} PresentationAttributesVisibility;
	typedef struct {
		enum CLIP {
			_auto,
			//shape,
			inherit=0
		};
		CLIP value;
		SVGShape *shape;
	} PresentationAttributesClip;
	
	typedef struct {
		PresentationAttributesAlignmentBaseline alignmentBaseline;
		PresentationAttributesBaselineShift baselineShift;
		
		PresentationAttributesVisibility visibility;
		// writingMode;
		//‘clip-path’, ‘clip-rule’, ‘color’, 
		//‘color-interpolation’, ‘color-interpolation-filters’, ‘color-profile’,
		//‘color-rendering’, ‘cursor’, ‘direction’, ‘display’, ‘dominant-baseline’, 
		//‘enable-background’, ‘fill’, ‘fill-opacity’, ‘fill-rule’, ‘filter’, 
		//‘flood-color’, ‘flood-opacity’, ‘font-family’, ‘font-size’, ‘font-size-adjust’,
		//‘font-stretch’, ‘font-style’, ‘font-variant’, ‘font-weight’, 
		//‘glyph-orientation-horizontal’, ‘glyph-orientation-vertical’, ‘image-rendering’, 
		//‘kerning’, ‘letter-spacing’, ‘lighting-color’, ‘marker-end’, ‘marker-mid’, 
		//‘marker-start’, ‘mask’, ‘opacity’, ‘overflow’, ‘pointer-events’, ‘shape-rendering’, 
		//‘stop-color’, ‘stop-opacity’, ‘stroke’, ‘stroke-dasharray’, ‘stroke-dashoffset’, 
		//‘stroke-linecap’, ‘stroke-linejoin’, ‘stroke-miterlimit’, ‘stroke-opacity’, 
		//‘stroke-width’, ‘text-anchor’, ‘text-decoration’, ‘text-rendering’, ‘unicode-bidi’,
		//‘word-spacing’
	} PresentationAttributes;
	/*
	 enum WRITING_MODE {
			lrtb, 
			rltb, 
			tbrl, 
			lr, 
			rl, 
			tb, 
			inherit=NULL
		};*/
}

#endif	/* SVGPRESENTATIONATTRIBUTES_H */

