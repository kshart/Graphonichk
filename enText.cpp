#include "grBaseTypes.h"
using namespace std;
using namespace Graphonichk;


// pos short x y
// index i

// indexed 1DBuffer
// float x1 float y1 float x2 float y2
vector<Font*> Font::buffer;
FT_Library Font::library;
TextFormat *TextFormat::defaultFormat = new TextFormat();


TextFormat::TextFormat() {
	this->fn = NULL;
	this->size = 50;
	this->direction = DIR::LTR;
	this->lineHeight = __SHRT_MAX__;
	this->tabSize = 4;
	this->whiteSpace = __SHRT_MAX__;
	this->textDecorationColor.color = 0x00FF00AA;
	
}
FontFace::FontFace(unsigned short size, Array<FontGlyph> *arr) :tex(0), texCoord(0), size(size), arr(arr) {
	
}

struct stNode;
struct stNode {
	struct stNode* child[2];
	unsigned short imgWidth, imgHeight, imgID;
};
typedef struct stNode Node;
typedef struct {
	Node* node;
	unsigned short outPix;
	bool rotate90;
} NodeResult;
int addImage(Array<Rect> *rects, unsigned short imgID, unsigned short viewWidth, unsigned short viewHeight,Node *in, NodeResult *result) {
	unsigned short thisOutPix;
	if ( rects->data[imgID].width <= viewWidth && rects->data[imgID].height <= viewHeight ) {
		if (in->child[0]!=NULL&&in->child[1]!=NULL) {
			addImage(rects, imgID, viewWidth-in->imgWidth, in->imgHeight, in->child[0], result);
			addImage(rects, imgID, viewWidth, viewHeight-in->imgHeight, in->child[1], result);
		}else if ( in->child[0]==NULL && in->child[1]==NULL ) {
			thisOutPix = viewWidth-rects->data[imgID].width;
			if (thisOutPix < result->outPix) {
				//printf("%i %i", in, result->node);
				result->node = in;
				result->outPix = thisOutPix;
				result->rotate90 = false;
			}
		}else if (in->child[0]!=NULL&&in->child[1]==NULL) {
			addImage(rects, imgID, viewWidth-in->imgWidth, viewHeight, in->child[0], result);
		}
	}else{
		
	}
	return true;
}
int addNodeInResult(Array<Rect> *rects, unsigned short imgID, NodeResult *result) {
	Node *node = result->node;
	node->imgWidth = rects->data[imgID].width;
	node->imgHeight = rects->data[imgID].height;
	node->imgID = imgID;
	if (node->child[0]!=NULL||node->child[1]!=NULL) return 1;//printf("ERRROORRR\n");
	if (result->outPix==0) {
		node->child[0] = new Node();
		node->child[0]->imgID = SHRT_MAX;
		node->child[0]->imgHeight = 0;
		node->child[0]->imgWidth = 0;
		node->child[0]->child[0] = NULL;
		node->child[0]->child[1] = NULL;
		node->child[1] = NULL;
		printf("imgID1 %i\n", imgID);
	}else{
		node->child[0] = new Node();
		node->child[0]->imgID = SHRT_MAX;
		node->child[0]->imgHeight = 0;
		node->child[0]->imgWidth = 0;
		node->child[0]->child[0] = NULL;
		node->child[0]->child[1] = NULL;
		node->child[1] = new Node();
		node->child[1]->imgID = SHRT_MAX;
		node->child[1]->imgHeight = 0;
		node->child[1]->imgWidth = 0;
		node->child[1]->child[0] = NULL;
		node->child[1]->child[1] = NULL;
		if (node->child[0]==NULL||node->child[1]==NULL) printf("ERRROORRR\n"); 
		printf("imgID2 %i\n", imgID);
	}
	return true;
}
void traceNods(unsigned short viewX, unsigned short viewY, unsigned short viewWidth, unsigned short viewHeight, Node *in, Array<Rect> *rects) {
	if (in->child[0]==NULL&&in->child[1]==NULL) {
	}else if (in->child[0]!=NULL&&in->child[1]==NULL) {
		rects->data[in->imgID].x = viewX;
		rects->data[in->imgID].y = viewY;
		traceNods(viewX+in->imgWidth, viewY, viewWidth-in->imgWidth, viewHeight, in->child[0], rects);
	}else if (in->child[0]!=NULL&&in->child[1]!=NULL) {
		traceNods(viewX+in->imgWidth, viewY, viewWidth-in->imgWidth, viewHeight, in->child[0], rects);
		traceNods(viewX, viewY+in->imgHeight, viewWidth, viewHeight-in->imgHeight, in->child[1], rects);
		if (viewY+rects->data[in->imgID].height>2048) {
			printf("1232123 %i \n", in->imgID);
		}else{
			rects->data[in->imgID].x = viewX;
			rects->data[in->imgID].y = viewY;
		}
	}
}

