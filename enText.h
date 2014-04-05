#ifndef GRTEXT_H
#define	GRTEXT_H

#include "grBaseTypes.h"

using namespace std;
namespace Graphonichk {
	class TextField;
	class TextFormat;
	class FontFace;
	class Font;
	typedef struct {
		short ch;
		//void *bmp;
		//unsigned short bmpX, bmpY;
		unsigned short width, height;
		GLuint id;
		short horiBearingX, horiBearingY, horiAdvance, vertBearingX, vertBearingY, vertAdvance;
		
	} FontGlyph;
	template class Array<FontGlyph>;
	
	class FontFace {
	public:
		FontFace(unsigned short size, Array<FontGlyph> *arr);
		unsigned short size;
		unsigned int ramUsed;
		GLuint texCoord;
		Texture *tex;
		Array<FontGlyph> *arr;
	};
	class TextFormat {
	public:
		static TextFormat *defaultFormat;
		TextFormat();
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
		static void trace();
		static int init();
		static FT_Library library;
		static vector<Font*> buffer;
		
		Font(string path);
		bool cached(unsigned short size);
		int error;
		string path;
		FT_Face face;
		vector<FontFace*> cache;
		FontFace *getFontFace(unsigned short size);
	};
	class TextLine :public ShapeBasic {
	public:
		TextLine();
		Font *font;
		TextFormat *format;
		string strUTF8;
	};
	class TextField :public Buffer, public ShapeRect {
	private:
		TextFormat *tf;
	public:
		TextField(unsigned short, unsigned short);
		void trace();
		int renderGLComptAll();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		int bufferGLComptAll();
		int bufferGL400();
		int bufferGL330();
		int bufferGL210();
		bool bufferMode(bool mode);
		
		void setString(string str);
		void setFormat(TextFormat *tf);
		
		string strUTF8;
		/*enum STR_TYPE:char {
			UTF8, UTF16, UTF32
		};
		STR_TYPE strType;*/
		
		GLuint vao, vbo;
		
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
	
	class FontFaceLoadTask :public EachFrameTask {
	public:
		FontFaceLoadTask(FontFace *face, size_t sizeTexCoord);
		int processExecute();
		FontFace *face;
		Array<float> bmpTexCoord;
	};
	class ShaderTextField :public GLShader {
	public:
		enum {CRC32=0x514f2123};
		ShaderTextField();
		//static void init();
		static void init33();
		static ShaderTextField* prog;
		
		GLint texture, coordTex, position, viewMatrix;
		
		friend GLShaderLoadTask;
		void init();
	};
}

#endif	/* GRTEXT_H */

