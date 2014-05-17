#ifndef GRTEXT_H
#define	GRTEXT_H

#include "grMain.h"

using namespace std;
namespace Graphonichk {
	class TextField;
	class TextFieldTask;
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
		GLuint texCoord = 0;
		Texture *tex = nullptr;
		Array<FontGlyph> arr;
	};
	class TextFormat {
	public:
		static TextFormat *defaultFormat;
		TextFormat();
		Font *fn = nullptr;
		unsigned short size = 40;
		enum DIR:char {
			LTR,
			RTL
		} direction = DIR::LTR;
		short letterSpacing;
		short lineHeight = __SHRT_MAX__;
		unsigned short tabSize = 4;
		unsigned short whiteSpace = __SHRT_MAX__;
		ubvec4 textDecorationColor;
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
		void trace() override;
		int renderGL330() override;
		int renderGL100() override;
		
		string strUTF8;
		/*enum STR_TYPE:char {
			UTF8, UTF16, UTF32
		};
		STR_TYPE strType;*/
		
		Texture *tex = nullptr;
		GLuint vbo;
		
		unsigned short borderSize;
		char background, multiline;
		unsigned int borderColor, backgroundColor, textColor;
		short paddingLeft = 0,
			paddingRight = 0,
			paddingTop = 0,
			paddingBottom = 0;
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
		
		void operator=(const string& right);
		void setFormat(TextFormat *tf);
		void setAligment(POSITION align);
	private:
		TextFormat *_tf;
		uint _symvolCount;
		POSITION position = POSITION::LEFT_TOP;
		friend TextFieldTask;
	};
	
	class TextFieldTask : public EachFrameTask {
	public:
		TextFieldTask(TextField *text);
		int processExecute() override;
		TextField *text;
	};
	class ShaderTextFieldBuffer :public ShaderShRect {
	public:
		enum {CRC32=0x0912D2FF};
		ShaderTextFieldBuffer();
		static ShaderTextFieldBuffer* prog;
		
		GLint textColor, textTexture, texCoord, grShaderData;
		
		friend GLShaderLoadTask;
		void init() override;
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