FontFaceLoadTask::FontFaceLoadTask(FontFace *face, size_t sizeTexCoord) :bmpTexCoord(sizeTexCoord), face(face) {
	
}
int FontFaceLoadTask::processExecute() {
	glGenTextures(1, &this->face->texCoord);
	glBindTexture(GL_TEXTURE_1D, this->face->texCoord );
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, this->bmpTexCoord.size/4, 0, GL_RGBA, GL_FLOAT, this->bmpTexCoord.data);
}

Font::Font(string path) {
	this->path = path;
	this->error = FT_New_Face( Font::library, path.c_str(), 0, &this->face );
	if (this->error != FT_Err_Ok) return;
	//error = FT_Set_Pixel_Sizes(face, 0, 20);
	/*
	FontGlyphs *fgs = new FontGlyphs;
	fgs->size = 50;
	fgs->count = this->face->num_glyphs;
	fgs->arr = (FontGlyph*)malloc(sizeof(FontGlyph)*fgs->count);
	fgs->ramUsed = sizeof(FontGlyphs)+sizeof(FontGlyph)*fgs->count;
	for(int i=0; i<this->face->num_glyphs; i++) {
		FT_Load_Glyph( this->face, i, FT_LOAD_DEFAULT );
		FT_Render_Glyph( this->face->glyph, FT_RENDER_MODE_LIGHT   );
		fgs->arr[i].bmpWidth = this->face->glyph->bitmap.width;
		fgs->arr[i].bmpHeight = this->face->glyph->bitmap.rows;
		fgs->arr[i].bmp = malloc(fgs->arr[i].bmpWidth*fgs->arr[i].bmpHeight);
		memcpy(fgs->arr[i].bmp, this->face->glyph->bitmap.buffer, fgs->arr[i].bmpWidth*fgs->arr[i].bmpHeight);
		fgs->arr[i].width = this->face->glyph->metrics.width >> 6;
		fgs->arr[i].height = this->face->glyph->metrics.height >> 6;
		fgs->arr[i].horiAdvance = this->face->glyph->metrics.horiAdvance >> 6;
		fgs->arr[i].horiBearingX = this->face->glyph->metrics.horiBearingX >> 6;
		fgs->arr[i].horiBearingY = this->face->glyph->metrics.horiBearingY >> 6;
		fgs->arr[i].vertAdvance = this->face->glyph->metrics.vertAdvance >> 6;
		fgs->arr[i].vertBearingX = this->face->glyph->metrics.vertBearingX >> 6;
		fgs->arr[i].vertBearingY = this->face->glyph->metrics.vertBearingY >> 6;
		fgs->ramUsed+=fgs->arr[i].bmpWidth*fgs->arr[i].bmpHeight;
	} 
	this->cache.push_back(fgs[0]);*/
	Font::buffer.push_back(this);
}
bool Font::cached(unsigned short size) {
	this->error = FT_Set_Pixel_Sizes(this->face, 0, size);
	if (this->error != FT_Err_Ok) return false;
	int glyphCount = this->face->num_glyphs;
	FontFace *fface = new FontFace(size, new Array<FontGlyph>(glyphCount));
	fface->ramUsed = sizeof(Array<FontGlyph>)+sizeof(FontGlyph)*glyphCount;
	int pix=0, maxWidth = 0, maxHeight = 0, mwid, mhid;
	char *imgRaw = (char*)malloc(2048*2048);
	Array<void*> bmps(glyphCount);
	Array<Rect> bmpRect(glyphCount);
	for (uint i=0; i<2048*2048; i++) imgRaw[i] = 0x55;
	
	FontFaceLoadTask *task = new FontFaceLoadTask(fface, glyphCount*4);
	Node node;
	node.imgHeight = 0;
	node.imgWidth = 0;
	node.imgID = SHRT_MAX;
	node.child[0] = NULL;
	node.child[1] = NULL;
	
	for(int i=0; i<this->face->num_glyphs; i++) {
		FT_Load_Glyph( this->face, i, FT_LOAD_DEFAULT );
		FT_Render_Glyph( this->face->glyph, FT_RENDER_MODE_LIGHT   );
		bmpRect.data[i].width = this->face->glyph->bitmap.width;
		bmpRect.data[i].height = this->face->glyph->bitmap.rows;
		bmps.data[i] = malloc(bmpRect.data[i].width*bmpRect.data[i].height);
		memcpy(bmps.data[i], this->face->glyph->bitmap.buffer, bmpRect.data[i].width*bmpRect.data[i].height);
		fface->arr->data[i].width = this->face->glyph->metrics.width >> 6;
		fface->arr->data[i].height = this->face->glyph->metrics.height >> 6;
		fface->arr->data[i].horiAdvance = this->face->glyph->metrics.horiAdvance >> 6;
		fface->arr->data[i].horiBearingX = this->face->glyph->metrics.horiBearingX >> 6;
		fface->arr->data[i].horiBearingY = this->face->glyph->metrics.horiBearingY >> 6;
		fface->arr->data[i].vertAdvance = this->face->glyph->metrics.vertAdvance >> 6;
		fface->arr->data[i].vertBearingX = this->face->glyph->metrics.vertBearingX >> 6;
		fface->arr->data[i].vertBearingY = this->face->glyph->metrics.vertBearingY >> 6;
		fface->ramUsed+=bmpRect.data[i].width*bmpRect.data[i].height;
		
		/*if (fface->arr->data[i].bmpWidth>maxWidth) {
			maxWidth = fface->arr->data[i].bmpWidth;
			mwid = i;
		}
		if (fface->arr->data[i].bmpHeight>maxHeight) {
			maxHeight = fface->arr->data[i].bmpHeight;
			mhid = i;
		}
		pix += fface->arr->data[i].bmpWidth*fface->arr->data[i].bmpHeight;*/
		
		NodeResult nodeRes;
		nodeRes.node = &node;
		nodeRes.outPix = SHRT_MAX;
		nodeRes.rotate90 = false;
		
		addImage(&bmpRect, i, 2048, 2048, &node, &nodeRes);
		//printf("%i %i\n", i, nodeRes.node);
		addNodeInResult(&bmpRect, i, &nodeRes);
	}
	printf("%i\n", this->face->num_glyphs);
	traceNods(0, 0, 2048, 2048, &node, &bmpRect);
	
	for(int i=0; i<this->face->num_glyphs; i++) {
		for (int y=0; y<bmpRect.data[i].height; y++) {
			memcpy( (void*)( (ptrdiff_t)imgRaw + (y+bmpRect.data[i].y)*2048 + bmpRect.data[i].x ), 
					(void*)( (ptrdiff_t)bmps.data[i] + bmpRect.data[i].width*y ), 
					bmpRect.data[i].width );
		}
		task->bmpTexCoord.data[i*4+0] = ((float)bmpRect.data[i].x)/2048;
		task->bmpTexCoord.data[i*4+1] = ((float)bmpRect.data[i].y)/2048;
		task->bmpTexCoord.data[i*4+2] = (float)(bmpRect.data[i].x+bmpRect.data[i].width)/2048;
		task->bmpTexCoord.data[i*4+3] = (float)(bmpRect.data[i].y+bmpRect.data[i].height)/2048;
	}
	Image *img = new Image(2048, 2048, Image::MONO_8, imgRaw);
	fface->tex = new Texture(img);
	Windows::window->eachFrame.addTask(task, 0);
	this->cache.push_back(fface);
}
void Font::trace() {
	printf("<Fonts count='%i'>\n", Font::buffer.size());
	//for (vector<ShapeRect*>::const_iterator it=this->child.begin(), end=this->child.end(); it!=end; ++it) {
	for(vector<Font*>::const_iterator it=Font::buffer.begin(), end=Font::buffer.end(); it!=end; ++it) {
		printf("\t<Font path='%s' cached='%i' error='%i'>\n", (*it)->path.c_str(), (*it)->cache.size(), (*it)->error);
		for(int t=0; t<(*it)->cache.size(); t++) {
			printf("\t\t<FontFace size='%i' count='%i' ramUsed='%iKb'/>\n", (*it)->cache[t]->size, (*it)->cache[t]->arr->size, (*it)->cache[t]->ramUsed/1024);
		}
		printf("\t</Font>\n");
	}
	printf("</Fonts>\n");
}
int Font::init() {
	FT_Init_FreeType( &(Font::library) );
}
FontFace* Font::getFontFace(unsigned short size) {
	for(int i=this->cache.size()-1; i>=0; i--) {
		if (this->cache[i]->size == size) return this->cache[i];
	}
	return NULL;
}

