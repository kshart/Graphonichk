#include "grBaseTypes.h"
#include "grProcessingQueue.h"

using namespace Graphonichk;
EachFrameTask::EachFrameTask() :info(0) {
	
}
int EachFrameTask::processExecute() {
	fprintf(stderr, "call int EachFrameTask::processExecute(%i)", this->info);
	return false;
}
TextureToUpdateTask::TextureToUpdateTask(Texture *t) :tex(t) {
	
}
int TextureToUpdateTask::processExecute() {
	if (tex->event!=Texture::TO_UPDATE) {
		TextureToUpdateTask *task = new TextureToUpdateTask(this->tex);
		Windows::window->eachFrame.addTask(task, 0);
		return false;
	}
	if (tex->img==NULL) {// <editor-fold defaultstate="collapsed" desc="tex->img==NULL">
		if (tex->GLID==0) {
			glGenTextures( 1, &tex->GLID );
			if (tex->GLID==0) {
				TextureToUpdateTask *task = new TextureToUpdateTask(this->tex);
				Windows::window->eachFrame.addTask(task, 0);
				return false;
			}
			glBindTexture( GL_TEXTURE_2D, tex->GLID );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexImage2D( GL_TEXTURE_2D, 0, 0, tex->width, tex->height, 0, tex->format, tex->type, NULL );
		}else{
			glBindTexture( GL_TEXTURE_2D, tex->GLID );
			glTexImage2D( GL_TEXTURE_2D, 0, 0, tex->width, tex->height, 0, tex->format, tex->type, NULL );
		}
		tex->event = Texture::LOADED;//</editor-fold>
	}else{// <editor-fold defaultstate="collapsed">
		if (tex->img->status!=Image::LOADED) {
			TextureToUpdateTask *task = new TextureToUpdateTask(this->tex);
			Windows::window->eachFrame.addTask(task, 0);
			return false;
		}
		switch (tex->img->type) {
			case Image::RGBA_32:
				printf("\tImage::RGBA_32\n");
				tex->format = GL_RGBA;
				tex->type = GL_UNSIGNED_BYTE;
				break;
			case Image::BGRA_32:
				tex->format = GL_BGRA;
				tex->type = GL_UNSIGNED_BYTE;
				break;
			case Image::RGB_24:
				printf("\tImage::RGB_24\n");
				tex->format = GL_RGB;
				tex->type = GL_UNSIGNED_BYTE;
				break;
			case Image::BGR_24:
				printf("\tImage::BGR_24\n");
				tex->format = GL_BGR;
				tex->type = GL_UNSIGNED_BYTE;
				break;
			case Image::MONO_8:
				printf("\tImage::MONO_8\n");
				tex->format = GL_ALPHA;
				tex->type = GL_UNSIGNED_BYTE;
				break;
		}
		printf("\tImage %i\n", tex->img->raw);
		printf("\t//////\n");
		tex->width = tex->img->width;
		tex->height = tex->img->height;
		if (tex->GLID==0) {
			glGenTextures( 1, &tex->GLID );
			if (tex->GLID==0) {
				TextureToUpdateTask *task = new TextureToUpdateTask(this->tex);
				Windows::window->eachFrame.addTask(task, 0);
				return false;
			}
			glBindTexture( GL_TEXTURE_2D, tex->GLID );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			printf("TEXTURE %i", tex->img->raw);
			glTexImage2D( GL_TEXTURE_2D, 0, tex->format, tex->width, tex->height, 0, tex->format, tex->type, tex->img->raw );
		}else{
			fprintf(stderr, "GLID!=0!!!!!!!!!!!!!!");
			glBindTexture( GL_TEXTURE_2D, tex->GLID );
			glTexImage2D( GL_TEXTURE_2D, 0, tex->format, tex->width, tex->height, 0, tex->format, tex->type, tex->img->raw );
		}
		tex->event = Texture::LOADED;
	}//</editor-fold>
}
TextureToDeleteTask::TextureToDeleteTask(GLuint glid) :GLID(glid) {
	
}
int TextureToDeleteTask::processExecute() {
	glDeleteTextures(1, &this->GLID);
	return true;
}


int ProcessingTask::processExecute() {
	printf("%i \n", this->info);
	return true;
}
int HeapFreeTask::processExecute() {
	free(data);
	return true;
}
int VBOUpdateTask::processExecute() {
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->dataSize, this->data);
	return true;
}
