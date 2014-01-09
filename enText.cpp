#include <vector>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/wglext.h>
#include <GL/glext.h>

#include "grBaseTypes.h"
#include "grWindows.h"
#include "grShape.h"
using namespace std;
using namespace grEngine;

/*int ft_init () {
	FT_Library library;
	FT_Face face;
	FT_GlyphSlot gs;
	FT_BitmapGlyphRec bmp;
	int error;
	
	error = FT_Init_FreeType( &library );
	if (error) return error;
	printf("FT_Init %i\n", error);
	error = FT_New_Face( library, "C:/Windows/Fonts/arial.ttf", 0, &face );
	if (error) return error;
	
	printf("FT_Init %i\n", error);
	printf("FT_Face %i\n", face->num_glyphs);
	error = FT_Set_Char_Size(face, 0, 20*64, 72, 72 );
	error = FT_Set_Pixel_Sizes(face, 0, 20);
	//FT_Set_Transform(face, &matrix, &delta);
	error = FT_Load_Glyph( face, FT_Get_Char_Index(face, 'W'), FT_LOAD_DEFAULT );
	//error = FT_Get_Glyph( face->glyph, &glyph );
	error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_LIGHT   );
	gs = face->glyph;
	printf(" width %i ", gs->metrics.width>>6);
	printf(" height %i\n", gs->metrics.height>>6);
	//printf("FT_Init %i\n", error);
	//printf("FT_Init %i\n", error);
	printf("FT_Init %i\n", error);
	glRasterPos2s(100, 100);
	glPixelZoom(1,-1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(gs->bitmap.width, gs->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, gs->bitmap.buffer);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	//printf("GLERR %i\n");
	return error;
}*/
//FT_Err_Unknown_File_Format

template<class Type> grEngine::Array<Type>::Array(unsigned int size) {
	this->data = (Type*)malloc( size*sizeof(Type) );
	if (this->data==NULL) {
		this->size = 0;
	}else{
		this->size = size;
	}
}

grEngine::TextFormat::TextFormat() {
	this->fn = NULL;
	//unsigned short strWidth, strHeight;
	this->size = 50;
	this->direction = DIR::LTR;
	//letterSpacing;
	this->lineHeight = __SHRT_MAX__;
	this->tabSize = 4;
	this->whiteSpace = __SHRT_MAX__;
	this->textDecorationColor.color = 0x00FF00AA;
	
}

grEngine::Font::Font(string path) {
	this->path = path;
	this->error = FT_New_Face( root.font.library, path.c_str(), 0, &this->face );
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
	root.fonts.push_back(this);
}
bool grEngine::Font::cached(unsigned short size) {
	this->error = FT_Set_Pixel_Sizes(this->face, 0, size);
	if (this->error != FT_Err_Ok) return false;
	int glyphCount = this->face->num_glyphs;
	FontFace *fface = new FontFace();
	fface->size = size;
	fface->arr->size = glyphCount;
	fface->arr = new Array<FontGlyph>(glyphCount);
	fface->ramUsed = sizeof(Array<FontGlyph>)+sizeof(FontGlyph)*glyphCount;
	for(int i=0; i<this->face->num_glyphs; i++) {
		FT_Load_Glyph( this->face, i, FT_LOAD_DEFAULT );
		FT_Render_Glyph( this->face->glyph, FT_RENDER_MODE_LIGHT   );
		fface->arr->data[i].bmpWidth = this->face->glyph->bitmap.width;
		fface->arr->data[i].bmpHeight = this->face->glyph->bitmap.rows;
		fface->arr->data[i].bmp = malloc(fface->arr->data[i].bmpWidth*fface->arr->data[i].bmpHeight);
		memcpy(fface->arr->data[i].bmp, this->face->glyph->bitmap.buffer, fface->arr->data[i].bmpWidth*fface->arr->data[i].bmpHeight);
		fface->arr->data[i].width = this->face->glyph->metrics.width >> 6;
		fface->arr->data[i].height = this->face->glyph->metrics.height >> 6;
		fface->arr->data[i].horiAdvance = this->face->glyph->metrics.horiAdvance >> 6;
		fface->arr->data[i].horiBearingX = this->face->glyph->metrics.horiBearingX >> 6;
		fface->arr->data[i].horiBearingY = this->face->glyph->metrics.horiBearingY >> 6;
		fface->arr->data[i].vertAdvance = this->face->glyph->metrics.vertAdvance >> 6;
		fface->arr->data[i].vertBearingX = this->face->glyph->metrics.vertBearingX >> 6;
		fface->arr->data[i].vertBearingY = this->face->glyph->metrics.vertBearingY >> 6;
		fface->ramUsed+=fface->arr->data[i].bmpWidth*fface->arr->data[i].bmpHeight;
	} 
	this->cache.push_back(fface);
}
void Font::trace() {
	printf("<Fonts count='%i'>\n", root.fonts.size());
	for(int i=0; i<root.fonts.size(); i++) {
		printf("\t<Font path='%s' cached='%i' error='%i'>\n", root.fonts[i]->path.c_str(), root.fonts[i]->cache.size(), root.fonts[i]->error);
		for(int t=0; t<root.fonts[i]->cache.size(); t++) {
			printf("\t\t<FontFace size='%i' count='%i' ramUsed='%iKb'/>\n", root.fonts[i]->cache[t]->size, root.fonts[i]->cache[t]->arr->size, root.fonts[i]->cache[t]->ramUsed/1024);
		}
		printf("\t</Font>\n");
	}
	printf("</Fonts>\n");
}
int Font::init() {
	FT_Init_FreeType( &(root.font.library) );
}
FontFace* Font::getFontFace(unsigned short size) {
	for(int i=this->cache.size()-1; i>=0; i--) {
		if (this->cache[i]->size == size) return this->cache[i];
	}
	return NULL;
}

