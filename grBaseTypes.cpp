#include <string>
#include "grBaseTypes.h"
using namespace std;
using namespace grEngine;

string get_ext (const string& st) {
    size_t pos = st.rfind('.');
    if (pos <= 0) return "";
    return st.substr(pos+1, std::string::npos);
}



System grEngine::root;
void System::init(HINSTANCE hi) {
	root.window = NULL;
	root.hInstance = hi;
	Windows::regFirstWin();
	FileLoad::init();
}
void System::threadDataSysInit(void *data){
	ThreadDataSys *s = (ThreadDataSys*)data;
	memcpy(&root, s->system, sizeof(System));
	root.window = s->window;
}
System::System() {
	root.window = NULL;
	root.hInstance = NULL;
}
System::System(Windows *win) {
	this->windows = root.windows;
	this->window = win;
}


/*
Shape* grEngine::loadImageFD (int type, FILE *file, uint32_t offset, uint32_t size) {
	Shape* sh = newShape();
	fseek(file, offset, SEEK_SET);
	if ( type==GR_IMG_I_BMP ) {// <editor-fold defaultstate="collapsed" desc="GR_TYPE_BMP_TO_CACHE">
		bmpHead bmph;
		fread(&bmph, sizeof(bmpHead), 1, file);
		printf("File signature=%i size=%i, offset=%i, sizeHead=%i, width=%i, height=%i, layers=%i, ", bmph.signature, bmph.sizeFile, bmph.offsetPixels, bmph.sizeHead, bmph.width, bmph.height, bmph.layers);
		if (bmph.null1!=0 || bmph.null2!=0 || bmph.sizeHead!=40 || bmph.layers!=1) {
			printf("ERROR_FILE_BMP_INVALID\n");
			return sh;
		}
		sh->width = bmph.width;
		sh->height = bmph.height;
		sh->texWidth = (bmph.width + 4-1) & ~(4-1);
		sh->texHeight = (bmph.height + 4-1) & ~(4-1);
		sh->type = GR_TYPE_BMP;
		fseek(file, offset+bmph.offsetPixels, SEEK_SET);
		if (bmph.bpp == 24) {// <editor-fold defaultstate="collapsed" desc="GR_RGB_24">
			sh->imgType = GR_RGB_24;
			sh->img = malloc(sh->texWidth*sh->texHeight*sizeof(rgb));
			char *buff = (char*)malloc(3 );
			for (int x=sh->width; x<sh->texWidth; x++) {
				for (int y=0; y<sh->texHeight; y++) {
					((rgb*)(sh->img))[x+sh->texWidth*y].r = 0xFF;
					((rgb*)(sh->img))[x+sh->texWidth*y].g = 0;
					((rgb*)(sh->img))[x+sh->texWidth*y].b = 0;
				}
			}
			for (int x=0; x<sh->width; x++) {
				for (int y=sh->height; y<sh->texHeight; y++) {
					((rgb*)(sh->img))[x+sh->texWidth*y].r = 0xFF;
					((rgb*)(sh->img))[x+sh->texWidth*y].g = 0;
					((rgb*)(sh->img))[x+sh->texWidth*y].b = 0;
				}
			}
			for (int y=0; y<bmph.height; y++) {
				for (int x = 0; x<bmph.width; x++) {
					fread(buff, 3, 1, file);
					((rgb*)(sh->img))[x+sh->texWidth*y].r = buff[2];
					((rgb*)(sh->img))[x+sh->texWidth*y].g = buff[1];//((rgb*)(buff))[x+bmph.width*(bmph.height-y-1)].g;
					((rgb*)(sh->img))[x+sh->texWidth*y].b = buff[0];//((rgb*)(buff))[x+bmph.width*(bmph.height-y-1)].r;

				}
				fread(buff, ((bmph.width*3+4-1)&~(4-1))-bmph.width*3, 1, file);
			}
			free(buff);//</editor-fold>
		}else if (bmph.bpp == 32) {// <editor-fold defaultstate="collapsed" desc="GR_RGBA_32">
			sh->imgType = GR_RGBA_32;
			sh->img = malloc(sh->texWidth*sh->texHeight*sizeof(rgba));
			int color32;
			for (int x=sh->width; x<sh->texWidth; x++)
				for (int y=0; y<sh->texHeight; y++)
					((rgba*)(sh->img))[x+sh->texWidth*y].color = 0xFF0000FF;
			for (int x=0; x<sh->width; x++)
				for (int y=sh->height; y<sh->texHeight; y++)
					((rgba*)(sh->img))[x+sh->texWidth*y].color = 0xFF0000FF;

			for (int y=0; y<bmph.height; y++) {
				for (int x = 0; x<bmph.width; x++) {
					fread(&color32, 3, 1, file);
					((rgba*)(sh->img))[x+sh->texWidth*y].color = color32;

				}
				fread(&color32, ((bmph.width*3+4-1)&~(4-1))-bmph.width*3, 1, file);
			}
		}// </editor-fold>
		//</editor-fold>
	}else if ( type==GR_IMG_I_TGA ) {// <editor-fold defaultstate="collapsed" desc="GR_IMG_TGA">
		tgaHead th;
		void *buff;
		fread(&th, sizeof(tgaHead), 1, file);
		printf("File width=%i, height=%i, IDLength=%i, colorMapType=%i, imgType=%i\n", th.width, th.height, th.IDLength, th.colorMapType, th.imgType);

		sh->x = th.x;
		sh->y = th.y;
		sh->width = th.width;
		sh->height = th.height;
		sh->texWidth = (th.width + 4-1) & ~(4-1);
		sh->texHeight = (th.height + 4-1) & ~(4-1);
		switch(th.imgType) {
			case TGA_TYPE_PALETTE://<editor-fold defaultstate="collapsed" desc="TGA_TYPE_PALETTE">
				break;//</editor-fold>
			case TGA_TYPE_TC://<editor-fold defaultstate="collapsed" desc="TGA_TYPE_TC">
				sh->type = GR_TYPE_BMP_TO_CACHE;
				if (th.colorDepth==TGA_DEP_32) {
					printf("32bits\n");
					buff = malloc(th.width*th.height*sizeof(rgba));
					sh->img = sh->img = malloc(sh->texWidth*sh->texHeight*sizeof(rgba));
					sh->imgType = GR_BGRA_32;
					fseek(file, offset+sizeof(tgaHead)+th.IDLength+th.colorMapType, SEEK_SET);
					fread(buff, th.width*th.height*sizeof(rgba), 1, file);
					if (th.imgDescriptor&TGA_TRANSFER_X && th.imgDescriptor&TGA_TRANSFER_Y) {//<editor-fold defaultstate="collapsed" desc="TGA_TRANSFER">
						printf("xy\n");
						for (int y=0; y<th.height; y++) {
							for (int x=th.width-1; x>=0; x--) {
								((rgba*)(sh->img))[x+sh->texWidth*y] = ((rgba*)buff)[th.height-x-1+th.width*y];
							}
						}
					}else if (th.imgDescriptor&TGA_TRANSFER_X) {
						printf("x\n");
						for (int y=0; y<th.height; y++) {
							for (int x=0; x<th.width; x++) {
								((rgba*)(sh->img))[x+sh->texWidth*y] = ((rgba*)buff)[th.height-x-1+th.width*(th.height-y-1)];
							}
						}
					}else if (th.imgDescriptor&TGA_TRANSFER_Y) {
						printf("y\n");
						for (int y=0; y<th.height; y++) {
							for (int x=0; x<th.width; x++) {
								((rgba*)(sh->img))[x+sh->texWidth*y] = ((rgba*)buff)[x+th.width*y];
							}
						}
					}else{
						printf("none\n");
						for (int y=0; y<th.height; y++) {
							for (int x=0; x<th.width; x++) {
								((rgba*)(sh->img))[x+sh->texWidth*y] = ((rgba*)buff)[x+th.width*(th.height-y-1)];
							}
						}
					}
					free(buff);//</editor-fold>
				}else if (th.colorDepth==TGA_DEP_24) {
					printf("24bits\n");
					buff = malloc(th.width*th.height*sizeof(rgb));
					sh->img = malloc(sh->texWidth*sh->texHeight*sizeof(rgb));
					sh->imgType = GR_BGR_24;
					fseek(file, offset+sizeof(tgaHead)+th.IDLength+th.colorMapType, SEEK_SET);
					fread(buff, th.width*th.height*sizeof(rgb), 1, file);
					if (th.imgDescriptor&TGA_TRANSFER_X && th.imgDescriptor&TGA_TRANSFER_Y) {//<editor-fold defaultstate="collapsed" desc="TGA_TRANSFER">
						printf("xy\n");
						for (int y=0; y<th.height; y++) {
							for (int x=0; x<th.width; x++) {
								((rgb*)(sh->img))[x+sh->texWidth*y] = ((rgb*)buff)[th.height-x-1+th.width*y];
							}
						}
					}else if (th.imgDescriptor&TGA_TRANSFER_X) {
						printf("x\n");
						for (int y=0; y<th.height; y++) {
							for (int x=0; x<th.width; x++) {
								((rgb*)(sh->img))[x+sh->texWidth*y] = ((rgb*)buff)[th.height-x-1+th.width*(th.height-y-1)];
							}
						}
					}else if (th.imgDescriptor&TGA_TRANSFER_Y) {
						printf("y\n");
						for (int y=0; y<th.height; y++) {
							for (int x=0; x<th.width; x++) {
								((rgb*)(sh->img))[x+sh->texWidth*y] = ((rgb*)buff)[x+th.width*y];
							}
						}
					}else{
						printf("none\n");
						for (int y=0; y<th.height; y++) {
							for (int x=0; x<th.width; x++) {
								((rgb*)(sh->img))[x+sh->texWidth*y] = ((rgb*)buff)[x+th.width*(th.height-y-1)];
							}
						}
					}
					free(buff);//</editor-fold>
				}
				break;//</editor-fold>
			case TGA_TYPE_MONOCHROME://<editor-fold defaultstate="collapsed" desc="TGA_TYPE_MONOCHROME">
				sh->img = malloc(th.width*th.height);
				break;//</editor-fold>
			default :
				sh->type = GR_TYPE_NULL;
		}// </editor-fold>
	}else{
		printf("ERROR_FILE_TYPE_NOT_SUPPORT\n");
	}
	return sh;
}*/
/*
Directory* grEngine::loadDirectory (std::string str) {
	FILE *file = fopen(str.c_str(), "rb");
	Directory* dir = loadDirectoryFD(str, file, 0, 0);
	fclose(file);
	return dir;
}
Directory* grEngine::loadDirectoryFD (std::string name, FILE *file, uint32_t offset, uint32_t size) {
	fseek(file, offset, SEEK_SET);
	sdHead sd;
	fread(&sd, sizeof(sdHead), 1, file);
	if (sd.signature!=DIR_SIGN) {
		return newDirectory(NULL);
	}
	printf("DirLoad name=%s, sign=%i, ver=%i, length=%i, size=%i\n", name.c_str(), sd.signature, sd.ver, sd.IDlength, sd.size);
	short *arr = (short*)malloc(10);
	Directory* dir = loadDirF(file, NULL, arr[1], arr[2], (unsigned int)(arr[3]));


	return dir;
}
Directory* grEngine::loadDirF (FILE *file, Directory* dir, short x, short y, int size) {
	Directory* dir1 = newDirectory(dir);
	short *arr = (short*)malloc(10);
	for (int i=0; i<size; i++) {
		fread(arr, 10, 1, file);
		if (arr[0]==GR_DIRECTORY) {
			loadDirF(file, dir1, arr[1], arr[2], (unsigned int)(arr[3]) );
		}else{
			addShape(newShape(), dir1);
		}
	}
}

int grEngine::saveDirectory (Directory *dir, std::string path) {
	FILE *file = fopen(path.c_str(), "wb");
	saveDirectoryFD(dir, path, file, 0, 0);
	fclose(file);
	return true;
}
int grEngine::saveDirectoryFD (Directory *dir, std::string path, FILE* file, uint32_t offset, uint32_t size) {
	sdHead head;
	head.ver = 1;
	head.signature = DIR_SIGN;
	head.IDlength = 0;
	head.size = 12;
	fwrite(&head, sizeof(sdHead), 1, file);
	uint32_t shapeID = 0;
	saveDirF(file, dir, &shapeID );
	printf("sID=%i\n", shapeID);
	//saveLib();
	return true;
}
int grEngine::saveDirF (FILE *file, Directory *dir, uint32_t *shapeID) {
	sdDir td;
	td.type = GR_DIRECTORY;
	td.x = dir->x;
	td.y = dir->y;
	td.size = 0x11223344;
	fwrite(&td, 10, 1, file);
	for (int i=0; i<dir->child.size(); i++) {
		switch (dir->child[i].type) {
			case GR_DIRECTORY:
				saveDirF(file, (Directory*)(dir->child[i].ln), shapeID );
				break;
			case GR_SHAPE:
				td.type = GR_SHAPE;
				td.x = ((Shape*)(dir->child[i].ln))->x;
				td.y = ((Shape*)(dir->child[i].ln))->y;
				if (((Shape*)(dir->child[i].ln))->libID == 0) {
					td.size = ++(*(shapeID));
					((Shape*)(dir->child[i].ln))->libID = td.size;
					printf("IDS=%i", td.size);
				}else{
					td.size = ((Shape*)(dir->child[i].ln))->libID;
				}
				fwrite(&td, 10, 1, file);
				break;
		}
	}
	return true;
}

int grEngine::saveShape (Shape* sh, std::string path, int type) {
	Image item;
	item.id = 0;
	item.path = path.c_str();
	item.pathLength = path.size();
	item.type = type;
	item.sh = *sh;
	return saveImage(&item);
}
int grEngine::saveImage (Image* item) {
	FILE *file = fopen(item->path, "wb");
	saveImageFD(item, file, 0);
	fclose(file);
	return true;
}
int grEngine::saveImageFD (Image* item, FILE* file, long offset) {
	fseek(file, offset, SEEK_SET);
	switch (item->type) {
		case GR_IMG_I_BMP:
			bmpHead bh;
			int null1;
			null1 = 0;
			bh.signature = 0x4D42;
			bh.null1 = 0;
			bh.null2 = 0;
			bh.offsetPixels = 54;
			bh.sizeHead = 40;
			bh.width = item->sh.width;
			bh.height = item->sh.height;
			bh.layers = 1;
			bh.compression = 0;
			bh.compressionSize = 0;
			bh.ppmWidth = 72;
			bh.ppmHeight = 72;
			bh.colorsUse = 0;
			bh.colorsSpecial = 0;
			printf("asdsad %i\n", item->sh.imgType);
			if (item->sh.imgType==GR_RGBA_32) {
				bh.bpp = 32;
				bh.sizeFile = 54+((bh.width*4+3)&~3)*bh.height;
				fwrite(&bh, sizeof(bmpHead), 1, file);
				for (int y=0; y<bh.height; y++) {
					for (int x=0; x<bh.width; x++) {
						fwrite(&((rgba*)(item->sh.img))[x+item->sh.texWidth*(item->sh.texHeight-y-1)].a, 1, 1, file);
						fwrite(&((rgba*)(item->sh.img))[x+item->sh.texWidth*(item->sh.texHeight-y-1)].b, 1, 1, file);
						fwrite(&((rgba*)(item->sh.img))[x+item->sh.texWidth*(item->sh.texHeight-y-1)].g, 1, 1, file);
						fwrite(&((rgba*)(item->sh.img))[x+item->sh.texWidth*(item->sh.texHeight-y-1)].r, 1, 1, file);
					}
					fwrite(&null1, ((bh.width*4+3)&~3)-bh.width*4, 1, file);
				}
			}else if (item->sh.imgType==GR_BGRA_32) {
				bh.bpp = 32;
				bh.sizeFile = 54+((bh.width*4+3)&~3)*bh.height;
				fwrite(&bh, sizeof(bmpHead), 1, file);
				for (int y=bh.height-1; y>=0; y--) {
					for (int x=0; x<bh.width; x++) {
						fwrite(&((rgba*)(item->sh.img))[x+item->sh.texWidth*y], 4, 1, file);
					}
					fwrite(&null1, ((bh.width*4+3)&~3)-bh.width*4, 1, file);
				}
			}else if (item->sh.imgType==GR_RGB_24) {
				printf("rgb24");
				bh.bpp = 24;
				bh.sizeFile = 54+((bh.width*3+3)&~3)*bh.height;
				fwrite(&bh, sizeof(bmpHead), 1, file);
				for (int y=0; y<bh.height; y++) {
					for (int x=0; x<bh.width; x++) {
						fwrite(&((rgb*)(item->sh.img))[x+item->sh.texWidth*(item->sh.texHeight-y-1)].b, 1, 1, file);
						fwrite(&((rgb*)(item->sh.img))[x+item->sh.texWidth*(item->sh.texHeight-y-1)].g, 1, 1, file);
						fwrite(&((rgb*)(item->sh.img))[x+item->sh.texWidth*(item->sh.texHeight-y-1)].r, 1, 1, file);
					}
					fwrite(&null1, ((bh.width*3+3)&~3)-bh.width*3, 1, file);
				}
			}else if (item->sh.imgType==GR_BGR_24) {
				printf("bgr24\n");
				bh.bpp = 24;
				bh.sizeFile = 54+((bh.width*3+3)&~3)*bh.height;
				fwrite(&bh, sizeof(bmpHead), 1, file);
				for (int y=bh.height-1; y>=0; y--) {
					for (int x=0; x<bh.width; x++) {
						fwrite(&((rgb*)(item->sh.img))[x+item->sh.texWidth*y], 3, 1, file);
					}
					fwrite(&null1, ((bh.width*3+3)&~3)-bh.width*3, 1, file);
				}
				printf("offset\\%i", ((bh.width*3+3)&~3)-bh.width*3);
			}
			return bh.sizeFile;
		case GR_IMG_I_TGA:
			break;
	}
	return 0;
}

std::vector<Shape*>* grEngine::loadLib (std::string str) {
	FILE *file = fopen(str.c_str(), "rb");
	std::string type = get_ext(str);
	std::vector<Shape*>* linc = loadLibFD(file);
	fclose(file);
	return linc;
}
std::vector<Shape*>* grEngine::loadLibFD (FILE *file) {
	std::vector<Shape*> *linc = (std::vector<Shape*>*)malloc(sizeof(std::vector<Shape*>));
	libHead lib;
	libImgHead *libImg = (libImgHead*)malloc( sizeof(libImgHead)*lib.sizeLib );
	char *name = (char*)malloc(lib.nameLength+1);
	char *desc = (char*)malloc(lib.descriptLength+1);
	name[lib.nameLength] = 0;
	desc[lib.descriptLength] = 0;
	fread(&lib, sizeof(libHead), 1, file);

	fread(name, lib.nameLength, 1, file);
	fread(desc, lib.descriptLength, 1, file);

	fread(libImg, sizeof(libImgHead)*lib.sizeLib, 1, file);
	printf("name=%s, desc=%s\n", name, desc);
	for (int i=0; i<lib.sizeLib; i++) {
		printf("%i type=%i size=%i ost=%i nL=%i \n", libImg[i].id, libImg[i].type, libImg[i].size, libImg[i].offset);
		linc->push_back(grEngine::loadImageFD(libImg[i].type, file, libImg[i].offset, libImg[i].size));
	}
	printf("Libe sign=%i, size=%i, items=%i, ver=%i", lib.signature, lib.sizeFile, lib.sizeLib, lib.ver);
	return linc;
}

int grEngine::saveLib (std::string str, std::vector<Shape*> shArr) {
	Image *arr = (Image*)malloc( sizeof(Image)* shArr.size());
	for (int i=0; i<shArr.size(); i++) {
		arr[i].sh = *(shArr[i]);
		arr[i].id = i;
		arr[i].type = GR_IMG_I_BMP;
		arr[i].path = "";
		arr[i].pathLength = 0;
	}
	saveLibImg(str, arr, shArr.size());
	return true;
}
int grEngine::saveLibFD (FILE *file, std::vector<Shape*> shArr, long offset) {
	Image *arr = (Image*)malloc( sizeof(Image)* shArr.size());
	for (int i=0; i<shArr.size(); i++) {
		arr[i].sh = *(shArr[i]);
		arr[i].id = i;
		arr[i].type = GR_IMG_I_BMP;
		arr[i].path = "";
		arr[i].pathLength = 0;
	}
	saveLibImgFD(file, arr, shArr.size(), offset);
	return true;
}
int grEngine::saveLibImg (std::string str, Image* arr, int length) {
	FILE *file = fopen(str.c_str(), "wb");
	std::string type = get_ext(str);
	saveLibImgFD(file, arr, length, 0 );
	fclose(file);
	return true;
}
int grEngine::saveLibImgFD (FILE *file, Image* shArr, int length, long start) {
	Image *img;
	libHead lhead;
	libImgHead ihead;
	long offset = 0;
	lhead.descriptLength = 0;
	lhead.nameLength = 0;
	lhead.signature = LIB_SIGN;
	lhead.ver = 1;
	lhead.sizeLib = length;
	for(int i=0; i<length; i++) {
		img = &shArr[i];
		ihead.id = i;
		ihead.type = img->type;
		ihead.offset = start+sizeof(libHead)+length*sizeof(libImgHead)+lhead.descriptLength+lhead.nameLength+offset;
		ihead.nameLength = 0;

		fseek(file, start+sizeof(libHead)+i*sizeof(libImgHead)+lhead.descriptLength+lhead.nameLength, SEEK_SET);
		fwrite(&ihead, sizeof(libImgHead), 1, file);

		offset += saveImageFD(img, file, start+sizeof(libHead)+length*sizeof(libImgHead)+lhead.descriptLength+lhead.nameLength+offset);

	}
	lhead.sizeFile = sizeof(libHead)+length*sizeof(libImgHead)+lhead.descriptLength+lhead.nameLength+offset-start;
	fseek(file, start, SEEK_SET);
	fwrite(&lhead, sizeof(libHead), 1, file);
	return true;
}
Shape* grEngine::newShape () {
	Shape* sh = (Shape*)malloc(sizeof(Shape));
	sh->width = 0;
	sh->height = 0;
	sh->libID = 0;
	sh->imgCache = 0;
	sh->img = 0;
	sh->x = 0;
	sh->y = 0;
	sh->type = GR_TYPE_NULL;
	return sh;
}
void grEngine::addShape(Shape* sh, Directory *dir) {
	sh->globalx = sh->x+dir->globalx;
	sh->globaly = sh->y+dir->globaly;
	dir->child.push_back( {GR_SHAPE, sh} );
}
*/
/*
void grEngine::Scene::redraw () {
	Texture *tex;

	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glEnable( GL_ALPHA_TEST );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	for (int i = 0; i<bufferLength; i++) {
		tex = this->buffer[i]->tex;
		this->buffer[i]->trace();
		switch (tex->type) {
			case GR_TYPE_BMP_TO_CACHE:// <editor-fold defaultstate="collapsed" desc="GR_TYPE_BMP_TO_CACHE">
				glGenTextures( 1, &tex->imgGL );
				if (tex->imgGL==0) {
					tex->type = GR_TYPE_CACHED_INVALID;
					goto errCache;
				}
				tex->type = GR_TYPE_CACHED;

				glBindTexture( GL_TEXTURE_2D, tex->imgGL );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
				if (tex->imgType==GR_RGBA_32) {
					glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tex->texWidth, tex->texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->img );
				}else if (tex->imgType==GR_BGRA_32) {
					glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tex->texWidth, tex->texHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, tex->img );
				}else if (tex->imgType==GR_RGB_24) {
					glPixelStorei( GL_PACK_ALIGNMENT, 1 );
					glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, tex->texWidth, tex->texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->img );
					printf("ERRORS %s\n", glGetError());
				}else if (tex->imgType==GR_BGR_24) {
					glPixelStorei( GL_PACK_ALIGNMENT, 1 );
					glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, tex->texWidth, tex->texHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, tex->img );
				}if (tex->imgType==GR_MONO_8) {
					glPixelStorei( GL_PACK_ALIGNMENT, 1 );
					glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, tex->texWidth, tex->texHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, tex->img );
				}
				printf("ERRORS %s\n", glGetError());
				glBegin( GL_QUADS );
					glTexCoord2f( 0.0, 0.0 );
					glVertex2i( this->buffer[i]->globalx, this->buffer[i]->globaly );
					glTexCoord2f( 0.0, 1.0 );
					glVertex2i( this->buffer[i]->globalx, this->buffer[i]->globaly+tex->texHeight );
					glTexCoord2f( 1.0, 1.0 );
					glVertex2i( this->buffer[i]->globalx+tex->texWidth, this->buffer[i]->globaly+tex->texHeight );
					glTexCoord2f( 1.0, 0.0 );
					glVertex2i( this->buffer[i]->globalx+tex->texWidth, this->buffer[i]->globaly );
				glEnd( );
				break;// </editor-fold>
			case GR_TYPE_CACHED:// <editor-fold defaultstate="collapsed" desc="GR_TYPE_CACHED">
				glBindTexture( GL_TEXTURE_2D, tex->imgGL );
				glBegin( GL_QUADS );
					glTexCoord2f( 0.0, 0.0 );
					glVertex2i( this->buffer[i]->globalx, this->buffer[i]->globaly );
					glTexCoord2f( 0.0, 1.0 );
					glVertex2i( this->buffer[i]->globalx, this->buffer[i]->globaly+tex->texHeight );
					glTexCoord2f( 1.0, 1.0 );
					glVertex2i( this->buffer[i]->globalx+tex->texWidth, this->buffer[i]->globaly+tex->texHeight );
					glTexCoord2f( 1.0, 0.0 );
					glVertex2i( this->buffer[i]->globalx+tex->texWidth, this->buffer[i]->globaly );
				glEnd( );
				break;// </editor-fold>
			case GR_TYPE_CACHED_INVALID:// <editor-fold defaultstate="collapsed" desc="GR_TYPE_CACHED">
			case GR_TYPE_BMP:
				errCache:
				glRasterPos2s(this->buffer[i]->globalx, this->buffer[i]->globaly);
				printf("%i %i\n", this->buffer[i]->globalx, this->buffer[i]->globaly);
				glPixelZoom(1,-1);
				if (tex->imgType==GR_RGBA_32) {
					glDrawPixels(tex->texWidth, tex->texHeight, GL_RGBA, GL_UNSIGNED_BYTE, tex->img);
				}else if (tex->imgType==GR_BGRA_32) {
					glDrawPixels(tex->texWidth, tex->texHeight, GL_BGRA, GL_UNSIGNED_BYTE, tex->img);
				}else if (tex->imgType==GR_RGB_24) {
					glDrawPixels(tex->texWidth, tex->texHeight, GL_RGB, GL_UNSIGNED_BYTE, tex->img);
				}else if (tex->imgType==GR_BGR_24) {
					glDrawPixels(tex->texWidth, tex->texHeight, GL_BGR, GL_UNSIGNED_BYTE, tex->img);
				}else if (tex->imgType==GR_MONO_8) {
					glDrawPixels(tex->texWidth, tex->texHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, tex->img);
				}else{
					printf("EEEEEERRRORRRRR");
				}
				break;// </editor-fold>
		}
	}
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glDisable( GL_ALPHA_TEST );
	glFlush( );
	glutSwapBuffers( );
}*/