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
/*
grEngine::Texture::Texture() {
	printf("\tTexture gen null tex %i\n");
	this->coordX = this->coordY = 1.0;
	this->width = this->height = 0;
	this->img = NULL;
	this->GLID = 0;
	this->loc = LOC::UNAVAILABLE;
	this->texType = TYPE::NC;
	this->err = ERR::NO;
	root.window->textureBuffer.push_back(this);
}*/
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
	if ( ((uint16_t*)data)[0] == 0x4D42L ) {// <editor-fold defaultstate="collapsed" desc="GR_IMG_I_BMP">
		bmpHead *bmph = ((bmpHead*)data);
		unsigned short align;
		printf("<Image sgn='%i', size='%i', offset='%i', sizeHead='%i', width='%i', height='%i', layers='%i'/>\n",
				bmph->signature, bmph->sizeFile, bmph->offsetPixels, bmph->sizeHead, bmph->width, bmph->height, bmph->layers);
		
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
				memcpy(this->img+y*rowSize, data+bmph->offsetPixels+(this->height-y-1)*rowSize, rowSize);
			}
		}else if (bmph->bpp == 32) {
			this->type = TYPE::RGBA_32;		
			this->img = malloc(this->width*this->height*4);
			rowSize = this->width*4;
			for(int y=0; y<this->height; y++) {
				memcpy(this->img+y*rowSize, data+bmph->offsetPixels+(this->height-y-1)*rowSize, rowSize);
			}
		}
	//</editor-fold>
	}else if ( ((uint64_t *)data)[0] == 0x0A1A0A0D474E5089L) {// <editor-fold defaultstate="collapsed" desc="GR_IMG_I_BMP">
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
			memcpy(this->img+y*rowSize, rowData, bytesPerRow);
		}
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		printf("Image size=%iKB w=%i h=%i depth=%i color=%i rowSize=%i\n", d1.size/1024, this->width, this->height, bitDepth, colorType, bytesPerRow);// </editor-fold>
	}else if ( ((uint16_t*)data)[0] == 0xD8FFL) {
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
			memcpy(this->img+y*rowSize, buf, this->width*cinfo.num_components);
		}
		jpeg_finish_decompress(&cinfo);
		
		printf("JPEG w='%i' h='%i' %i\n", cinfo.image_height, cinfo.image_width, cinfo.output_height);
	}
}
Image::Image(string path) {
	
}
/*int Texture::load(string path, Texture::LOC loc) {
	string ext = get_ext(path);;
	int fileext;
	bool loaded;
	printf("Texture::load start\n");
	if (ext==GR_IMG_BMP) {
		fileext = GR_IMG_I_BMP;
	}else if (ext==GR_IMG_TGA) {
		fileext = GR_IMG_I_TGA;
	}else{
		printf("\tERR:EXT_NS\n");
		return false;
	}
	FILE *file = fopen(path.c_str(), "rb");
	if (file == NULL) {
		printf("\tERR:FILE_NF\n");
		return false;
	}
	//loc
	loaded = this->loadFD(file, loc, fileext, 0);
	fclose(file);
	printf("Texture::load end\n");
	return loaded;
}*//*
int Texture::loadFD(FILE *file, LOC loc, char type, long offset) {
	unsigned short align;
	tgaHead th;
	void *buff = NULL;
	fseek(file, offset, SEEK_SET);
	switch ( type ) {
		case GR_IMG_I_BMP:// <editor-fold defaultstate="collapsed" desc="GR_IMG_I_BMP">
			bmpHead bmph;
			fread(&bmph, sizeof(bmpHead), 1, file);
			printf("\tFile signature=%i size=%i, offset=%i, sizeHead=%i, width=%i, height=%i, layers=%i\n",
					bmph.signature, bmph.sizeFile, bmph.offsetPixels, bmph.sizeHead, bmph.width, bmph.height, bmph.layers);
			if (bmph.null1!=0 || bmph.null2!=0 || bmph.sizeHead!=40 || bmph.layers!=1) {
				printf("\tERROR_FILE_BMP_INVALID\n");
				return false;
			}
			this->loc = loc;
			this->width = bmph.width;
			this->height = bmph.height; //(w*3+3)&~3 -w;
			fseek(file, offset+bmph.offsetPixels, SEEK_SET);
			if (bmph.bpp == 24) {// <editor-fold defaultstate="collapsed" desc="GR_RGB_24">
				this->texType = TYPE::BGR_24;
				align = ((bmph.width*3+3)&~3) -bmph.width*3;
				printf("\talign = %i\n", align);
				this->img = malloc( (this->width*3 + align)*this->height);
				fread(this->img, this->width*3+align, this->height, file);//</editor-fold>
			}else if (bmph.bpp == 32) {// <editor-fold defaultstate="collapsed" desc="GR_RGBA_32">
				this->texType = TYPE::RGBA_32;		
				this->img = malloc(this->width*this->height*4);
				fread(this->img, this->width*this->height*4, 1, file);
			}// </editor-fold>
			break;//</editor-fold>
		case GR_IMG_I_TGA:// <editor-fold defaultstate="collapsed" desc="GR_IMG_TGA">
			fread(&th, sizeof(tgaHead), 1, file);
			printf("\tTGAFile width=%i, height=%i, IDLength=%i, colorMapType=%i, imgType=%i\n", th.width, th.height, th.IDLength, th.colorMapType, th.imgType);
			this->width = th.width;
			this->height = th.height;
			switch(th.imgType) {
				case TGA_TYPE_PALETTE://<editor-fold defaultstate="collapsed" desc="TGA_TYPE_PALETTE">
					return false;//</editor-fold>
				case TGA_TYPE_TC://<editor-fold defaultstate="collapsed" desc="TGA_TYPE_TC">
					if (th.colorDepth==TGA_DEP_32) {
						printf("\t32bits\n");
						buff = malloc(th.width*th.height*sizeof(argb));
						this->img = malloc(this->width*this->height*sizeof(argb));
						this->texType = TYPE::BGRA_32;
						fseek(file, offset+sizeof(tgaHead)+th.IDLength+th.colorMapType, SEEK_SET);
						fread(buff, th.width*th.height*sizeof(argb), 1, file);
						if (th.imgDescriptor&TGA_TRANSFER_X && th.imgDescriptor&TGA_TRANSFER_Y) {//<editor-fold defaultstate="collapsed" desc="TGA_TRANSFER">
							printf("\txy\n");
							for (int y=0; y<th.height; y++) {
								for (int x=th.width-1; x>=0; x--) {
									((argb*)(this->img))[x+this->width*y] = ((argb*)buff)[th.height-x-1+th.width*y];
								}
							}
						}else if (th.imgDescriptor&TGA_TRANSFER_X) {
							printf("\tx\n");
							for (int y=0; y<th.height; y++) {
								for (int x=0; x<th.width; x++) {
									((argb*)(this->img))[x+this->width*y] = ((argb*)buff)[th.height-x-1+th.width*(th.height-y-1)];
								}
							}
						}else if (th.imgDescriptor&TGA_TRANSFER_Y) {
							printf("\ty\n");
							for (int y=0; y<th.height; y++) {
								for (int x=0; x<th.width; x++) {
									((argb*)(this->img))[x+this->width*y] = ((argb*)buff)[x+th.width*y];
								}
							}
						}else{
							printf("\tnone\n");
							for (int y=0; y<th.height; y++) {
								for (int x=0; x<th.width; x++) {
									((argb*)(this->img))[x+this->width*y] = ((argb*)buff)[x+th.width*(th.height-y-1)];
								}
							}
						}
						free(buff);//</editor-fold>
					}else if (th.colorDepth==TGA_DEP_24) {
						printf("\t24bits\n");
						align = (bmph.width*3+3)&~3 -bmph.width*3;
						buff = malloc( th.width*sizeof(rgb)*th.height );
						this->img = malloc( (th.width*sizeof(rgb)+align)*th.height );
						this->texType = TYPE::BGR_24;
						fseek(file, offset+sizeof(tgaHead)+th.IDLength+th.colorMapType, SEEK_SET);
						fread(buff, th.width*th.height*sizeof(rgb), 1, file);
						if (th.imgDescriptor&TGA_TRANSFER_X && th.imgDescriptor&TGA_TRANSFER_Y) {//<editor-fold defaultstate="collapsed" desc="TGA_TRANSFER">
							printf("\txy\n");
							for (int y=0; y<th.height; y++) {
								for (int x=0; x<th.width; x++) {
									((rgb*)(this->img))[x+this->width*y] = ((rgb*)buff)[th.height-x-1+th.width*y];
								}
							}
						}else if (th.imgDescriptor&TGA_TRANSFER_X) {
							printf("\tx\n");
							for (int y=0; y<th.height; y++) {
								for (int x=0; x<th.width; x++) {
									((rgb*)(this->img))[x+this->width*y] = ((rgb*)buff)[th.height-x-1+th.width*(th.height-y-1)];
								}
							}
						}else if (th.imgDescriptor&TGA_TRANSFER_Y) {
							printf("\ty\n");
							for (int y=0; y<th.height; y++) {
								for (int x=0; x<th.width; x++) {
									((rgb*)(this->img))[x+this->width*y] = ((rgb*)buff)[x+th.width*y];
								}
							}
						}else{
							printf("\tnone\n");
							for (int y=0; y<th.height; y++) {
								for (int x=0; x<th.width; x++) {
									((rgb*)(this->img))[x+this->width*y] = ((rgb*)buff)[x+th.width*(th.height-y-1)];
								}
							}
						}
						free(buff);//</editor-fold>
					}
					break;//</editor-fold>
				case TGA_TYPE_MONOCHROME://<editor-fold defaultstate="collapsed" desc="TGA_TYPE_MONOCHROME">
					//this->img = malloc(th.width*th.height);
					return false;
					break;//</editor-fold>
			};
			free(buff);
			break;// </editor-fold>
		default :
			printf("\tERROR_FILE_TYPE_NOT_SUPPORT\n");
			return false;
	}
	if (loc == LOC::BMP) {
		this->loc = LOC::BMP;
		return true;
	}
	// <editor-fold defaultstate="expanding" desc="LOC::TEXTURE">
	glGenTextures( 1, &this->GLID );
	if (this->GLID==0) {
		printf("\tERRORS_GL %i\n", glGetError());
		printf("\tTexture NOGEN %i\n");
	}else{
		glBindTexture( GL_TEXTURE_2D, this->GLID );
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
				glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, this->width, this->height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, this->img );
				break;
		}
		this->loc = LOC::TEXTURE;
	}
	//</editor-fold>
	return true;
}
int Texture::save(string path) {
	FILE *file = fopen(path.c_str(), "wb");
	std::string ext = get_ext(path);
	int tt;
	if (ext==GR_IMG_BMP) {
		tt = GR_IMG_I_BMP;
	}else if (ext==GR_IMG_TGA) {
		tt = GR_IMG_I_TGA;
	}else{
		return false;
	}
	this->saveFD(file, tt, 0);
	fclose(file);
	return true;
}
int Texture::saveFD(FILE *file, char type, long offset) {
	int null1 = 0;
	fseek(file, offset, SEEK_SET);
	switch (type) {
		case GR_IMG_I_BMP:// <editor-fold defaultstate="collapsed" desc="GR_IMG_I_BMP">
			bmpHead bh;
			bh.signature = 0x4D42;
			bh.null1 = 0;
			bh.null2 = 0;
			bh.offsetPixels = 54;
			bh.sizeHead = 40;
			bh.width = this->width;
			bh.height = this->height;
			bh.layers = 1;
			bh.compression = 0;
			bh.compressionSize = 0;
			bh.ppmWidth = 72;
			bh.ppmHeight = 72;
			bh.colorsUse = 0;
			bh.colorsSpecial = 0;
			printf("imgType %i\n", this->texType);
			switch (this->texType) {
				case TYPE::RGBA_32:
					bh.bpp = 32;
					bh.sizeFile = 54+((bh.width*4+3)&~3)*bh.height;
					fwrite(&bh, sizeof(bmpHead), 1, file);
					for (int y=0; y<bh.height; y++) {
						for (int x=0; x<bh.width; x++) {
							fwrite(&((argb*)(this->img))[x+this->width*(this->height-y-1)].a, 1, 1, file);
							fwrite(&((argb*)(this->img))[x+this->width*(this->height-y-1)].b, 1, 1, file);
							fwrite(&((argb*)(this->img))[x+this->width*(this->height-y-1)].g, 1, 1, file);
							fwrite(&((argb*)(this->img))[x+this->width*(this->height-y-1)].r, 1, 1, file);
						}
						fwrite(&null1, ((bh.width*4+3)&~3)-bh.width*4, 1, file);
					}
					return bh.sizeFile;
				case TYPE::BGRA_32:
					bh.bpp = 32;
					bh.sizeFile = 54+((bh.width*4+3)&~3)*bh.height;
					fwrite(&bh, sizeof(bmpHead), 1, file);
					for (int y=bh.height-1; y>=0; y--) {
						for (int x=0; x<bh.width; x++) {
							fwrite(&((argb*)(this->img))[x+this->width*y], 4, 1, file);
						}
						fwrite(&null1, ((bh.width*4+3)&~3)-bh.width*4, 1, file);
					}
					return bh.sizeFile;
				case TYPE::RGB_24:
					bh.bpp = 24;
					bh.sizeFile = 54+((bh.width*3+3)&~3)*bh.height;
					fwrite(&bh, sizeof(bmpHead), 1, file);
					for (int y=0; y<bh.height; y++) {
						for (int x=0; x<bh.width; x++) {
							fwrite(&((rgb*)(this->img))[x+this->width*(this->height-y-1)].b, 1, 1, file);
							fwrite(&((rgb*)(this->img))[x+this->width*(this->height-y-1)].g, 1, 1, file);
							fwrite(&((rgb*)(this->img))[x+this->width*(this->height-y-1)].r, 1, 1, file);
						}
						fwrite(&null1, ((bh.width*3+3)&~3)-bh.width*3, 1, file);
					}
					return bh.sizeFile;
				case TYPE::BGR_24:
					bh.bpp = 24;
					bh.sizeFile = 54+((bh.width*3+3)&~3)*bh.height;
					fwrite(&bh, sizeof(bmpHead), 1, file);
					for (int y=bh.height-1; y>=0; y--) {
						for (int x=0; x<bh.width; x++) {
							fwrite(&((rgb*)(this->img))[x+this->width*y], 3, 1, file);
						}
						fwrite(&null1, ((bh.width*3+3)&~3)-bh.width*3, 1, file);
					}
					printf("offset\\%i", ((bh.width*3+3)&~3)-bh.width*3);
					return bh.sizeFile;
			}
			return bh.sizeFile;// </editor-fold>
		case GR_IMG_I_TGA:// <editor-fold defaultstate="collapsed" desc="GR_IMG_I_TGA">
			tgaHead th;
			th.IDLength = 0;
			th.colorMapType = 0;
			th.colorMapIndex = 0;
			th.colorMapLength = 0;
			th.colorMapItemSize = 0;
			th.x = 0;
			th.y = 0;
			th.width = this->width;
			th.height = this->height;
			th.imgDescriptor = TGA_TRANSFER_Y;
			if (this->texType==TYPE::RGBA_32) {// <editor-fold defaultstate="collapsed" desc="GR_RGBA_32">
				th.colorDepth = TGA_DEP_32;
				th.imgType = TGA_TYPE_TC;
				fwrite(&th, sizeof(tgaHead), 1, file);
				for(int y=0; y<this->height; y++) {
					for (int x=0; x<this->width; x++) {
						fwrite( &(((argb*)(this->img))[x+this->width*y].b), 1, 1, file);
						fwrite( &(((argb*)(this->img))[x+this->width*y].g), 1, 1, file);
						fwrite( &(((argb*)(this->img))[x+this->width*y].r), 1, 1, file);
						fwrite( &(((argb*)(this->img))[x+this->width*y].a), 1, 1, file);
					}
				}// </editor-fold>
			}else if (this->texType==TYPE::BGRA_32) {// <editor-fold defaultstate="collapsed" desc="GR_BGRA_32">
				th.colorDepth = TGA_DEP_32;
				th.imgType = TGA_TYPE_TC;
				fwrite(&th, sizeof(tgaHead), 1, file);
				for(int y=0; y<this->height; y++) {
					fwrite( &(((abgr*)(this->img))[this->width*y]), this->width*4, 1, file);
				}// </editor-fold>
			}else if (this->texType==TYPE::RGB_24) {// <editor-fold defaultstate="collapsed" desc="GR_RGB_24">
				th.colorDepth = TGA_DEP_24;
				th.imgType = TGA_TYPE_TC;
				fwrite(&th, sizeof(tgaHead), 1, file);
				for(int y=0; y<this->height; y++) {
					for (int x=0; x<this->width; x++) {
						fwrite( &(((rgb*)(this->img))[x+this->width*y].b), 1, 1, file);
						fwrite( &(((rgb*)(this->img))[x+this->width*y].g), 1, 1, file);
						fwrite( &(((rgb*)(this->img))[x+this->width*y].r), 1, 1, file);
					}
				}// </editor-fold>
			}else if (this->texType==TYPE::BGR_24) {// <editor-fold defaultstate="collapsed" desc="GR_BGR_24">
				th.colorDepth = TGA_DEP_24;
				th.imgType = TGA_TYPE_TC;
				fwrite(&th, sizeof(tgaHead), 1, file);
				for(int y=0; y<this->height; y++) {
					fwrite( &(((bgr*)(this->img))[this->width*y]), this->width*3, 1, file);
				}// </editor-fold>
			}else if (this->texType==TYPE::MONO_8) {// <editor-fold defaultstate="collapsed" desc="GR_MONO_8">
				th.colorDepth = TGA_DEP_8;
				th.imgType = TGA_TYPE_MONOCHROME;
				fwrite(&th, sizeof(tgaHead), 1, file);
				for(int y=0; y<this->height; y++) {
					fwrite( &(((char*)(this->img))[this->width*y]), this->width, 1, file);
				}// </editor-fold>
			}else{
				printf("ERROR_TGA_SAVE");
			}
			break;
	}// </editor-fold>
	return true;
}*/