TextField::TextField(unsigned short w, unsigned short h) :Shape(0){
	this->bufferTexture = new Texture(w, h, GL_ALPHA, GL_UNSIGNED_BYTE);
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
	if (mode && !this->bufferActivate ) {
		root.window->FBOBuffer.push_back(this);
		
		this->bufferActivate = true;
		this->bufferInit = false;
	}else if ( !mode && this->bufferActivate ) {
		this->bufferActivate = false;
		this->bufferTexture->close();
	}
	return true;
}
void TextField::trace() {
	//printf("<Bitmap x='%i' y='%i' gx='%i' gy='%i' w='%i' h='%i' texId='%i'/>\n", this->x, this->y, this->globalx, this->globaly, this->width, this->height, this->tex);
}
int TextField::bufferGLComptAll() {
	if (this->bufferTexture->GLID == 0) return false; 
		glBindFramebuffer(GL_FRAMEBUFFER, this->bufferFrame);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, this->bufferTexture->GLID, 0);
		glViewport(0, 0, this->width, this->height);
		root.window->ogl->setViewportMatrix(0, this->height, this->width, 0);
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
		if (this->tf == NULL) {format=root.font.globalTextFormat;}else{format=this->tf;}
		if (format->fn == NULL) {
			if (root.fonts.empty()) return false;
			font = root.fonts[0];
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
		if ( FT_Set_Char_Size(font->face, 0, format->size*64, root.window->dpi, root.window->dpi ) != FT_Err_Ok) return false;
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
					lastGlyph.bmpWidth	= font->face->glyph->bitmap.width;
					lastGlyph.bmpHeight	= font->face->glyph->bitmap.rows;
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
					glDrawPixels(lastGlyph.bmpWidth, lastGlyph.bmpHeight, GL_ALPHA, GL_UNSIGNED_BYTE, lastGlyph.bmp);
					glBegin(GL_POINTS);
					glVertex2s(bx, by);
					glEnd();
					bx+=lastGlyph.horiAdvance;
				}else{
					int v1 = lastGlyph.horiBearingX + lastGlyph.width/2 - lastGlyph.horiAdvance;
					//glRasterPos2s(bx+f->cache[0].arr[id].horiBearingX+v1, by - f->cache[0].arr[lastID].horiBearingY - f->cache[0].arr[id].height );
					glRasterPos2s(bx+(font->face->glyph->metrics.horiBearingX>>6)+v1, by-(font->face->glyph->metrics.horiBearingY>>6)-(font->face->glyph->metrics.height>>6) );
					glDrawPixels(font->face->glyph->bitmap.width, font->face->glyph->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, font->face->glyph->bitmap.buffer);
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
					glDrawPixels(fface->arr->data[id].bmpWidth, fface->arr->data[id].bmpHeight, GL_ALPHA, GL_UNSIGNED_BYTE, fface->arr->data[id].bmp);
					glBegin(GL_POINTS);
					glVertex2s(bx, by);
					glEnd();
					bx+=fface->arr->data[id].horiAdvance;
				}else{
					int v1 = fface->arr->data[lastID].horiBearingX + fface->arr->data[lastID].width/2 - fface->arr->data[lastID].horiAdvance;
					//glRasterPos2s(bx+f->cache[0].arr[id].horiBearingX+v1, by - f->cache[0].arr[lastID].horiBearingY - f->cache[0].arr[id].height );
					glRasterPos2s(bx+fface->arr->data[id].horiBearingX+v1, by-fface->arr->data[lastID].horiBearingY - fface->arr->data[id].height );
					glDrawPixels(fface->arr->data[id].bmpWidth, fface->arr->data[id].bmpHeight, GL_ALPHA, GL_UNSIGNED_BYTE, fface->arr->data[id].bmp);
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
int TextField::bufferGL300() {
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
	glBegin( GL_QUADS );// <editor-fold defaultstate="collapsed" desc="GL_QUADS">
				glTexCoord2d( 0.0, 0.0 );	glVertex2i(this->globalx, this->globaly );
				glTexCoord2d( 0.0, 1.0 );	glVertex2i(this->globalx, this->globaly+tex->height );
				glTexCoord2d( 1.0, 1.0 );	glVertex2i(this->globalx+tex->width, this->globaly+tex->height );
				glTexCoord2d( 1.0, 0.0 );	glVertex2i(this->globalx+tex->width, this->globaly );
			glEnd();// </editor-fold>
	glDisable( GL_TEXTURE_2D );
	return true;
} 
int TextField::renderGL400() {
	return false;
}
int TextField::renderGL300() {
	return false;
}
int TextField::renderGL210() {
	return false;
}