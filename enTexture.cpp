#include "grMain.h"

#include <png.h>
#include <jpeglib.h>
#include <jerror.h>
#include <setjmp.h>
#include <zlib.h>


using namespace std;
using namespace Graphonichk;



Texture::Texture(unsigned short rectCount, usvec4 *rect) :
		_loadedInFile(false), event(Texture::NONE),
		img(nullptr), GLID(0),
		width(0), height(0),
		format(0), type(0), 
		rects(rectCount) {
	memcpy(this->rects.data, rect, rectCount*sizeof(usvec4));
}
Texture* Texture::getTexture(unsigned short w, unsigned short h, GLuint format, GLuint internalType, GLuint type, unsigned short rectCount, usvec4 *rect) {
	Texture *tex = new Texture(rectCount, rect);
	glGenTextures( 1, &tex->GLID );
	if (tex->GLID==0) {
		delete tex;
		return nullptr;
	}
	tex->width = w;
	tex->height = h;
	tex->type = type;
	tex->internalType = internalType;
	tex->format = format;
	tex->event = Texture::LOADED;
	glBindTexture( GL_TEXTURE_2D, tex->GLID );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );//GL_NEAREST
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, internalType, w, h, 0, format, type, NULL );
	return tex;
}
Texture::Texture(unsigned short w, unsigned short h, GLuint internalType, GLuint format, GLuint type, unsigned short rectCount, usvec4 *rect) :
		_loadedInFile(false), event(Texture::TO_UPDATE),
		img(nullptr), GLID(0),
		width(w), height(h),
		format(format), internalType(internalType), type(type),
		rects(rectCount) {
	memcpy(this->rects.data, rect, rectCount*sizeof(usvec4));
	ADD_TEXTURE_TO_UPDATE_BUFFER(this);
}
Texture::Texture(string path, unsigned short rectCount, usvec4 *rect) :
		_loadedInFile(true), event(Texture::TO_UPDATE),
		img(new Image(path)), GLID(0),
		width(0), height(0),
		format(0), type(0), 
		rects(rectCount) {
	memcpy(this->rects.data, rect, rectCount*sizeof(usvec4));
	ADD_TEXTURE_TO_UPDATE_BUFFER(this);
}
Texture::Texture(Image *img, unsigned short rectCount, usvec4 *rect) :
		_loadedInFile(false), event(Texture::TO_UPDATE),
		img(img), GLID(0),
		width(img->width), height(img->height), 
		rects(rectCount) {
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
			this->format = GL_LUMINANCE;
			this->type = GL_UNSIGNED_BYTE;
			break;
		case Image::ALPHA_8:
			this->format = GL_ALPHA;
			this->type = GL_UNSIGNED_BYTE;
			break;
		case Image::MONO_1:
		case Image::MONO_2:
		case Image::MONO_4:
		case Image::MONO_16:
		case Image::ALPHA_16:
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
	memcpy(this->rects.data, rect, rectCount*sizeof(usvec4));
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
int Image::convert(TYPE type) {
	switch (this->type) {
		case Image::RGBA_32:
			puts("Image::RGBA_32");
			switch (type) {
				case Image::RGBA_32: return true;
				case Image::BGRA_32:
					for(size_t i=0, size=this->width*this->height; i<size; i++) {
						ubvec4 color = ((ubvec4*)this->raw)[i];
						((ubvec4*)this->raw)[i].r = color.b;
						((ubvec4*)this->raw)[i].b = color.r;
					}
					this->type = type;
					return true;
				case Image::RGB_24:
					for(size_t i=1, size=this->width*this->height; i<size; i++) {
						((ubvec3*)this->raw)[i].r = ((ubvec4*)this->raw)[i].r;
						((ubvec3*)this->raw)[i].g = ((ubvec4*)this->raw)[i].g;
						((ubvec3*)this->raw)[i].b = ((ubvec4*)this->raw)[i].b;
					}
					this->raw = realloc(this->raw, this->width*this->height*sizeof(ubvec3));
					this->type = type;
					return true;
				case Image::BGR_24:
					for(size_t i=1, size=this->width*this->height; i<size; i++) {
						ubvec4 color = ((ubvec4*)this->raw)[i];
						((ubvec3*)this->raw)[i].r = color.b;
						((ubvec3*)this->raw)[i].g = color.g;
						((ubvec3*)this->raw)[i].b = color.r;
					}
					this->raw = realloc(this->raw, this->width*this->height*sizeof(ubvec3));
					this->type = type;
					return true;
				case Image::ALPHA_8:
					for(size_t i=0, size=this->width*this->height; i<size; i++) {
						ubvec4 color = ((ubvec4*)this->raw)[i];
						((uint8_t*)this->raw)[i] = color.a;
					}
					this->raw = realloc(this->raw, this->width*this->height);
					this->type = type;
					return true;
				case Image::MONO_8:
					for(size_t i=0, size=this->width*this->height; i<size; i++) {
						ubvec4 color = ((ubvec4*)this->raw)[i];
						((uint8_t*)this->raw)[i] = (color.r+color.g+color.b)/3;
					}
					this->raw = realloc(this->raw, this->width*this->height);
					this->type = type;
					return true;
				case Image::MONO_1: break;
				case Image::MONO_2: break;
				case Image::MONO_4: break;
				case Image::MONO_16:
					for(size_t i=0, size=this->width*this->height; i<size; i++) {
						ubvec4 color = ((ubvec4*)this->raw)[i];
						((uint16_t*)this->raw)[i] = (uint16_t)((color.r+color.g+color.b)*85);
					}
					this->raw = realloc(this->raw, this->width*this->height*sizeof(uint16_t));
					this->type = type;
					return true;
				case Image::ALPHA_16:
					for(size_t i=0, size=this->width*this->height; i<size; i++) {
						ubvec4 color = ((ubvec4*)this->raw)[i];
						((uint16_t*)this->raw)[i] = (uint16_t)(color.a*0x101);
					}
					this->raw = realloc(this->raw, this->width*this->height*sizeof(uint16_t));
					this->type = type;
					return true;
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
			break;
		case Image::BGRA_32:
			break;
		case Image::RGB_24:
			break;
		case Image::BGR_24:
			break;
		case Image::MONO_8:
		case Image::ALPHA_8:
			break;
		case Image::MONO_1:
		case Image::MONO_2:
		case Image::MONO_4:
		case Image::MONO_16:
		case Image::ALPHA_16:
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
	return true;
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
							this->type = TYPE::ALPHA_8;
							rowSize = ((this->width+3)&~3);
							break;
						case 16:
							this->type = TYPE::ALPHA_16;
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
			png_bytep rowData = (png_bytep)malloc(bytesPerRow);

			for(int y=0; y<this->height; y++) {
				png_read_row(png_ptr, rowData, NULL);
				memcpy( (void*)((intptr_t)(this->raw)+y*rowSize), rowData, bytesPerRow);
			}
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			printf("Image size=%fKB w=%i h=%i depth=%i color=%i rowSize=%i\n", (float)d1.size/1024.0f, this->width, this->height, bitDepth, colorType, (int)bytesPerRow);// </editor-fold>
		}else if ( ((uint16_t*)data)[0] == 0xD8FFL) {// <editor-fold defaultstate="collapsed" desc="JPEG">
			puts("JPEG");
			struct jpeg_decompress_struct cinfo;
			my_error_mgr jerr;
			JSAMPARRAY buffer;
			int row_stride;
			cinfo.err = jpeg_std_error(&jerr.pub);
			if (setjmp(jerr.setjmp_buffer)) {
				puts("ERROR JPEG");
				jpeg_destroy_decompress(&cinfo);
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
	if (_tex->event!=Texture::TO_UPDATE) return false;
	if (_tex->img==nullptr) {// <editor-fold defaultstate="collapsed" desc="tex->img==NULL">
		if (_tex->GLID==0) {
			glGenTextures( 1, &_tex->GLID );
			if (_tex->GLID==0) return false;
			glBindTexture( GL_TEXTURE_2D, _tex->GLID );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexImage2D( GL_TEXTURE_2D, 0, _tex->internalType, _tex->width, _tex->height, 0, _tex->format, _tex->type, NULL );
			if (_tex->rects.size>0) {
				glGenTextures( 1, &_tex->rectGLID );
				if (_tex->rectGLID==0) return false;
				Array<usvec4> arr(_tex->rects.size);
				unsigned short mnWidth = round(((float)0xFFFF)/_tex->width);
				unsigned short mnHeight = round(((float)0xFFFF)/_tex->height);
				for(size_t i=0; i<_tex->rects.size; i++) {
					arr[i].x = _tex->rects[i].x*mnWidth;
					arr[i].y = _tex->rects[i].y*mnHeight;
					arr[i].z = _tex->rects[i].z*mnWidth;
					arr[i].w = _tex->rects[i].w*mnHeight;
				}
				glBindTexture(GL_TEXTURE_1D, _tex->rectGLID );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
				glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA16, _tex->rects.size, 0, GL_RGBA, GL_UNSIGNED_SHORT, arr.data);
			} 
		}else{
			glBindTexture( GL_TEXTURE_2D, _tex->GLID );
			glTexImage2D( GL_TEXTURE_2D, 0, _tex->internalType, _tex->width, _tex->height, 0, _tex->format, _tex->type, NULL );
			if (_tex->rects.size>0) {
				glGenTextures( 1, &_tex->rectGLID );
				if (_tex->rectGLID==0) return false;
				Array<usvec4> arr(_tex->rects.size);
				unsigned short mnWidth = round(((float)0xFFFF)/_tex->width);
				unsigned short mnHeight = round(((float)0xFFFF)/_tex->height);
				for(size_t i=0; i<_tex->rects.size; i++) {
					arr[i].x = _tex->rects[i].x*mnWidth;
					arr[i].y = _tex->rects[i].y*mnHeight;
					arr[i].z = _tex->rects[i].z*mnWidth;
					arr[i].w = _tex->rects[i].w*mnHeight;
				}
				glBindTexture(GL_TEXTURE_1D, _tex->rectGLID );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
				glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA16, _tex->rects.size, 0, GL_RGBA, GL_UNSIGNED_SHORT, arr.data);
			} 
		}
		_tex->event = Texture::LOADED;//</editor-fold>
	}else{// <editor-fold defaultstate="collapsed">
		if (_tex->img->status!=Image::LOADED) return false;
		switch (_tex->img->type) {
			case Image::RGBA_32:
				_tex->format = GL_RGBA;
				_tex->internalType = GL_RGBA8;
				_tex->type = GL_UNSIGNED_BYTE;
				break;
			case Image::BGRA_32:
				_tex->format = GL_BGRA;
				_tex->internalType = GL_BGRA;
				_tex->type = GL_UNSIGNED_BYTE;
				break;
			case Image::RGB_24:
				_tex->format = GL_RGB;
				_tex->internalType = GL_RGB;
				_tex->type = GL_UNSIGNED_BYTE;
				break;
			case Image::BGR_24:
				_tex->format = GL_BGR;
				_tex->internalType = GL_BGR;
				_tex->type = GL_UNSIGNED_BYTE;
				break;
			case Image::MONO_8:
				_tex->format = GL_INTENSITY;
				_tex->internalType = GL_INTENSITY;
				_tex->type = GL_UNSIGNED_BYTE;
				break;
			case Image::ALPHA_8:
				_tex->format = GL_ALPHA;
				_tex->internalType = GL_ALPHA;
				_tex->type = GL_UNSIGNED_BYTE;
				break;
		}
		_tex->width = _tex->img->width;
		_tex->height = _tex->img->height;
		if (_tex->GLID==0) {
			glGenTextures( 1, &_tex->GLID );
			if (_tex->GLID==0) return false;
			glBindTexture( GL_TEXTURE_2D, _tex->GLID );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexImage2D( GL_TEXTURE_2D, 0, _tex->internalType, _tex->width, _tex->height, 0, _tex->format, _tex->type, _tex->img->raw );
			if (_tex->rects.size>0) {
				glGenTextures( 1, &_tex->rectGLID );
				if (_tex->rectGLID==0) return false;
				Array<usvec4> arr(_tex->rects.size);
				unsigned short mnWidth = round(((float)0xFFFF)/_tex->width);
				unsigned short mnHeight = round(((float)0xFFFF)/_tex->height);
				for(size_t i=0; i<_tex->rects.size; i++) {
					arr[i].x = _tex->rects[i].x*mnWidth;
					arr[i].y = _tex->rects[i].y*mnHeight;
					arr[i].z = _tex->rects[i].z*mnWidth;
					arr[i].w = _tex->rects[i].w*mnHeight;
				}
				glBindTexture(GL_TEXTURE_1D, _tex->rectGLID );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
				glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA16, _tex->rects.size, 0, GL_RGBA, GL_UNSIGNED_SHORT, arr.data);
			} 
		}else{
			glBindTexture( GL_TEXTURE_2D, _tex->GLID );
			glTexImage2D( GL_TEXTURE_2D, 0, _tex->internalType, _tex->width, _tex->height, 0, _tex->format, _tex->type, _tex->img->raw );
			if (_tex->rects.size>0) {
				glGenTextures( 1, &_tex->rectGLID );
				if (_tex->rectGLID==0) return false;
				Array<usvec4> arr(_tex->rects.size);
				unsigned short mnWidth = round(((float)0xFFFF)/_tex->width);
				unsigned short mnHeight = round(((float)0xFFFF)/_tex->height);
				for(size_t i=0; i<_tex->rects.size; i++) {
					arr[i].x = _tex->rects[i].x*mnWidth;
					arr[i].y = _tex->rects[i].y*mnHeight;
					arr[i].z = _tex->rects[i].z*mnWidth;
					arr[i].w = _tex->rects[i].w*mnHeight;
				}
				glBindTexture(GL_TEXTURE_1D, _tex->rectGLID );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
				glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA16, _tex->rects.size, 0, GL_RGBA, GL_UNSIGNED_SHORT, arr.data);
			} 
		}
		_tex->event = Texture::LOADED;
	}//</editor-fold>
	return true;
}
TextureToDeleteTask::TextureToDeleteTask(GLuint glid) :_GLID(glid) {
	
}
int TextureToDeleteTask::processExecute() {
	glDeleteTextures(1, &this->_GLID);
	return true;
}