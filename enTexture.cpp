#include <vector>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/wglext.h>
#include <GL/glext.h>
#include <png.h>
#include <jpeglib.h>
#include <jerror.h>
#include <setjmp.h>
#include <zlib.h>
#include "grBaseTypes.h"

using namespace std;
using namespace grEngine;

Texture::Texture(string path) {
	FileLoad *fl = new FileLoad(path);
	fl->addEventHandler(EventFileLoad::FILE_SUCCESS, Texture::loaded, this);
	
	this->format = 0;
	this->type = 0;
	this->width = this->height = 0;
	this->coordX = this->coordY = 1.0;
	this->status = STATUS::TO_LOAD;
	this->img = (FileLoad*)fl;
	this->GLID = 0;
	root.window->textureUpdateBuffer.push_back(this);
}
Texture::Texture(Image *img) {
	switch (img->type) {
		case Image::RGBA_32:
			this->format = GL_RGBA;
			this->type = GL_UNSIGNED_BYTE;
			break;
		case Image::BGRA_32:
			this->format = GL_BGRA;
			this->type = GL_UNSIGNED_BYTE;
			break;
		case Image::RGB_24:
			this->format = GL_RGB;
			this->type = GL_UNSIGNED_BYTE;
			break;
		case Image::BGR_24:
			this->format = GL_BGR;
			this->type = GL_UNSIGNED_BYTE;
			break;
		case Image::MONO_8:
			this->format = GL_ALPHA;
			this->type = GL_UNSIGNED_BYTE;
			break;
	}
	this->width = img->width;
	this->height = img->height;
	this->coordX = this->coordY = 1.0;
	this->status = STATUS::ERR_NO;
	this->img = img->img;
	this->GLID = 0;
	root.window->textureBuffer.push_back(this);
}
Texture::Texture(unsigned short w, unsigned short h, GLuint format, GLuint type) {
	this->coordX = this->coordY = 1.0;
	this->img = NULL;
	this->status = STATUS::ERR_NO;
	this->type = type;
	this->format = format;
	this->width = w;
	this->height = h;
	
		/*glBindTexture( GL_TEXTURE_2D, this->GLID );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		switch ( this->texType ) {
			case TYPE::RGBA_32:
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->img );
				break;
			case TYPE::BGRA_32:
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, this->img );
				break;
			case TYPE::RGB_24:
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->img );
				break;
			case TYPE::BGR_24:
				glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_BGR, GL_UNSIGNED_BYTE, this->img );
				break;
			case TYPE::MONO_8:
				glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, this->width, this->height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, this->img );
				break;
		}
		this->loc = LOC::TEXTURE;*/
	root.window->textureBuffer.push_back(this);
}
Texture::~Texture() {
	glDeleteTextures(1, &this->GLID);
}
void Texture::close() {
	free(this->img);
	this->status = STATUS::TO_DELETE;
}
void Texture::trace() {
	printf("\t<Texture w='%i' h='%i' cx='%f' cy='%f' status='%i' type='%i' format='%i' texId='%i' GLID='%i'/>\n", 
		this->width, this->height, this->coordX, this->coordY, this->status, this->type, this->format, this, this->GLID);
}

