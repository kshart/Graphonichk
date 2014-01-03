/* 
 * File:   enText.h
 * Author: Артём Каширин
 *
 * Created on 16 Ноябрь 2013 г., 15:10
 */

#ifndef ENTEXT_H
#define	ENTEXT_H

#include "grBaseTypes.h"
#include "grShape.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <GL/gl.h>

using namespace std;
namespace grEngine {
	class TextField;
	class TextFormat;
	class Font;

	typedef struct {//22
		short ch;
		void *bmp;
		unsigned short width, height, bmpWidth, bmpHeight;
		short horiBearingX, horiBearingY, horiAdvance, vertBearingX, vertBearingY, vertAdvance;
		
	} FontGlyph;
	typedef struct {
		unsigned int size, count, ramUsed;
		FontGlyph *arr;
	} FontGlyphs;
	class TextFormat {
	  public:
		TextFormat();
		static TextFormat *globalTextFormat;
		enum DIR:char {
			LTR,
			RTL
		};
		Font *fn;
		//unsigned short strWidth, strHeight;
		unsigned short size;
		DIR direction;
		short letterSpacing;
		short lineHeight;
		unsigned short tabSize;
		unsigned short whiteSpace;
		argb textDecorationColor;
		/*void
			hyphens,
			textAlign,
			textAlignLast,
			textDecoration,
			textDecorationLine,
			textDecorationStyle,
			textIndent,
			textOverflow,
			textShadow,
			textTransform,
			unicodeBidi,
			wordBreak,
			wordSpacing,
			writingMode;*/
	};
	class Font {
	  public:
		static TextFormat *tf;
		static FT_Library library;
		static vector<Font*> fonts;
		static void trace();
		static int init();
		
		Font(string path);
		void cached(unsigned int id);
		int error;
		string path;
		FT_Face face;
		vector<FontGlyphs> cache;
	};
	class TextField :public Buffer, public Shape {
	  private:
		TextFormat *tf;
	  public:
		TextField(short, short, unsigned short, unsigned short);
		void trace();
		int renderGLComptAll();
		int renderGL400();
		int renderGL300();
		int renderGL210();
		int bufferGLComptAll();
		int bufferGL400();
		int bufferGL300();
		int bufferGL210();
		void setString(string str);
		void setFormat(TextFormat *tf);
		
		union {
			//string strUTF8;
			//u16string strUTF16;
			//u32string strUTF32;
		};
		string strUTF8;
		enum STR_TYPE:char {
			UTF8, UTF16, UTF32
		};
		STR_TYPE strType;
		
		unsigned short borderSize;
		char background, multiline;
		unsigned int borderColor, backgroundColor, textColor;
		short padding, paddingLeft, paddingRight, paddingTop, paddingBottom;
		/*struct {
			char decoration, decorationStyle, writingMode, whiteSpace,
				align, alignLast;
			rgb decorationColor;
			short marginLeft, marginRight, marginTop, marginBottom,
				paddingLeft, paddingRight, paddingTop, paddingBottom,
				borderLeft, borderRight, borderTop, borderBottom,
				size,
				letterSpacing,
				lineHeight,
				tabSize,
				indent,
				wordBreak,
				wordSpacing;
		} style;*/
	};
	
	
}

#endif	/* ENTEXT_H */

