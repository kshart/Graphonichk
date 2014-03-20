//add gcc -mno-ms-bitfields
#ifndef GRBASETYPES_H
#define	GRBASETYPES_H

#include <stack>
#include <queue>
#include <vector>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#if defined(WIN32)
	#include <windows.h>
	#include <windowsx.h>
	#include <GL/wglext.h>
	#include <GL/glext.h>
#elif defined(X11)
	#include<stdio.h>
	#include<stdlib.h>
	#include<X11/X.h>
	#include<X11/Xlib.h>
	#include<GL/glx.h>
#endif

#define M_PI 3.14159265358979323846
using namespace std;
//111

// <editor-fold defaultstate="collapsed" desc="#define IMG_TYPE">
#define GR_IMG_BMP "bmp"//0x4D42
#define GR_IMG_TGA "tga"
enum {
	GR_IMG_I_BMP,
	GR_IMG_I_TGA
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="#define TGA">
#define TGA_TYPE_NO 0
#define TGA_TYPE_PALETTE 1
#define TGA_TYPE_TC 2
#define TGA_TYPE_MONOCHROME 3
#define TGA_TYPE_RLE_PALETTE 9
#define TGA_TYPE_RLE_TC 10
#define TGA_TYPE_RLE_MONOCHROME 11
#define TGA_DEP_8 8
#define TGA_DEP_16 16
#define TGA_DEP_24 24
#define TGA_DEP_32 32
#define TGA_TRANSFER_X 0x10
#define TGA_TRANSFER_Y 0x20
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="#define LIB">
#define LIB_SIGN 0x5247
// </editor-fold>

typedef unsigned int uint;

#include "grEvent.h"
#include "grProcessingQueue.h"
#include "grFile.h"
#include "grMatrix.h"
namespace Graphonichk {
	class System;
	class Windows;
	class GLShader;
	class Texture;
	//<Shape>
	//class Buffer;
	//</Shape>
	class Bitmap;
	class FPoint;
	class FLines;
	class FRect;
	class FCircle;
	//Shape
	//Event
	class EventKeyboard;
	class EventMouse;
	class EventWindow;
	//Event
	//
	//Font
	class TextField;
	class Font;
	class TextFormat;
	//Font
	class Rect;
	class Point;
	class Point {
	  public:
		short x, y;
		
	};
	class Rect {
	  public:
		short x, y;
		unsigned short width, height;
	};
	

	template<typename Type> class Array {
	public:
		Type *data;
		unsigned int size;
		
		Array(unsigned int size) {
			this->data = (Type*)malloc( size*sizeof(Type) );
			if (this->data==NULL) {
				this->size = 0;
			}else{
				this->size = size;
			}
		}
		~Array() {
			this->size = 0;
			free(this->data);
		}
	};
	typedef struct __attribute__((packed)) {
		uint16_t	signature;
		uint32_t	sizeFile;
		uint16_t	null1;
		uint16_t	null2;
		uint32_t	offsetPixels;
		uint32_t	sizeHead;
		uint32_t	width;
		uint32_t	height;
		uint16_t	layers;
		uint16_t	bpp;
		uint32_t	compression;
		uint32_t	compressionSize;
		uint32_t	ppmWidth;
		uint32_t	ppmHeight;
		uint32_t	colorsUse;
		uint32_t	colorsSpecial;
	} bmpHead;
	typedef struct __attribute__((packed)) {
		uint8_t	IDLength;
		uint8_t	colorMapType;
		uint8_t	imgType;
		//3
		uint16_t	colorMapIndex;
		uint16_t	colorMapLength;
		uint8_t		colorMapItemSize;
		//5
		uint16_t	x;
		uint16_t	y;
		uint16_t	width;
		uint16_t	height;
		//8
		uint8_t	colorDepth;
		uint8_t	imgDescriptor;
		//7+8+2
	} tgaHead;
	typedef struct __attribute__((packed)) {
		uint16_t signature;
		uint16_t ver;
		uint32_t dirLength;
		uint32_t imgLength;
		uint32_t imgOffset;
		uint32_t size;
		uint8_t	nameLength;
		uint8_t	descriptLength;//CDCDE
		//8
		//id
		//item
	} sdHead;
	typedef struct __attribute__((packed)) {
		uint16_t x;
		uint16_t y;
		uint32_t size;
		//8
		//id
		//item
	} sdTypeDir;
	typedef struct __attribute__((packed)) {
		uint16_t x;
		uint16_t y;
		uint32_t ID;
		uint32_t offset;
		//8
		//id
		//item
	} sdTypeImg;
	
	typedef union {
		struct {
			uint8_t b;
			uint8_t g;
			uint8_t r;
			uint8_t a;
		};
		uint32_t color;
	} argb;
	typedef struct {
		char r;
		char g;
		char b;
	} rgb;
	typedef union {
		struct {
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};
		uint32_t color;
	} abgr;
	typedef struct {
		uint8_t b;
		uint8_t g;
		uint8_t r;
	} bgr;
	typedef uint8_t monochrome;
	
	

	typedef struct{
		enum TYPE {
			PIXELS,
			INCHES,
			CENTIMETERS,
			MILIMETERS,
			POINTS,
			PICAS,
			PERCENTAGES
		};
		TYPE type;
		union {
			int pixels;
			float inches;
			float centimeters;
			float milimeters;
			float points;
			float picas;
			float percentages;
		};
	} Metric;
	void metricToPixels(Metric *m, int v1=0);
	//template class EventDispatcher<EventFileLoad>;
	#ifdef WIN32
	class System {
	  public:
		static HINSTANCE hInstance;
		static void init();
	};
	#endif
	
	enum ASCII_SPEC:char {
		NUL=0x00,//ignore
		SOH=0x01,//ignore
		STX=0x02,//ignore
		ETX=0x03,//ignore
		EOT=0x04,//ignore
		ENQ=0x05,//ignore
		ACK=0x06,//ignore
		BEL=0x07,//ignore
		BS=0x08,//РІРѕР·РІСЂР°С‚ РЅР° РѕРґРёРЅ СЃРёРјРІРѕР»
		TAB=0x09,//РіРѕСЂРёР·РѕРЅС‚Р°Р»СЊРЅР°СЏ С‚Р°Р±СѓР»СЏС†РёСЏ
		LF=0x0A,//РїРµСЂРµРІРѕРґ СЃС‚СЂРѕРєРё
		VT=0x0B,//РІРµСЂС‚РёРєР°Р»СЊРЅР°СЏ С‚Р°Р±СѓР»СЏС†РёСЏ
		FF=0x0C,//ignore
		CR=0x0D,//ignore РІРѕР·РІСЂР°С‚ РєР°СЂРµС‚РєРё
		SO=0x0E,//
		SI=0x0F,//
		DLE=0x10,//ignore
		DC1=0x11,//ignore
		DC2=0x12,//ignore
		DC3=0x13,//ignore
		DC4=0x14,//ignore
		NAK=0x15,//ignore
		SYN=0x16,//ignore
		ETB=0x17,//ignore
		CAN=0x18,//ignore
		EM=0x19,//ignore
		SUB=0x1A,//ignore
		ESC=0x1B,//ignore
		FS=0x1C,//ignore
		GS=0x1D,//ignore
		RS=0x1E,//ignore
		US=0x1F,//ignore
		DEL=0x7F//ignore
	};
}
string get_ext(const string& st);
string get_name(const string& st);
#include "grOpenGL.h"
#include "grShape.h"
#include "grWindows.h"
#include "enTexture.h"
#include "enText.h"


#endif	/* GRBASETYPES_H */