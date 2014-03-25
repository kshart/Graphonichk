#ifndef GRTEXTURE_H
#define	GRTEXTURE_H
#include "grBaseTypes.h"

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
			MONO_ALPHA_8,
			MONO_ALPHA_16,
			MONO_1,
			MONO_2,
			MONO_4,
			MONO_8,
			MONO_16
		};
		enum EXT:char {
			EXT_NC,
			EXT_TGA,
			ETX_BMP
		};
		Image(char *data, unsigned int size);
		Image(string path);
		int saveAsXML(FILE *str, unsigned short tab=0);
		
		string name;
		unsigned short width, height;
		STATUS status;
		void *raw;
		TYPE type;
		static void loaded(const EventFileLoad* e);
	};
	class Texture {
	private:
		static vector<Texture*> buffer;
		static uint toUpdate, toDelete;
	public:
		/* format = 0 Рё type = 0 Рё GLID = 0
		 */
		enum EVENT:char {
			NONE,
			TO_DELETE,
			TO_UPDATE,
			LOADED
		};
		~Texture();
		Texture(string path);
		Texture(Image *img);
		Texture(unsigned short w, unsigned short h, GLuint format, GLuint type);
		int saveAsXML(FILE *str, unsigned short tab=0);
		void trace();
		
		
		
		EVENT event;
		Image *img;
		unsigned short width, height;
		GLuint format, type, GLID;
		
		//static void addTexture();
		static void texturesUpdate();
	};
};

#endif	/* GRTEXTURE_H */

