#include "grMain.h"
using namespace std;
using namespace Graphonichk;

grAlgoritm::CompositionRect::CompositionRect(unsigned short w, unsigned short h) :viewWidth(w), viewHeight(h) {
	
}
int grAlgoritm::CompositionRect::addNode(Array<Rect> &rects, unsigned short imgID) {
	this->result.outPix = SHRT_MAX;
	this->result.rotate90 = false;
	this->result.node = nullptr;
	
	this->node.addNode(rects, imgID, this->viewWidth, this->viewHeight, &this->result);
	
	CompositionRectNode *rn = this->result.node;
	if (rn->child[0]!=NULL||rn->child[1]!=NULL) return false;//printf("ERRROORRR\n");
	rn->imgWidth = rects[imgID].width+1;
	rn->imgHeight = rects[imgID].height+1;
	rn->imgID = imgID;
	if (this->result.outPix==0) {
		rn->child[0] = new CompositionRectNode();
		rn->child[1] = nullptr;
	}else{
		rn->child[0] = new CompositionRectNode();
		rn->child[1] = new CompositionRectNode();
		if (this->node.child[0]==NULL||this->node.child[1]==NULL) printf("ERRROORRR\n");
	}
	return true;
}
void grAlgoritm::CompositionRect::trace(Array<Rect> &rects) {
	this->node.trace(0, 0, this->viewWidth, this->viewHeight, rects);
}
int grAlgoritm::CompositionRectNode::addNode(Array<Rect> &rects, unsigned short imgID, unsigned short viewWidth, unsigned short viewHeight, grAlgoritm::CompositionRectNodeResult* result) {
	unsigned short thisOutPix;
	if ( rects[imgID].width+1 <= viewWidth && rects[imgID].height+1 <= viewHeight ) {
		if (this->child[0]!=NULL&&this->child[1]!=NULL) {
			this->child[0]->addNode(rects, imgID, viewWidth-this->imgWidth, this->imgHeight, result);
			this->child[1]->addNode(rects, imgID, viewWidth, viewHeight-this->imgHeight, result);
		}else if ( this->child[0]==NULL && this->child[1]==NULL ) {
			thisOutPix = viewWidth-rects[imgID].width+1;
			if (thisOutPix < result->outPix) {
				result->node = this;
				result->outPix = thisOutPix;
				result->rotate90 = false;
			}
		}else if (this->child[0]!=NULL&&this->child[1]==NULL) {
			this->child[0]->addNode(rects, imgID, viewWidth-this->imgWidth, viewHeight, result);
		}
	}else{

	}
	return true;
}

// pos short x y
// index i

// indexed 1DBuffer
// float x1 float y1 float x2 float y2
vector<Font*> Font::buffer;
FT_Library Font::library;
TextFormat *TextFormat::defaultFormat = new TextFormat();


