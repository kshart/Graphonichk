#include <malloc.h>
#include <png.h>
#include <jpeglib.h>
#include <jerror.h>
#include <setjmp.h>
#include <zlib.h>

#include "grBaseTypes.h"

#define ADD_TEXTURE_TO_UPDATE_BUFFER(tex) \
	TextureToUpdateTask *task = new TextureToUpdateTask(tex);\
	Windows::window->eachFrame.addTask(task, 0);\
	//Texture::buffer.push_back(tex);
	//Texture::toUpdate++;
#define ADD_TEXTURE_TO_DELETE_BUFFER(glid) \
	TextureToDeleteTask *task = new TextureToDeleteTask(glid);\
	Windows::window->eachFrame.addTask(task, 0);\
	//Texture::buffer.push_back(tex);
	//Texture::toDelete++;
using namespace std;
using namespace Graphonichk;



Texture::Texture() :_loadedInFile(false) {
	this->event = Texture::NONE;
	this->img = NULL;
	this->width = 0;
	this->height = 0;
	this->format = 0;
	this->type = 0;
	this->GLID = 0;
}
Texture* Texture::getTexture(unsigned short w, unsigned short h, GLuint format, GLuint type) {
	Texture *tex = new Texture();
	glGenTextures( 1, &tex->GLID );
	if (tex->GLID==0) return nullptr;
	tex->width = w;
	tex->height = h;
	tex->type = type;
	tex->format = format;
	glBindTexture( GL_TEXTURE_2D, tex->GLID );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexImage2D( GL_TEXTURE_2D, 0, format, w, h, 0, format, type, NULL );
	return tex;
}
Texture::Texture(unsigned short w, unsigned short h, GLuint format, GLuint type) :_loadedInFile(false) {
	this->event = Texture::TO_UPDATE;
	this->img = NULL;
	this->width = w;
	this->height = h;
	this->format = format;
	this->type = type;
	this->GLID = 0;
	ADD_TEXTURE_TO_UPDATE_BUFFER(this);
}
Texture::Texture(string path) :_loadedInFile(true) {
	this->type = 0;
	this->width = this->height = 0;
	this->event = EVENT::TO_UPDATE;
	this->GLID = 0;
	this->img = new Image(path);
	ADD_TEXTURE_TO_UPDATE_BUFFER(this);
}
Texture::Texture(Image *img) :_loadedInFile(false) {
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
		case Image::MONO_1:
		case Image::MONO_2:
		case Image::MONO_4:
		case Image::MONO_16:
		case Image::MONO_ALPHA_8:
		case Image::MONO_ALPHA_16:
		case Image::BGR_48:
		case Image::RGB_48:
		case Image::BGRA_64:
		case Image::RGBA_64:
		case Image::PALETTE_1:
		case Image::PALETTE_2:
		case Image::PALETTE_4:
		case Image::PALETTE_8:
		case Image::NC:
			fputs("Texture::Texture(Image *img) NOT SUPPORTED TYPE", stderr);
			break;
	}
	this->img = img;
	this->width = img->width;
	this->height = img->height;
	this->event = EVENT::TO_UPDATE;
	this->GLID = 0;
	ADD_TEXTURE_TO_UPDATE_BUFFER(this);
}
Texture::~Texture() {
	if (this->_loadedInFile) delete this->img;
	this->event = EVENT::TO_DELETE;
	ADD_TEXTURE_TO_DELETE_BUFFER(this->GLID);
}
void Texture::trace() {
	printf("\t<Texture w='%i' h='%i' event='%i' type='%i' format='%i' GLID='%i'/>\n", 
		this->width, this->height, this->event, this->type, this->format, this->GLID);
}