TextField::TextField(unsigned short w, unsigned short h) :ShapeRect(0), vao(0) {
	//this->bufferTexture = new Texture(w, h, GL_ALPHA, GL_UNSIGNED_BYTE);
	this->padding = this->paddingLeft = this->paddingRight = this->paddingTop = this->paddingBottom = 0;
	this->width = w;
	this->height = h;
	this->tf = NULL;
	//this->style
	//this->style.borderRight = this->fn->cache[0].arr[ FT_Get_Char_Index(this->fn->face, ' ')].horiAdvance;
	//this->style.borderTop = this->style.borderBottom = this->style.lineHeight = this->fn->cache[0].size*1.5;
}
void TextField::setString(string str) {
	this->strUTF8 = str;
	this->bufferMode(true);
}
bool TextField::bufferMode(bool mode) {
	/*if (mode && !this->bufferActivate ) {
		Windows::window->FBOBuffer.push_back(this);
		
		this->bufferActivate = true;
		this->bufferInit = false;
	}else if ( !mode && this->bufferActivate ) {
		this->bufferActivate = false;
		delete this->bufferTexture;
		this->bufferTexture = NULL;
	}*/
	return false;
}
void TextField::trace() {
	//printf("<Bitmap x='%i' y='%i' gx='%i' gy='%i' w='%i' h='%i' texId='%i'/>\n", this->x, this->y, this->globalx, this->globaly, this->width, this->height, this->tex);
}
int TextField::bufferGLComptAll() {
	if (this->bufferTexture->GLID == 0) return false; 
		glBindFramebuffer(GL_FRAMEBUFFER, this->bufferFrame);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, this->bufferTexture->GLID, 0);
		glViewport(0, 0, this->width, this->height);
		
		ViewMatrix matrix = ViewMatrixOrtho(0, this->width, this->height, 0, -1, 1);
		OpenGL::setViewMatrix(matrix);
		glClearColor(0,0,0,0);
		glClear( GL_COLOR_BUFFER_BIT );
		
		glEnable( GL_BLEND );
		glEnable( GL_ALPHA_TEST );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBegin(GL_LINES);
			glVertex2s(0, 0);
			glVertex2s(100, 100);
		glEnd();
		printf("|| %i %i %i %i ||\n", this->bufferTexture->width, this->bufferTexture->height, this->width, this->height );
		TextFormat *format;
		Font *font;
		FontFace *fface;
		int lineHeight, lastID=0, id, bx, by, ch;
		if (this->tf == NULL) {format=TextFormat::defaultFormat;}else{format=this->tf;}
		if (format->fn == NULL) {
			if (Font::buffer.empty()) return false;
			font = Font::buffer[0];
		}else{
			font = format->fn;
		}
		if (format->lineHeight == __SHRT_MAX__) { lineHeight = format->size*1.5; }else{ lineHeight = format->lineHeight; }
		bx = this->paddingLeft;
		by = this->paddingTop+lineHeight;
		fface = font->getFontFace(format->size);
		
		
		this->bufferTexture->trace();
		printf("TextField str=%s\n", this->strUTF8.c_str());
		
	if (fface == NULL) {// <editor-fold defaultstate="collapsed" desc="CACHE OFF">
		FontGlyph spaceGlyph, lastGlyph;
		if ( FT_Set_Char_Size(font->face, 0, format->size*64, Windows::window->dpi, Windows::window->dpi ) != FT_Err_Ok) return false;
		FT_Load_Glyph(font->face, FT_Get_Char_Index(font->face, ' '), FT_LOAD_DEFAULT);
		FT_Render_Glyph(font->face->glyph, FT_RENDER_MODE_LIGHT);
		/*spaceGlyph.bmpWidth = font->face->glyph->bitmap.width;
		spaceGlyph.bmpHeight = font->face->glyph->bitmap.rows;
		spaceGlyph.bmp = malloc(spaceGlyph.bmpWidth*spaceGlyph.bmpHeight);
		memcpy(spaceGlyph.bmp, font->face->glyph->bitmap.buffer, spaceGlyph.bmpWidth*spaceGlyph.bmpHeight);*/
		spaceGlyph.width		= font->face->glyph->metrics.width >> 6;
		spaceGlyph.height		= font->face->glyph->metrics.height >> 6;
		spaceGlyph.horiAdvance	= font->face->glyph->metrics.horiAdvance >> 6;
		spaceGlyph.horiBearingX	= font->face->glyph->metrics.horiBearingX >> 6;
		spaceGlyph.horiBearingY	= font->face->glyph->metrics.horiBearingY >> 6;
		spaceGlyph.vertAdvance	= font->face->glyph->metrics.vertAdvance >> 6;
		spaceGlyph.vertBearingX	= font->face->glyph->metrics.vertBearingX >> 6;
		spaceGlyph.vertBearingY	= font->face->glyph->metrics.vertBearingY >> 6;
		
		for(int i=0; i<this->strUTF8.length(); i++) {
			if ( (unsigned char)(this->strUTF8[i])<0x20) {
				switch (this->strUTF8[i]) {
					case ASCII_SPEC::LF:
						bx = this->paddingLeft;
						by += lineHeight;
						break;
					case ASCII_SPEC::TAB:
						bx += spaceGlyph.horiAdvance * format->tabSize;
						break;
				}
				lastID = 0;
				printf(" %#02x", this->strUTF8[i]);
			}else{
				if ( (unsigned char)(this->strUTF8[i])<0x80 ) {//utf8-1 0xxx-xxxx
					ch = this->strUTF8[i];
					id = FT_Get_Char_Index(font->face, this->strUTF8[i]);
				}else if ( (unsigned char)(this->strUTF8[i])<0xE0 ) {//utf8-2 110x-xxxx 10xx-xxxx
					ch = (this->strUTF8[i]&0x1F)<<6 | (this->strUTF8[i+1]&0x3F);
					id = FT_Get_Char_Index(font->face, ch);
					i++;
				}else if ( (unsigned char)(this->strUTF8[i])<0xF0 ) {//utf8-3 1110-xxxx 10xx-xxxx 10xx-xxxx
					ch = (this->strUTF8[i]&0x0F)<<12 | (this->strUTF8[i+1]&0x3F)<<6 | (this->strUTF8[i+2]&0x3F );
					id = FT_Get_Char_Index(font->face, ch);
					i+=2;
				}else if ( (unsigned char)(this->strUTF8[i])<0xF8 ) {//utf8-4 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
					ch = (this->strUTF8[i]&0x0F)<<18 | (this->strUTF8[i+1]&0x3F)<<12 | (this->strUTF8[i+1]&0x3F)<<6 | (this->strUTF8[i+2]&0x3F);
					id = FT_Get_Char_Index(font->face, ch);
					i+=3;
				}
				FT_Load_Glyph( font->face, id, FT_LOAD_DEFAULT );
				FT_Render_Glyph( font->face->glyph, FT_RENDER_MODE_LIGHT   );
				if (font->face->glyph->metrics.horiAdvance > 0) {
					lastID = ch;
					/*//lastGlyph.bmpWidth	= font->face->glyph->bitmap.width;
					//lastGlyph.bmpHeight	= font->face->glyph->bitmap.rows;
					lastGlyph.bmp		= malloc(lastGlyph.bmpWidth*lastGlyph.bmpHeight);
					memcpy(lastGlyph.bmp, font->face->glyph->bitmap.buffer, lastGlyph.bmpWidth*lastGlyph.bmpHeight);*/
					lastGlyph.width			= font->face->glyph->metrics.width >> 6;
					lastGlyph.height		= font->face->glyph->metrics.height >> 6;
					lastGlyph.horiAdvance	= font->face->glyph->metrics.horiAdvance >> 6;
					lastGlyph.horiBearingX	= font->face->glyph->metrics.horiBearingX >> 6;
					lastGlyph.horiBearingY	= font->face->glyph->metrics.horiBearingY >> 6;
					lastGlyph.vertAdvance	= font->face->glyph->metrics.vertAdvance >> 6;
					lastGlyph.vertBearingX	= font->face->glyph->metrics.vertBearingX >> 6;
					lastGlyph.vertBearingY	= font->face->glyph->metrics.vertBearingY >> 6;
					glRasterPos2s( bx+lastGlyph.horiBearingX, by-lastGlyph.horiBearingY );
					//glDrawPixels(lastGlyph.bmpWidth, lastGlyph.bmpHeight, GL_ALPHA, GL_UNSIGNED_BYTE, lastGlyph.bmp);
					glBegin(GL_POINTS);
					glVertex2s(bx, by);
					glEnd();
					bx+=lastGlyph.horiAdvance;
				}else{
					int v1 = lastGlyph.horiBearingX + lastGlyph.width/2 - lastGlyph.horiAdvance;
					//glRasterPos2s(bx+f->cache[0].arr[id].horiBearingX+v1, by - f->cache[0].arr[lastID].horiBearingY - f->cache[0].arr[id].height );
					glRasterPos2s(bx+(font->face->glyph->metrics.horiBearingX>>6)+v1, by-(font->face->glyph->metrics.horiBearingY>>6)-(font->face->glyph->metrics.height>>6) );
					//glDrawPixels(font->face->glyph->bitmap.width, font->face->glyph->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, font->face->glyph->bitmap.buffer);
				}
			}
		}
		// </editor-fold>
	}else{// <editor-fold defaultstate="collapsed" desc="CACHE ON">
		FontGlyph spaceGlyph = fface->arr->data[ FT_Get_Char_Index(font->face, ' ') ];
		for(int i=0; i<this->strUTF8.length(); i++) {
			if ( (unsigned char)(this->strUTF8[i])<0x20) {
				switch (this->strUTF8[i]) {
					case ASCII_SPEC::LF:
						bx = this->paddingLeft;
						by += lineHeight;
						break;
					case ASCII_SPEC::TAB:
						bx += spaceGlyph.horiAdvance * format->tabSize;
						break;
				}
				lastID = 0;
				printf(" %#02x", this->strUTF8[i]);
			}else{
				if ( (unsigned char)(this->strUTF8[i])<0x80 ) {//utf8-1 0xxx-xxxx
					ch = this->strUTF8[i];
					id = FT_Get_Char_Index(font->face, this->strUTF8[i]);
				}else if ( (unsigned char)(this->strUTF8[i])<0xE0 ) {//utf8-2 110x-xxxx 10xx-xxxx
					ch = (this->strUTF8[i]&0x1F)<<6 | (this->strUTF8[i+1]&0x3F);
					id = FT_Get_Char_Index(font->face, ch);
					i++;
				}else if ( (unsigned char)(this->strUTF8[i])<0xF0 ) {//utf8-3 1110-xxxx 10xx-xxxx 10xx-xxxx
					ch = (this->strUTF8[i]&0x0F)<<12 | (this->strUTF8[i+1]&0x3F)<<6 | (this->strUTF8[i+2]&0x3F );
					id = FT_Get_Char_Index(font->face, ch);
					i+=2;
				}else if ( (unsigned char)(this->strUTF8[i])<0xF8 ) {//utf8-4 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
					ch = (this->strUTF8[i]&0x0F)<<18 | (this->strUTF8[i+1]&0x3F)<<12 | (this->strUTF8[i+1]&0x3F)<<6 | (this->strUTF8[i+2]&0x3F);
					id = FT_Get_Char_Index(font->face, ch);
					i+=3;
				}
				if (fface->arr->data[id].horiAdvance > 0) {
					lastID = ch;
					glRasterPos2s( bx+fface->arr->data[id].horiBearingX, by-fface->arr->data[id].horiBearingY );
					//glDrawPixels(fface->arr->data[id].bmpWidth, fface->arr->data[id].bmpHeight, GL_ALPHA, GL_UNSIGNED_BYTE, fface->arr->data[id].bmp);
					glBegin(GL_POINTS);
					glVertex2s(bx, by);
					glEnd();
					bx+=fface->arr->data[id].horiAdvance;
				}else{
					int v1 = fface->arr->data[lastID].horiBearingX + fface->arr->data[lastID].width/2 - fface->arr->data[lastID].horiAdvance;
					//glRasterPos2s(bx+f->cache[0].arr[id].horiBearingX+v1, by - f->cache[0].arr[lastID].horiBearingY - f->cache[0].arr[id].height );
					glRasterPos2s(bx+fface->arr->data[id].horiBearingX+v1, by-fface->arr->data[lastID].horiBearingY - fface->arr->data[id].height );
					//glDrawPixels(fface->arr->data[id].bmpWidth, fface->arr->data[id].bmpHeight, GL_ALPHA, GL_UNSIGNED_BYTE, fface->arr->data[id].bmp);
				}
			}
		}
	}// </editor-fold>
	/*if ( ch == 0x0306 ) {
								FontGlyph fg = f->cache[0].arr[id], lfg = f->cache[0].arr[lastID];
								int v1, v2;
								v1 = lfg.horiBearingX + lfg.width/2 - lfg.horiAdvance;
								printf("<lfg w='%i' h='%i' hbx='%i' hby='%i' ha='%i' vbx='%i' vby='%i' va='%i'/>\n", lfg.width, lfg.height, lfg.horiBearingX, lfg.horiBearingY, lfg.horiAdvance, lfg.vertBearingX, lfg.vertBearingY, lfg.vertAdvance);
								printf("<0x0306 w='%i' h='%i' hbx='%i' hby='%i' ha='%i' vbx='%i' vby='%i' va='%i'/>\n", fg.width, fg.height, fg.horiBearingX, fg.horiBearingY, fg.horiAdvance, fg.vertBearingX, fg.vertBearingY, fg.vertAdvance);
								glRasterPos2s(bx+fg.horiBearingX+v1, by-lfg.horiBearingY-fg.height );
								glDrawPixels(f->cache[0].arr[id].bmpWidth, f->cache[0].arr[id].bmpHeight, GL_ALPHA, GL_UNSIGNED_BYTE, f->cache[0].arr[id].bmp);
								glBegin(GL_POINTS);
								glVertex2s(bx, by);
								glEnd();
							}else{*/

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glDisable( GL_BLEND );
	glDisable( GL_ALPHA_TEST );
	return true;
}
int TextField::bufferGL400() {
	return false;
}
int TextField::bufferGL330() {
	return false;
}
int TextField::bufferGL210() {
	return false;
}
int TextField::renderGLComptAll() {
	Texture *tex = this->bufferTexture;
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, tex->GLID);
	if (this->tf == NULL) {
		glColor4ub(0xFF,0xFF,0xFF,0xFF);
	}else{
		glColor4ub(this->tf->textDecorationColor.r,this->tf->textDecorationColor.g,this->tf->textDecorationColor.b,this->tf->textDecorationColor.a);
	}
	glBegin( GL_QUADS );
		glTexCoord2d( 0.0, 0.0 );	glVertex2i(this->globalx, this->globaly );
		glTexCoord2d( 0.0, 1.0 );	glVertex2i(this->globalx, this->globaly+tex->height );
		glTexCoord2d( 1.0, 1.0 );	glVertex2i(this->globalx+tex->width, this->globaly+tex->height );
		glTexCoord2d( 1.0, 0.0 );	glVertex2i(this->globalx+tex->width, this->globaly );
	glEnd();
	glDisable( GL_TEXTURE_2D );
	return true;
} 
int TextField::renderGL400() {
	return false;
}
int TextField::renderGL330() {
	TextFormat *format;
	Font *font;
	FontFace *fface;
	int lineHeight, lastID=0, id, bx, by, ch;
	if (this->tf == NULL) {format=TextFormat::defaultFormat;}else{format=this->tf;}
	if (format->fn == NULL) {
		if (Font::buffer.empty()) return false;
		font = Font::buffer[0];
	}else{
		font = format->fn;
	}
	if (format->lineHeight == __SHRT_MAX__) { lineHeight = format->size*1.5; }else{ lineHeight = format->lineHeight; }
	bx = this->paddingLeft;
	by = this->paddingTop+lineHeight;
	format->size = 60;
	fface = font->getFontFace(format->size);
	if (fface==nullptr) return false;
	if (GLShader::shader->crc32!=ShaderTextField::CRC32) GLShader::setShader(ShaderTextField::prog);
	//if (OpenGL::textureGLID!=this->tex->GLID) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fface->tex->GLID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_1D, fface->texCoord);
		glActiveTexture(GL_TEXTURE0);
		//OpenGL::textureGLID = this->tex->GLID;
	//}
	if (this->vao==0) {
		short vertex[6] = {
			this->globalx, this->globaly, FT_Get_Char_Index(font->face, '8'), 
			fface->arr->data[FT_Get_Char_Index(font->face, '8')].width, fface->arr->data[FT_Get_Char_Index(font->face, '8')].height, 0
		};
		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);
		
		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, 6*2, vertex, GL_STATIC_DRAW);
		glVertexAttribPointer(ShaderTextField::prog->position, 3, GL_SHORT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(ShaderTextField::prog->position);
	}
	glBindVertexArray(this->vao);
	glDrawArrays(GL_LINES, 0, 2);
	return false;
}
int TextField::renderGL210() {
	return false;
}
ShaderTextField* ShaderTextField::prog = nullptr;
ShaderTextField::ShaderTextField() :GLShader(ShaderTextField::CRC32) {
	
}
void ShaderTextField::init() {
	this->position = glGetAttribLocation(this->shaderProgram, "position");
	this->texture = glGetUniformLocation(this->shaderProgram, "texture");
	this->coordTex = glGetUniformLocation(this->shaderProgram, "coordTex");
	this->viewMatrix = glGetUniformBlockIndex(this->shaderProgram, "viewMatrix");
	glUniform1i(this->texture , 0);
	glUniform1i(this->coordTex , 1);
	glUniformBlockBinding(this->shaderProgram, this->viewMatrix, 1);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, OpenGL::viewMatrix, 0, 4*4*sizeof(float));
}
void ShaderTextField::init33() {
	ShaderTextField *sh = new ShaderTextField();
	const GLchar *vrsh = 
		"in vec3 position;"
		"void main () {"
			"gl_Position = vec4(position, 1);"
		"}",
		*frsh = 
		"uniform sampler2D texture;"
		"in vec2 coord;"
		"out vec4 color;"
		"void main () {"
			"color = texture(texture, coord);"
		"}",
		*gmsh = 
		"layout (lines) in;"
		"layout (triangle_strip) out;"
		"layout (max_vertices = 4) out;"
		"layout(shared) uniform viewMatrix {"
			"mat4 viewMatrixValue;"
		"};"
		"uniform sampler1D coordTex;"
		"out vec2 coord;"
		"void main () {"
			"vec4 rect = vec4(gl_in[0].gl_Position.xy, gl_in[1].gl_Position.xy);"
			"int index = int(gl_in[0].gl_Position.z);"
			"vec4 rectTex = texelFetch(coordTex, index, 0);"
			"gl_Position = vec4(rect.xy, 0.0, 1.0)*viewMatrixValue;"
			"coord = vec2(rectTex.r, rectTex.g);"
			"EmitVertex();"
			
			"gl_Position = vec4(rect.x, rect.y+rect.w, 0.0, 1.0)*viewMatrixValue;"
			"coord = vec2(rectTex.r, rectTex.a);"
			"EmitVertex();"
			
			"gl_Position = vec4(rect.x+rect.z, rect.y, 0.0, 1.0)*viewMatrixValue;"
			"coord = vec2(rectTex.b, rectTex.g);"
			"EmitVertex();"
			
			"gl_Position = vec4(rect.x+rect.z, rect.y+rect.w, 0.0, 1.0)*viewMatrixValue;"
			"coord = vec2(rectTex.b, rectTex.a);"
			//"gl_PointSize = 10.0;"
			"EmitVertex();"
			"EndPrimitive();"
		"}";
	ShaderTextField::prog = sh;
	GLShaderLoadTask *task = new GLShaderLoadTask(sh, vrsh, frsh, gmsh);
	Windows::window->eachFrame.addTask(task, 0);
}