TextFormat::TextFormat() {
	this->textDecorationColor = ubvec4(0, 0xFF, 0, 0xAA);
}
FontFace::FontFace(unsigned short size, size_t glyphCount) :size(size), arr(glyphCount) {
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
	FontFace *fface = new FontFace(size, glyphCount);
	fface->ramUsed = sizeof(Array<FontGlyph>)+sizeof(FontGlyph)*glyphCount;
	int pix=0, maxWidth = 0, maxHeight = 0, mwid, mhid;
	char *imgRaw = (char*)malloc(2048*2048);
	Array<void*> bmps(glyphCount);
	Array<Rect> bmpRect(glyphCount);
	for (uint i=0; i<2048*2048; i++) imgRaw[i] = 0x55;
	
	usvec4 *rectTexCoord = (usvec4*)malloc(glyphCount*sizeof(usvec4));
	grAlgoritm::CompositionRect rectNode(2048, 2048);
	
	for(int i=0; i<this->face->num_glyphs; i++) {
		FT_Load_Glyph( this->face, i, FT_LOAD_DEFAULT );
		FT_Render_Glyph( this->face->glyph, FT_RENDER_MODE_LIGHT   );
		bmpRect.data[i].width = this->face->glyph->bitmap.width;
		bmpRect.data[i].height = this->face->glyph->bitmap.rows;
		bmps.data[i] = malloc(bmpRect.data[i].width*bmpRect.data[i].height);
		memcpy(bmps.data[i], this->face->glyph->bitmap.buffer, bmpRect.data[i].width*bmpRect.data[i].height);
		fface->arr.data[i].bmpWidth = this->face->glyph->bitmap.width;
		fface->arr.data[i].bmpHeight = this->face->glyph->bitmap.rows;
		fface->arr.data[i].width = this->face->glyph->metrics.width >> 6;
		fface->arr.data[i].height = this->face->glyph->metrics.height >> 6;
		fface->arr.data[i].horiAdvance = this->face->glyph->metrics.horiAdvance >> 6;
		fface->arr.data[i].horiBearingX = this->face->glyph->metrics.horiBearingX >> 6;
		fface->arr.data[i].horiBearingY = this->face->glyph->metrics.horiBearingY >> 6;
		fface->arr.data[i].vertAdvance = this->face->glyph->metrics.vertAdvance >> 6;
		fface->arr.data[i].vertBearingX = this->face->glyph->metrics.vertBearingX >> 6;
		fface->arr.data[i].vertBearingY = this->face->glyph->metrics.vertBearingY >> 6;
		fface->ramUsed+=bmpRect.data[i].width*bmpRect.data[i].height;
		rectNode.addNode(bmpRect, i);
	}
	rectNode.trace(bmpRect);
	
	for(int i=0; i<this->face->num_glyphs; i++) {
		for (int y=0; y<bmpRect.data[i].height; y++) {
			memcpy( (void*)( (ptrdiff_t)imgRaw + (y+bmpRect.data[i].y)*2048 + bmpRect.data[i].x ), 
					(void*)( (ptrdiff_t)bmps.data[i] + bmpRect.data[i].width*y ), 
					bmpRect.data[i].width );
		}
		rectTexCoord[i].x = bmpRect.data[i].x;
		rectTexCoord[i].y = bmpRect.data[i].y;
		rectTexCoord[i].z = bmpRect.data[i].x+bmpRect.data[i].width;
		rectTexCoord[i].w = bmpRect.data[i].y+bmpRect.data[i].height;
	}
	Image *img = new Image(2048, 2048, Image::ALPHA_8, imgRaw);
	fface->tex = new Texture(img, glyphCount, rectTexCoord);
	this->cache.push_back(fface);
	delete rectTexCoord;
	return true;
}
void Font::trace() {
	printf("<Fonts count='%i'>\n", Font::buffer.size());
	//for (vector<ShapeRect*>::const_iterator it=this->child.begin(), end=this->child.end(); it!=end; ++it) {
	for(Font* &font : Font::buffer) {
		printf("\t<Font path='%s' cached='%i' error='%i'>\n", font->path.c_str(), font->cache.size(), font->error);
		for(int t=0; t<font->cache.size(); t++) {
			printf("\t\t<FontFace size='%i' count='%i' ramUsed='%iKb'/>\n", font->cache[t]->size, font->cache[t]->arr.size, font->cache[t]->ramUsed/1024);
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

TextFieldTask::TextFieldTask(TextField* text) :text(text) {
}
int TextFieldTask::processExecute() {
	if (OpenGL::ver==OpenGL::VER_CORE_100) return true; 
	TextFormat *format = text->_tf;
	Font *font = nullptr;
	FontFace *fface = nullptr;
	
	if (format->fn == NULL) {
		if (Font::buffer.empty()) return false;
		font = Font::buffer[0];
	}else{
		font = format->fn;
	}
	fface = font->getFontFace(format->size);
	if (fface==nullptr) {
		font->cached(format->size);
		return false;
	}else if (fface->tex->event!=Texture::LOADED) {
		return false;
	}else{
		short minX, maxX, minY, maxY;
		int bx, by, ch, id, lastID=0, lineHeight;
		struct vertexPoint {
			svec4 position;
			short attrRectID;
		} vec;
		vector<struct vertexPoint> vertexs;
		FontGlyph spaceGlyph = fface->arr.data[ FT_Get_Char_Index(font->face, ' ') ];
		
		lineHeight = (format->lineHeight == __SHRT_MAX__) ? format->size*1.5 : format->lineHeight;
		bx = text->paddingLeft;
		by = text->paddingTop+lineHeight;
		minX = maxX = bx;
		minY = maxY = by;
		for(int i=0; i<text->strUTF8.length(); i++) {
			if ( (unsigned char)(text->strUTF8[i])<0x20) {
				switch (text->strUTF8[i]) {
					case ASCII_SPEC::LF:
						bx = text->paddingLeft;
						by += lineHeight;
						break;
					case ASCII_SPEC::TAB:
						bx = ceil( ((float)bx)/(spaceGlyph.horiAdvance*format->tabSize))*spaceGlyph.horiAdvance*format->tabSize ;
						break;
				}
				lastID = 0;
			}else{
				if ( (unsigned char)(text->strUTF8[i])<0x80 ) {//utf8-1 0xxx-xxxx
					ch = text->strUTF8[i];
					id = FT_Get_Char_Index(font->face, text->strUTF8[i]);
				}else if ( (unsigned char)(text->strUTF8[i])<0xE0 ) {//utf8-2 110x-xxxx 10xx-xxxx
					ch = (text->strUTF8[i]&0x1F)<<6 | (text->strUTF8[i+1]&0x3F);
					id = FT_Get_Char_Index(font->face, ch);
					i++;
				}else if ( (unsigned char)(text->strUTF8[i])<0xF0 ) {//utf8-3 1110-xxxx 10xx-xxxx 10xx-xxxx
					ch = (text->strUTF8[i]&0x0F)<<12 | (text->strUTF8[i+1]&0x3F)<<6 | (text->strUTF8[i+2]&0x3F );
					id = FT_Get_Char_Index(font->face, ch);
					i+=2;
				}else if ( (unsigned char)(text->strUTF8[i])<0xF8 ) {//utf8-4 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
					ch = (text->strUTF8[i]&0x0F)<<18 | (text->strUTF8[i+1]&0x3F)<<12 | (text->strUTF8[i+1]&0x3F)<<6 | (text->strUTF8[i+2]&0x3F);
					id = FT_Get_Char_Index(font->face, ch);
					i+=3;
				}
				if (fface->arr.data[id].horiAdvance > 0) {
					lastID = ch;
					vec.position.x = bx+fface->arr.data[id].horiBearingX;
					vec.position.y = by-fface->arr.data[id].horiBearingY;
					vec.position.z = fface->arr.data[id].bmpWidth;
					vec.position.w = fface->arr.data[id].bmpHeight;
					vec.attrRectID = id;
					vertexs.push_back(vec);
					bx+=fface->arr.data[id].horiAdvance;
					if (vec.position.x < minX) minX = vec.position.x;
					if (vec.position.y < minY) minY = vec.position.y;
					if (vec.position.x+fface->arr.data[id].bmpWidth > maxX) maxX = vec.position.x+fface->arr.data[id].bmpWidth;
					if (vec.position.y+fface->arr.data[id].bmpHeight > maxY) maxY = vec.position.y+fface->arr.data[id].bmpHeight;
				}else{
					//int v1 = fface->arr.data[lastID].horiBearingX + fface->arr.data[lastID].width/2 - fface->arr.data[lastID].horiAdvance;
					//glRasterPos2s(bx+f->cache[0].arr[id].horiBearingX+v1, by - f->cache[0].arr[lastID].horiBearingY - f->cache[0].arr[id].height );
						//glRasterPos2s(bx+fface->arr->data[id].horiBearingX+v1, by-fface->arr->data[lastID].horiBearingY - fface->arr->data[id].height );
						//glDrawPixels(fface->arr->data[id].bmpWidth, fface->arr->data[id].bmpHeight, GL_ALPHA, GL_UNSIGNED_BYTE, fface->arr->data[id].bmp);
				}
			}
		}
		text->_symvolCount = vertexs.size();
		text->setRect(maxX-minX, maxY-minY);
		text->setAligment(text->position);
		if (text->tex==nullptr) {
			text->tex = Texture::getTexture(text->width, text->height, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE);
			if (text->tex==nullptr) return false;
		}else{
			glBindTexture(GL_TEXTURE_2D, text->tex->GLID);
			glTexImage2D( GL_TEXTURE_2D, 0, text->tex->internalType, text->width, text->height, 0, text->tex->format, text->tex->type, NULL );
		}
		
		
		
		GLuint buffer, bvao, bvbo;
		
		
		glGenFramebuffers(1, &buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, buffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, text->tex->GLID, 0);
		if (GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
			puts("FBO set up successfully. Yay!\n");
		}else{
			printf("FBO set up error %i.\n", glGetError());
			return false;
		}
		
		Matrix3D mtr = Matrix3D::ViewOrtho(minX, maxX, maxY, minY, -1, 1);
		OpenGL::pushViewport();
		OpenGL::pushViewMatrix();
		OpenGL::setViewport(0, 0, text->width, text->height);
		OpenGL::setViewMatrix(mtr);
		
		SET_SHADER(ShaderBitmapAtlas);
		glGenVertexArrays(1, &bvao);
		glBindVertexArray(bvao);
		
		glGenBuffers(1, &bvbo);
		glBindBuffer(GL_ARRAY_BUFFER, bvbo);
		glBufferData(GL_ARRAY_BUFFER, text->_symvolCount*10, vertexs.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(ShaderShRect::POSITION, 4, GL_SHORT, GL_FALSE, 10, NULL);
		glEnableVertexAttribArray(ShaderShRect::POSITION);
		
		glVertexAttribPointer(ShaderBitmapAtlas::prog->attrRectID, 1, GL_SHORT, GL_FALSE, 10, (void*)8 );
		glEnableVertexAttribArray(ShaderBitmapAtlas::prog->attrRectID);
		
		glBindTexture(GL_TEXTURE_2D, fface->tex->GLID);
		glBindTexture(GL_TEXTURE_1D, fface->tex->rectGLID);
		//glDrawArrays(GL_POINTS, 0, text->_symvolCount);
		//glUniform1i(ShaderBitmapAtlas::prog->rectID, -1);
		for (int i=0; i<text->_symvolCount; i++) {
			
			glUniform1i(ShaderBitmapAtlas::prog->rectID, vertexs[i].attrRectID);
			glDrawArrays(GL_POINTS, i, 1);
		}
		//printf("ShaderBitmapAtlas::prog->attrRectID %i %i %i %i\n", ShaderBitmapAtlas::prog->attrRectID, ShaderShRect::POSITION, ShaderBitmapAtlas::prog->attrRectID, glGetError());
		glDeleteVertexArrays(1, &bvao);
		glDeleteBuffers(1, &bvbo);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &buffer);
		OpenGL::popViewMatrix();
		OpenGL::popViewport();
	}
}
TextField::TextField(unsigned short w, unsigned short h) :ShapeRect(0), _tf(TextFormat::defaultFormat) {
	//this->bufferTexture = new Texture(w, h, GL_ALPHA, GL_UNSIGNED_BYTE);
	this->setRect(w, h);
	//this->style
	//this->style.borderRight = this->fn->cache[0].arr[ FT_Get_Char_Index(this->fn->face, ' ')].horiAdvance;
	//this->style.borderTop = this->style.borderBottom = this->style.lineHeight = this->fn->cache[0].size*1.5;
}
void TextField::trace() {
	printf("<TextField x='%i' y='%i' gx='%i' gy='%i' w='%i' h='%i' texId='%i'/>\n", local.x, local.y, global.x, global.y, width, height, (int)tex);
}
int TextField::renderGL100() {
	if (this->tex==nullptr) return false;
	if (this->_tf==nullptr) {
		glColor4ub(0xFF,0xFF,0xFF,0xFF);
	}else{
		glColor4ub(this->_tf->textDecorationColor.r,this->_tf->textDecorationColor.g,this->_tf->textDecorationColor.b,this->_tf->textDecorationColor.a);
	}
	svec2 global = this->global+this->offset;
	//glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, tex->GLID);
	
	
	glBegin( GL_QUADS );
		glTexCoord2f( 0.0, 0.0 );	glVertex2s(global.x,		global.y );
		glTexCoord2f( 0.0, 1.0 );	glVertex2s(global.x,		global.y+height );
		glTexCoord2f( 1.0, 1.0 );	glVertex2s(global.x+width,	global.y+height );
		glTexCoord2f( 1.0, 0.0 );	glVertex2s(global.x+width,	global.y );
	glEnd();
	//glDisable( GL_TEXTURE_2D );
	return true;
} 
int TextField::renderGL330() {
	if (this->tex==0) return false;
	SET_SHADER(ShaderTextFieldBuffer);
	glBindTexture(GL_TEXTURE_2D, this->tex->GLID);
	
	glBindVertexArray(this->vao);
	glDrawArrays(GL_POINTS, 0, 1);
	
	return true;
}
void TextField::operator=(const string& right) {
	this->strUTF8 = right;
	Windows::window->eachFrame.addTask(new TextFieldTask(this));
	printf("text11 %s\n", right.c_str());
}
void TextField::setFormat(TextFormat* tf) {
	this->_tf = tf;
}
void TextField::setAligment(POSITION align) {
	this->position = align;
	switch (this->position) {
		case POSITION::LEFT_TOP:
			this->setOffset(paddingLeft, paddingTop);
			break;
		case POSITION::LEFT_BOTTOM:
			this->setOffset(paddingLeft, -height-paddingBottom);
			break;
		case POSITION::LEFT_CENTER:
			this->setOffset(paddingLeft, -(height+paddingTop+paddingBottom)/2);
			break;
		case POSITION::RIGHT_TOP:
			this->setOffset(-width-paddingRight, paddingTop);
			break;
		case POSITION::RIGHT_BOTTOM:
			this->setOffset(-width-paddingRight, -height-paddingBottom);
			break;
		case POSITION::RIGHT_CENTER:
			this->setOffset(-width-paddingRight, -(height+paddingTop+paddingBottom)/2);
			break;
		case POSITION::TOP_CENTER:
			this->setOffset(-width/2, 0);
			break;
		case POSITION::BOTTOM_CENTER:
			this->setOffset(-width/2, -height);
			break;
		case POSITION::CENTER:
			this->setOffset(-width/2, -height/2);
			break;
	}
}


ShaderTextFieldBuffer* ShaderTextFieldBuffer::prog = nullptr;
ShaderTextFieldBuffer::ShaderTextFieldBuffer() :ShaderShRect(ShaderTextFieldBuffer::CRC32) {
	
}
void ShaderTextFieldBuffer::init() {
	this->ShaderShRect::init();
	this->texCoord = glGetAttribLocation(this->shaderProgram, "coordTex");
	this->textColor = glGetAttribLocation(this->shaderProgram, "textColor");
	this->textTexture = glGetUniformLocation(this->shaderProgram, "textTexture");
	glUniform1i(this->textTexture, 0);
}




/*int TextField::bufferGLComptAll() {
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
		if (format->fn == NULL) {
			if (Font::buffer.empty()) return false;
			font = Font::buffer[0];
		}else{
			font = format->fn;
		}
		lineHeight = (format->lineHeight == __SHRT_MAX__) ? format->size*1.5 : format->lineHeight;
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
		spaceGlyph.bmpWidth = font->face->glyph->bitmap.width;
		spaceGlyph.bmpHeight = font->face->glyph->bitmap.rows;
		spaceGlyph.bmp = malloc(spaceGlyph.bmpWidth*spaceGlyph.bmpHeight);
		memcpy(spaceGlyph.bmp, font->face->glyph->bitmap.buffer, spaceGlyph.bmpWidth*spaceGlyph.bmpHeight);
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
					//lastGlyph.bmpWidth	= font->face->glyph->bitmap.width;
					//lastGlyph.bmpHeight	= font->face->glyph->bitmap.rows;
					lastGlyph.bmp		= malloc(lastGlyph.bmpWidth*lastGlyph.bmpHeight);
					memcpy(lastGlyph.bmp, font->face->glyph->bitmap.buffer, lastGlyph.bmpWidth*lastGlyph.bmpHeight);
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
	if ( ch == 0x0306 ) {
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
							}else{

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glDisable( GL_BLEND );
	glDisable( GL_ALPHA_TEST );
	return true;
}*/