typedef struct{
	char *data;
	unsigned int size, pt;
} DATA;
typedef struct {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
} my_error_mgr;
void PNGReadData(png_structp png_ptr, png_bytep data, png_size_t length) {
	//printf("PNGReadData %i\n", ((DATA*)png_get_io_ptr(png_ptr))->size );
	DATA *d = ((DATA*)png_get_io_ptr(png_ptr));
	if (d->pt+length > d->size) return;
	memcpy(data, d->data+d->pt, length);
	d->pt += length;
}
Image::Image(unsigned short width, unsigned short height, TYPE type, char *raw) {
	this->width = width;
	this->height = height;
	this->raw = raw;
	this->type = type;
	this->status = STATUS::LOADED;
}
Image::Image(char* data, unsigned int size) {
	this->load(data, size);
}
Image::Image(string path) {
	this->name = path;
	this->width = this->height = 0;
	this->status = Image::TO_LOAD;
	this->raw = NULL;
	this->type = Image::NC;
	FileLoad *fl = new FileLoad(path);
	fl->addEventHandler(EventFileLoad::FILE_SUCCESS, Image::loaded, this);
	
}
void Image::copyPixels(unsigned short x, unsigned short y, unsigned short width, unsigned short height, char *raw) {
	
}
bool Image::load(char* data, unsigned int size) {
	if ( !name.empty() && raw==NULL ) {
		size_t rowSize;
		if ( ((uint16_t*)data)[0] == 0x4D42L ) {// <editor-fold defaultstate="collapsed" desc="BMP">
			bmpHead *bmph = ((bmpHead*)data);
			unsigned short align;
			if (bmph->null1!=0 || bmph->null2!=0 || bmph->sizeHead!=40 || bmph->layers!=1) {
				printf("<ERROR text='FILE_BMP_INVALID'/>\n");
				this->status = Image::ERR_LOAD;
				return false;
			}
			this->width = bmph->width;
			this->height = bmph->height; //(w*3+3)&~3 -w;
			if (bmph->bpp == 24) {
				this->type = TYPE::BGR_24;
				align = ((this->width*3+3)&~3) -this->width*3;
				this->raw = malloc( (this->width*3 + align)*this->height);
				rowSize = ((this->width*3+3)&~3);
				for(int y=0; y<this->height; y++) {
					memcpy( (void*)((intptr_t)(this->raw)+y*rowSize), data+bmph->offsetPixels+(this->height-y-1)*rowSize, rowSize);
				}
			}else if (bmph->bpp == 32) {
				this->type = TYPE::RGBA_32;		
				this->raw = malloc(this->width*this->height*4);
				rowSize = this->width*4;
				for(int y=0; y<this->height; y++) {
					memcpy( (void*)((intptr_t)(this->raw)+y*rowSize), data+bmph->offsetPixels+(this->height-y-1)*rowSize, rowSize);
				}
			}
		//</editor-fold>
		}else if ( ((uint64_t *)data)[0] == 0x0A1A0A0D474E5089L) {// <editor-fold defaultstate="collapsed" desc="PNG">
			printf("PNG\n");
			png_structp png_ptr;
			png_infop info_ptr;
			png_uint_32 retval;
			int bitDepth = 0, colorType = -1;
			DATA d1;
			d1.data = data;
			d1.size = size;
			d1.pt = 0;
			png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
			if (!png_ptr) {
				this->status = Image::ERR_LOAD;
				return false;
			}
			info_ptr = png_create_info_struct(png_ptr);
			if (!info_ptr) {
				png_destroy_read_struct(&png_ptr, NULL, NULL);
				this->status = Image::ERR_LOAD;
				return false;
			}
			png_set_read_fn(png_ptr, &d1, PNGReadData);
			png_set_sig_bytes(png_ptr, 0);
			if (setjmp(png_jmpbuf(png_ptr))) {
				printf("setjmp pt=%i w=%i h=%i\n", d1.pt, this->width, this->height);
				png_destroy_read_struct(&png_ptr, NULL, NULL);
				this->status = Image::ERR_LOAD;
				return false;
			}
			png_set_keep_unknown_chunks(png_ptr, 1, NULL, 0);
			png_read_info(png_ptr, info_ptr);
			retval = png_get_IHDR(png_ptr, info_ptr, ((png_uint_32*)&this->width), ((png_uint_32*)&this->height), &bitDepth, &colorType, NULL, NULL, NULL);
			if (!retval) {
				png_destroy_read_struct(&png_ptr, NULL, NULL);
				this->status = Image::ERR_LOAD;
				return false;
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
			this->raw = malloc(rowSize*this->height);
			const png_uint_32 bytesPerRow = png_get_rowbytes(png_ptr, info_ptr);
			byte* rowData = (byte*)malloc(bytesPerRow);

			for(int y=0; y<this->height; y++) {
				png_read_row(png_ptr, (png_bytep)rowData, NULL);
				memcpy( (void*)((intptr_t)(this->raw)+y*rowSize), rowData, bytesPerRow);
			}
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			printf("Image size=%iKB w=%i h=%i depth=%i color=%i rowSize=%i\n", d1.size/1024, this->width, this->height, bitDepth, colorType, bytesPerRow);// </editor-fold>
		}else if ( ((uint16_t*)data)[0] == 0xD8FFL) {// <editor-fold defaultstate="collapsed" desc="JPEG">
			printf("JPEG\n");
			struct jpeg_decompress_struct cinfo;
			my_error_mgr jerr;
			//FILE * infile;
			JSAMPARRAY buffer;
			int row_stride;
			//src = reinterpret_cast<JpegStream*> (cinfo->src);
			cinfo.err = jpeg_std_error(&jerr.pub);
			//jerr.pub.error_exit = my_error_exit;
			if (setjmp(jerr.setjmp_buffer)) {
				printf("ERROR JPEG\n");
				jpeg_destroy_decompress(&cinfo);
				//fclose(infile);
				this->status = Image::ERR_LOAD;
				return false;
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
			this->raw = malloc(rowSize*this->height);

			jpeg_start_decompress(&cinfo);
			unsigned char *buf = (unsigned char*)malloc(this->width*cinfo.num_components);
			for(int y=0; cinfo.output_scanline<cinfo.output_height; y++) {
				jpeg_read_scanlines(&cinfo, &buf, 1);
				memcpy( (void*)((intptr_t)(this->raw)+y*rowSize), buf, this->width*cinfo.num_components);
			}
			jpeg_finish_decompress(&cinfo);

			printf("JPEG w='%i' h='%i' %i\n", cinfo.image_height, cinfo.image_width, cinfo.output_height);// </editor-fold>
		}
		this->status = Image::LOADED;
		return true;
	}else{
		return false;
	}
}
void Image::loaded(const EventFileLoad* e) {
	Image *img = (Image*)(e->obj);
	img->load( (char*)(e->file->data), e->file->size);
}


TextureToUpdateTask::TextureToUpdateTask(Texture *t) :_tex(t) {
	
}
int TextureToUpdateTask::processExecute() {
	if (this->_tex->event!=Texture::TO_UPDATE) {
		TextureToUpdateTask *task = new TextureToUpdateTask(this->_tex);
		Windows::window->eachFrame.addTask(task, 0);
		return false;
	}
	if (_tex->img==NULL) {// <editor-fold defaultstate="collapsed" desc="tex->img==NULL">
		if (_tex->GLID==0) {
			glGenTextures( 1, &_tex->GLID );
			if (_tex->GLID==0) {
				TextureToUpdateTask *task = new TextureToUpdateTask(this->_tex);
				Windows::window->eachFrame.addTask(task, 0);
				return false;
			}
			glBindTexture( GL_TEXTURE_2D, _tex->GLID );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexImage2D( GL_TEXTURE_2D, 0, _tex->format, _tex->width, _tex->height, 0, _tex->format, _tex->type, NULL );
		}else{
			glBindTexture( GL_TEXTURE_2D, _tex->GLID );
			glTexImage2D( GL_TEXTURE_2D, 0, _tex->format, _tex->width, _tex->height, 0, _tex->format, _tex->type, NULL );
		}
		this->_tex->event = Texture::LOADED;//</editor-fold>
	}else{// <editor-fold defaultstate="collapsed">
		if (_tex->img->status!=Image::LOADED) {
			TextureToUpdateTask *task = new TextureToUpdateTask(this->_tex);
			Windows::window->eachFrame.addTask(task, 0);
			return false;
		}
		switch (_tex->img->type) {
			case Image::RGBA_32:
				printf("\tImage::RGBA_32\n");
				_tex->format = GL_RGBA;
				_tex->type = GL_UNSIGNED_BYTE;
				break;
			case Image::BGRA_32:
				_tex->format = GL_BGRA;
				_tex->type = GL_UNSIGNED_BYTE;
				break;
			case Image::RGB_24:
				printf("\tImage::RGB_24\n");
				_tex->format = GL_RGB;
				_tex->type = GL_UNSIGNED_BYTE;
				break;
			case Image::BGR_24:
				printf("\tImage::BGR_24\n");
				_tex->format = GL_BGR;
				_tex->type = GL_UNSIGNED_BYTE;
				break;
			case Image::MONO_8:
				printf("\tImage::MONO_8\n");
				_tex->format = GL_ALPHA;
				_tex->type = GL_UNSIGNED_BYTE;
				break;
		}
		printf("\tImage %i\n", _tex->img->raw);
		printf("\t//////\n");
		_tex->width = _tex->img->width;
		_tex->height = _tex->img->height;
		if (_tex->GLID==0) {
			glGenTextures( 1, &_tex->GLID );
			if (_tex->GLID==0) {
				TextureToUpdateTask *task = new TextureToUpdateTask(this->_tex);
				Windows::window->eachFrame.addTask(task, 0);
				return false;
			}
			glBindTexture( GL_TEXTURE_2D, _tex->GLID );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexImage2D( GL_TEXTURE_2D, 0, _tex->format, _tex->width, _tex->height, 0, _tex->format, _tex->type, _tex->img->raw );
		}else{
			glBindTexture( GL_TEXTURE_2D, _tex->GLID );
			glTexImage2D( GL_TEXTURE_2D, 0, _tex->format, _tex->width, _tex->height, 0, _tex->format, _tex->type, _tex->img->raw );
		}
		_tex->event = Texture::LOADED;
	}//</editor-fold>
}
TextureToDeleteTask::TextureToDeleteTask(GLuint glid) :_GLID(glid) {
	
}
int TextureToDeleteTask::processExecute() {
	glDeleteTextures(1, &this->_GLID);
	return true;
}