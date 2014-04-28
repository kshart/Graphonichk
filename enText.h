#ifndef GRTEXT_H
#define	GRTEXT_H

#include "grMain.h"

using namespace std;
namespace Graphonichk {
	class TextField;
	class TextFormat;
	class FontFace;
	class Font;
	typedef struct {
		short ch;
		void *bmp;
		unsigned short bmpWidth, bmpHeight;
		unsigned short width, height;
		GLuint id;
		short horiBearingX, horiBearingY, horiAdvance, vertBearingX, vertBearingY, vertAdvance;
		
	} FontGlyph;
	template class Array<FontGlyph>;
	
	class FontFace {
	public:
		FontFace(unsigned short size, size_t glyphCount);
		unsigned short size;
		unsigned int ramUsed;
		GLuint texCoord;
		Texture *tex;
		Array<FontGlyph> arr;
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
	/*class CharField :public ShapeRect {
	private:
		Font *font;
		unsigned short fontSize;
	public:
		int renderGL330();
	};*/
	class TextLine :public ShapeBasic {
	public:
		TextLine();
		Font *font;
		TextFormat *format;
		string strUTF8;
	};
	class TextField :public ShapeRect {
	private:
		TextFormat *_tf;
		uint _symvolCount;
	public:
		enum POSITION {
			LEFT_TOP,
			LEFT_BOTTOM,
			LEFT_CENTER,
			RIGHT_TOP,
			RIGHT_BOTTOM,
			RIGHT_CENTER,
			TOP_CENTER,
			BOTTOM_CENTER,
			CENTER
		};
		TextField(unsigned short, unsigned short);
		void trace();
		int renderGLComptAll();
		int renderGL400();
		int renderGL330();
		int renderGL210();
		
		void setString(string str);
		void setFormat(TextFormat *tf);
		
		string strUTF8;
		/*enum STR_TYPE:char {
			UTF8, UTF16, UTF32
		};
		STR_TYPE strType;*/
		
		Texture *tex;
		GLuint vbo;
		
		POSITION position;
		unsigned short borderSize;
		char background, multiline;
		unsigned int borderColor, backgroundColor, textColor;
		short paddingLeft, paddingRight, paddingTop, paddingBottom;
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
		Array<uint16_t> bmpTexCoord;
	};
	class ShaderTextField :public GLShader {
	public:
		enum {CRC32=0x51923};
		ShaderTextField();
		//static void init();
		static void init33();
		static ShaderTextField* prog;
		
		GLint position, texture, coordTex, grShaderData;
		
		friend GLShaderLoadTask;
		void init();
	};
	class ShaderTextFieldBuffer :public ShaderShRect {
	public:
		enum {CRC32=0x0912D2FF};
		ShaderTextFieldBuffer();
		//static void init();
		static void init33();
		static ShaderTextFieldBuffer* prog;
		
		GLint textColor, textTexture, texCoord, grShaderData;
		
		friend GLShaderLoadTask;
		void init();
	};
}

using namespace Graphonichk;

namespace grAlgoritm {
	class CompositionRect;
	class CompositionRectNode;
	class CompositionRectNodeResult;
	
	class CompositionRectNode {
	public:
		CompositionRectNode() :imgWidth(0), imgHeight(0), imgID(SHRT_MAX), rotate90(false) {
			this->child[0] = nullptr;
			this->child[1] = nullptr;
		}
		
		int addNode(Array<Rect> &rects, unsigned short imgID, unsigned short viewWidth, unsigned short viewHeight, CompositionRectNodeResult *result);
		void trace(unsigned short viewX, unsigned short viewY, unsigned short viewWidth, unsigned short viewHeight, Array<Rect> &rects) {
			if (this->child[0]==NULL&&this->child[1]==NULL) {
			}else if (this->child[0]!=NULL&&this->child[1]==NULL) {
				rects[this->imgID].x = viewX;
				rects[this->imgID].y = viewY;
				this->child[0]->trace(viewX+this->imgWidth, viewY, viewWidth-this->imgWidth, viewHeight, rects);
			}else if (this->child[0]!=NULL&&this->child[1]!=NULL) {
				this->child[0]->trace(viewX+this->imgWidth, viewY, viewWidth-this->imgWidth, viewHeight, rects);
				this->child[1]->trace(viewX, viewY+this->imgHeight, viewWidth, viewHeight-this->imgHeight, rects);
				if (viewY+rects[this->imgID].height>2048) {
					printf("1232123 %i \n", this->imgID);
				}else{
					rects[this->imgID].x = viewX;
					rects[this->imgID].y = viewY;
				}
			}
		}
		
		CompositionRectNode* child[2];
		unsigned short imgWidth, imgHeight, imgID;
		bool rotate90;
	};
	class CompositionRectNodeResult {
	public:
		CompositionRectNode* node;
		unsigned short outPix;
		bool rotate90;
	};
	class CompositionRect {
	private:
		unsigned short viewWidth, viewHeight;
		CompositionRectNode node;
		CompositionRectNodeResult result; 
	public:
		CompositionRect(unsigned short w, unsigned short h);
		
		int addNode(Array<Rect> &rects, unsigned short imgID);
		void trace(Array<Rect> &rects);
	};
	
}
#endif	/* GRTEXT_H */

