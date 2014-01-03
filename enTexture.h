/* 
 * File:   enTexture.h
 * Author: Артем
 *
 * Created on 16 Ноябрь 2013 г., 19:56
 */

#ifndef ENTEXTURE_H
#define	ENTEXTURE_H
#include "grBaseTypes.h"
#include <vector>
#include <GL/gl.h>

using namespace std;
namespace grEngine {
	class Image {
	  public:
		void asd();
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
		unsigned short width, height;
		void *img;
		TYPE type;
	};
	class Texture {
	  private:
		~Texture();
	  public:
		enum STATUS:char {
			TO_DELETE,
			TO_LOAD,
			ERR_NO,
			ERR_GENTEX
		};
		/*если format = 0 и type = 0 и GLID = 0 и status = TO_LOAD
		 * то другие поля могут использоватся не по назначению
		 *
		 */
		Texture();
		Texture(string path);
		Texture(Image *img);
		Texture(unsigned short w, unsigned short h, GLuint format, GLuint type);
		void close();
		void trace();

		unsigned short width, height;
		double coordX, coordY;
		STATUS status;
		void *img;
		GLuint format, type, GLID;
		
		static void loaded(const EventFileLoad *e);
	};
};

#endif	/* ENTEXTURE_H */

