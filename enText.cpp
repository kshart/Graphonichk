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
	this->error = FT_Set_Char_Size(face, 0, 50*64, root.window->dpi, root.window->dpi );
	if (this->error != FT_Err_Ok) return;
	//error = FT_Set_Pixel_Sizes(face, 0, 20);
	
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
	this->cache.push_back(fgs[0]);
	root.fonts.push_back(this);
}
void grEngine::Font::cached(unsigned int id) {
}

void Font::trace() {
	printf("<Fonts count='%i'>\n", root.fonts.size());
	for(int i=0; i<root.fonts.size(); i++) {
		printf("\t<Font path='%s' cached='%i' error='%i'>\n", root.fonts[i]->path.c_str(), root.fonts[i]->cache.size(), root.fonts[i]->error);
		for(int t=0; t<root.fonts[i]->cache.size(); t++) {
			printf("\t\t<FontFace size='%i' count='%i' ramUsed='%iKb'/>\n", root.fonts[i]->cache[t].size, root.fonts[i]->cache[t].count, root.fonts[i]->cache[t].ramUsed/1024);
		}
		printf("\t</Font>\n");
	}
	printf("</Fonts>\n");
}
int Font::init() {
	FT_Init_FreeType( &(root.font.library) );
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
	//if (Windows::renderComplete) {
	
	//5, 10, 15, 20, 25, 30
	//8, 15, 23, 31, 38, 46
	if (this->bufferTexture->GLID == 0) return false; 
	printf("TextField::bufferGLComptAll\n");
		glBindFramebuffer(GL_FRAMEBUFFER, this->bufferFrame);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, this->bufferTexture->GLID, 0);
		printf("|| %i %i %i %i ||\n", this->bufferTexture->width, this->bufferTexture->height, this->width, this->height );
		root.window->ogl->resizeWindow(this->width, this->height);
		glClearColor(0,0,0,0);
		glClear( GL_COLOR_BUFFER_BIT );
		
		glEnable( GL_BLEND );
		glEnable( GL_ALPHA_TEST );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		TextFormat *t;
		Font *font;
		if (this->tf == NULL) {t=root.font.globalTextFormat;}else{t=this->tf;}
		if (t->fn == NULL) {
			if (root.fonts.empty()) return false;
			font=root.fonts[0];
		}else{
			font=t->fn;
		}
		
		int lineHeight, lastID=0, id, bx, by, ch;
		if (t->lineHeight == __SHRT_MAX__) { lineHeight = t->size*1.5; }else{ lineHeight = t->lineHeight; }
		bx = this->paddingLeft;
		by = this->paddingTop+lineHeight;
		FontGlyph spaceGlyph = font->cache[0].arr[ FT_Get_Char_Index(font->face, ' ') ];
		
		this->bufferTexture->trace();
		printf("TextField str=%s\n", this->strUTF8.c_str());
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				for(int i=0; i<this->strUTF8.length(); i++) {
					if ( (unsigned char)(this->strUTF8[i])<0x20) {
						switch (this->strUTF8[i]) {
							case ASCII_SPEC::LF:
								bx = this->paddingLeft;
								by += lineHeight;
								break;
							case ASCII_SPEC::TAB:
								bx += spaceGlyph.horiAdvance*t->tabSize;
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
						if (font->cache[0].arr[id].horiAdvance > 0) {
							lastID = ch;
							glRasterPos2s(bx+font->cache[0].arr[id].horiBearingX, by-font->cache[0].arr[id].horiBearingY);
							glDrawPixels(font->cache[0].arr[id].bmpWidth, font->cache[0].arr[id].bmpHeight, GL_ALPHA, GL_UNSIGNED_BYTE, font->cache[0].arr[id].bmp);
							glBegin(GL_POINTS);
							glVertex2s(bx, by);
							glEnd();
							bx+=font->cache[0].arr[id].horiAdvance;
						}else{
							int v1 = font->cache[0].arr[lastID].horiBearingX + font->cache[0].arr[lastID].width/2 - font->cache[0].arr[lastID].horiAdvance;
							//glRasterPos2s(bx+f->cache[0].arr[id].horiBearingX+v1, by - f->cache[0].arr[lastID].horiBearingY - f->cache[0].arr[id].height );
							glRasterPos2s(bx+font->cache[0].arr[id].horiBearingX+v1, by-font->cache[0].arr[lastID].horiBearingY - font->cache[0].arr[id].height );
							glDrawPixels(font->cache[0].arr[id].bmpWidth, font->cache[0].arr[id].bmpHeight, GL_ALPHA, GL_UNSIGNED_BYTE, font->cache[0].arr[id].bmp);
						}
					}
				}
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
		printf("\n");
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		
		glDisable( GL_BLEND );
		glDisable( GL_ALPHA_TEST );
		return true;
	//}
	//Windows::FBOBuffer.push_back(this);
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