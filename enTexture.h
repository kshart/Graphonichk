#ifndef GRTEXTURE_H
#define	GRTEXTURE_H
#include "grMain.h"

#define ADD_TEXTURE_TO_UPDATE_BUFFER(tex) \
	Windows::window->eachFrame.addTask(new TextureToUpdateTask(tex));\
	//Texture::buffer.push_back(tex);
	//Texture::toUpdate++;
#define ADD_TEXTURE_TO_DELETE_BUFFER(glid) \
	Windows::window->eachFrame.addTask(new TextureToDeleteTask(glid));\
	//Texture::buffer.push_back(tex);
	//Texture::toDelete++;

using namespace std;
namespace Graphonichk {
	class Image {
	private:
		bool load(char *data, unsigned int size);
	public:
		enum STATUS:char {
			TO_DELETE,
			TO_LOAD,
			ERR_LOAD,
			LOADED
		};
		enum TYPE:char{
			NC,
			BGR_24,//8 bit for pixels
			BGR_48,//16 bit for pixels
			BGRA_32,//8 bit for pixels
			BGRA_64,//16 bit for pixels
			RGB_24,//8 bit for pixels
			RGB_48,//16 bit for pixels
			RGBA_32,//8 bit for pixels
			RGBA_64,//16 bit for pixels
			PALETTE_1,
			PALETTE_2,
			PALETTE_4,
			PALETTE_8,
			MONO_1,
			MONO_2,
			MONO_4,
			MONO_8,
			MONO_16,
			ALPHA_8,
			ALPHA_16,
		};
		enum EXT:char {
			EXT_NC,
			EXT_TGA,
			ETX_BMP
		};
		Image(unsigned short width, unsigned short height, TYPE type, char *raw);
		Image(char *data, unsigned int size);
		Image(string path);
		int convert(TYPE type);
		void copyPixels(unsigned short x, unsigned short y, unsigned short width, unsigned short height, char *raw);
		string name;
		unsigned short width, height;
		STATUS status;
		void *raw;
		TYPE type;
		static void loaded(const EventFileLoad* e);
	};
	class Texture {
	private:
		//static vector<Texture*> buffer;
		//static uint toUpdate, toDelete;
		bool _loadedInFile;
		Texture(unsigned short rectCount=0, usvec4 *rect=nullptr);
	public:
		enum EVENT:char {
			NONE,
			TO_DELETE,
			TO_UPDATE,
			LOADED
		};
		Texture(string path, unsigned short rectCount=0, usvec4 *rect=nullptr);
		Texture(Image *img, unsigned short rectCount=0, usvec4 *rect=nullptr);
		Texture(unsigned short w, unsigned short h, GLuint internalType, GLuint format, GLuint type, unsigned short rectCount=0, usvec4 *rect=nullptr);
		~Texture();
		
		Image *getImage();
		void trace();
		
		
		EVENT event;
		Image *img;
		Array<usvec4> rects;
		unsigned short width, height;
		GLuint format, type, internalType, GLID, rectGLID;
		
		static Texture* getTexture(unsigned short w, unsigned short h, GLuint format, GLuint internalType, GLuint type, unsigned short rectCount=0, usvec4 *rect=nullptr);
	};
	
	class TextureToUpdateTask :public EachFrameTask {
	private:
		Texture *_tex;
	public:
		TextureToUpdateTask(Texture *t);
		int processExecute();
	};
	class TextureToDeleteTask :public EachFrameTask {
	private:
		GLuint _GLID;
	public:
		TextureToDeleteTask(GLuint GLID);
		int processExecute();
	};
};

#endif	/* GRTEXTURE_H */