void Texture::loaded(const EventFileLoad* e) {
	Texture *tex = (Texture*)(e->obj);
	Image *img = new Image((char*)(e->file->data), e->file->size);
	switch (img->type) {
		case Image::RGBA_32:
			tex->format = GL_RGBA;
			tex->type = GL_UNSIGNED_BYTE;
			break;
		case Image::BGRA_32:
			tex->format = GL_BGRA;
			tex->type = GL_UNSIGNED_BYTE;
			break;
		case Image::RGB_24:
			tex->format = GL_RGB;
			tex->type = GL_UNSIGNED_BYTE;
			break;
		case Image::BGR_24:
			tex->format = GL_BGR;
			tex->type = GL_UNSIGNED_BYTE;
			break;
		case Image::MONO_8:
			tex->format = GL_ALPHA;
			tex->type = GL_UNSIGNED_BYTE;
			break;
	}
	tex->width = img->width;
	tex->height = img->height;
	tex->coordX = tex->coordY = 1.0;
	tex->status = STATUS::ERR_NO;
	tex->img = img->img;
	tex->GLID = 0;
	tex->trace();
}
typedef struct{
	char *data;
	unsigned int size, pt;
} DATA;
typedef struct {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
} my_error_mgr;
void PNGReadData(png_structp png_ptr, png_bytep data, png_size_t length) {
	//printf("PNGReadData %i\n", ((DATA*)png_get_io_ptr(png_ptr))->size );
	DATA *d = ((DATA*)png_get_io_ptr(png_ptr));
	if (d->pt+length > d->size) return;
	memcpy(data, d->data+d->pt, length);
	d->pt += length;
}
Image::Image(char* data, unsigned int size) {
	size_t rowSize;
	if ( ((uint16_t*)data)[0] == 0x4D42L ) {// <editor-fold defaultstate="collapsed" desc="BMP">
		bmpHead *bmph = ((bmpHead*)data);
		unsigned short align;
		if (bmph->null1!=0 || bmph->null2!=0 || bmph->sizeHead!=40 || bmph->layers!=1) {
			printf("<ERROR text='FILE_BMP_INVALID'/>\n");
			return;
		}
		this->width = bmph->width;
		this->height = bmph->height; //(w*3+3)&~3 -w;
		if (bmph->bpp == 24) {
			this->type = TYPE::BGR_24;
			align = ((this->width*3+3)&~3) -this->width*3;
			this->img = malloc( (this->width*3 + align)*this->height);
			rowSize = ((this->width*3+3)&~3);
			for(int y=0; y<this->height; y++) {
				memcpy( (void*)((unsigned long int)(this->img)+y*rowSize), data+bmph->offsetPixels+(this->height-y-1)*rowSize, rowSize);
			}
		}else if (bmph->bpp == 32) {
			this->type = TYPE::RGBA_32;		
			this->img = malloc(this->width*this->height*4);
			rowSize = this->width*4;
			for(int y=0; y<this->height; y++) {
				memcpy( (void*)((unsigned long int)(this->img)+y*rowSize), data+bmph->offsetPixels+(this->height-y-1)*rowSize, rowSize);
			}
		}
	//</editor-fold>
	}else if ( ((uint64_t *)data)[0] == 0x0A1A0A0D474E5089L) {// <editor-fold defaultstate="collapsed" desc="PNG">
		printf("PNG\n");
		printf("Compiled with libpng %s; using libpng %s.\n", PNG_LIBPNG_VER_STRING, png_get_header_ver(NULL));
		printf("Compiled with zlib %s; using zlib %s.\n", ZLIB_VERSION, zlibVersion());
		png_structp png_ptr;
		png_infop info_ptr;
		png_uint_32 retval;
		int bitDepth = 0, colorType = -1;
		DATA d1;
		d1.data = data;
		d1.size = size;
		d1.pt = 0;
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		if (!png_ptr) return;
		info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr) {
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			return;
		}
		png_set_read_fn(png_ptr, &d1, PNGReadData);
		png_set_sig_bytes(png_ptr, 0);
		if (setjmp(png_jmpbuf(png_ptr))) {
			printf("setjmp pt=%i w=%i h=%i\n", d1.pt, this->width, this->height);
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			return;
		}
		png_set_keep_unknown_chunks(png_ptr, 1, NULL, 0);
		png_read_info(png_ptr, info_ptr);
		retval = png_get_IHDR(png_ptr, info_ptr, ((png_uint_32*)&this->width), ((png_uint_32*)&this->height), &bitDepth, &colorType, NULL, NULL, NULL);
		if (!retval) {
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			return;
		}
		switch (colorType) {
			case PNG_COLOR_TYPE_GRAY:
				switch (bitDepth) {
					case 1:
						this->type = TYPE::MONO_1;
						rowSize = ((this->width+3)&~3);
						break;
					case 2:
						this->type = TYPE::MONO_2;
						rowSize = ((this->width+3)&~3);
						break;
					case 4:
						this->type = TYPE::MONO_4;
						rowSize = ((this->width+3)&~3);
						break;
					case 8:
						this->type = TYPE::MONO_8;
						rowSize = ((this->width+3)&~3);
						break;
					case 16:
						this->type = TYPE::MONO_16;
						rowSize = ((this->width*2+3)&~3);
						break;
				} break;
			case PNG_COLOR_TYPE_GRAY_ALPHA:
				switch (bitDepth) {
					case 8:
						this->type = TYPE::MONO_ALPHA_8;
						rowSize = ((this->width+3)&~3);
						break;
					case 16:
						this->type = TYPE::MONO_ALPHA_16;
						rowSize = ((this->width*2+3)&~3);
						break;
				} break;
			case PNG_COLOR_TYPE_PALETTE:
				switch (bitDepth) {
					case 1:
						this->type = TYPE::PALETTE_1;
						break;
					case 2:
						this->type = TYPE::PALETTE_2;
						break;
					case 4:
						this->type = TYPE::PALETTE_4;
						break;
					case 8:
						this->type = TYPE::PALETTE_8;
						break;
				};
				rowSize = ((this->width+3)&~3);
				break;
			case PNG_COLOR_TYPE_RGB:
				switch (bitDepth) {
					case 8:
						this->type = TYPE::RGB_24;
						rowSize = ((this->width*3+3)&~3);
						break;
					case 16:
						this->type = TYPE::RGB_48;
						rowSize = ((this->width*6+3)&~3);
						break;
				} break;
			case PNG_COLOR_TYPE_RGB_ALPHA:
				switch (bitDepth) {
					case 8:
						this->type = TYPE::RGBA_32;
						rowSize = this->width*4;
						break;
					case 16:
						this->type = TYPE::RGBA_64;
						rowSize = this->width*8;
						break;
				} break;
		}
		this->img = malloc(rowSize*this->height);
		const png_uint_32 bytesPerRow = png_get_rowbytes(png_ptr, info_ptr);
		byte* rowData = (byte*)malloc(bytesPerRow);
		
		for(int y=0; y<this->height; y++) {
			png_read_row(png_ptr, (png_bytep)rowData, NULL);
			memcpy( (void*)((unsigned long int)(this->img)+y*rowSize), rowData, bytesPerRow);
		}
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		printf("Image size=%iKB w=%i h=%i depth=%i color=%i rowSize=%i\n", d1.size/1024, this->width, this->height, bitDepth, colorType, bytesPerRow);// </editor-fold>
	}else if ( ((uint16_t*)data)[0] == 0xD8FFL) {// <editor-fold defaultstate="collapsed" desc="JPEG">
		printf("JPEG\n");
		struct jpeg_decompress_struct cinfo;
		my_error_mgr jerr;
		FILE * infile;
		JSAMPARRAY buffer;
		int row_stride;
		//src = reinterpret_cast<JpegStream*> (cinfo->src);
		cinfo.err = jpeg_std_error(&jerr.pub);
		//jerr.pub.error_exit = my_error_exit;
		if (setjmp(jerr.setjmp_buffer)) {
			printf("ERROR JPEG\n");
			jpeg_destroy_decompress(&cinfo);
			fclose(infile);
			return;
		}
		jpeg_create_decompress(&cinfo);
		jpeg_mem_src(&cinfo, (unsigned char*)data, size);
		jpeg_read_header(&cinfo, TRUE);
		this->width = cinfo.image_width;
		this->height = cinfo.image_height;
		switch (cinfo.num_components) {
			case 1:
				this->type = TYPE::MONO_8;
				rowSize = ((this->width+3)&~3);
				break;
			case 3:
				this->type = TYPE::RGB_24;
				rowSize = ((this->width*3+3)&~3);
				break;
			case 4:
				this->type = TYPE::RGBA_32;
				rowSize = this->width*4;
				break;
		};
		this->img = malloc(rowSize*this->height);
		
		jpeg_start_decompress(&cinfo);
		unsigned char *buf = (unsigned char*)malloc(this->width*cinfo.num_components);
		for(int y=0; cinfo.output_scanline<cinfo.output_height; y++) {
			jpeg_read_scanlines(&cinfo, &buf, 1);
			memcpy( (void*)((unsigned long int)(this->img)+y*rowSize), buf, this->width*cinfo.num_components);
		}
		jpeg_finish_decompress(&cinfo);
		
		printf("JPEG w='%i' h='%i' %i\n", cinfo.image_height, cinfo.image_width, cinfo.output_height);// </editor-fold>
	}
}
Image::Image(string path) {
	
}
