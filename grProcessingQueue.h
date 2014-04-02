#ifndef GRPROCESSINGQUEUE_H
#define	GRPROCESSINGQUEUE_H
#include <stack>
#include <queue>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <windows.h>
#include <windowsx.h>
using namespace std;

namespace Graphonichk {
	class Image;
	class Texture;
	
	class ProcessingTask {
	public:
		int processExecute();
		int info;
	};
	class EachFrameTask {
	public:
		EachFrameTask();
		virtual int processExecute();
		int info;
	};
	class TextureToUpdateTask :public EachFrameTask {
	public:
		TextureToUpdateTask(Texture *t);
		int processExecute();
		Texture *tex;
	};
	class TextureToDeleteTask :public EachFrameTask {
	public:
		TextureToDeleteTask(GLuint GLID);
		int processExecute();
		GLuint GLID;
	};
	
	class HeapFreeTask {
	public:
		int processExecute();
		void* data;
	};
	class VBOUpdateTask {
	public:
		int processExecute();
		GLuint bufferID;
		void* data;
		size_t dataSize;
	};
	class Model3D;
	class Model3DLoadTask :public EachFrameTask {
	public:
		int processExecute();
		Model3D *model;
	};
	
	
	template<class TTask> class ProcessingQueue {
		queue<TTask*> essentialTasks1;
		queue<TTask*> essentialTasks2;
		char queueIsUse;
		CRITICAL_SECTION accessPush;
	public:
		ProcessingQueue(){
			this->queueIsUse = 0;
			InitializeCriticalSection(&this->accessPush);
		}
		int addTask(TTask *task, int type) {
			EnterCriticalSection(&this->accessPush);
			if (this->queueIsUse == 1) {
				this->essentialTasks2.push(task);
			}else{
				this->essentialTasks1.push(task);
			}
			LeaveCriticalSection(&this->accessPush);
			return true;
		}
		int performTasks() {
			EnterCriticalSection(&this->accessPush);
			this->queueIsUse = !this->queueIsUse;
			LeaveCriticalSection(&this->accessPush);
			if (this->queueIsUse == 1) {
				//printf("performTasks %i\n", this->essentialTasks1.size());
				while ( !this->essentialTasks1.empty() ) {
					this->essentialTasks1.front()->processExecute();
					delete this->essentialTasks1.front();
					this->essentialTasks1.pop();
				}
			}else{
				//printf("performTasks %i\n", this->essentialTasks2.size());
				while ( !this->essentialTasks2.empty() ) {
					this->essentialTasks2.front()->processExecute();
					delete this->essentialTasks2.front();
					this->essentialTasks2.pop();
				}
			}
			
			
			return true;
		}
	};
	
	template<class TTask> class ProcessingQueueTimeLimited {
		queue<TTask*> essentialTasks1;
		queue<TTask*> essentialTasks2;
		char queueIsUse;
		CRITICAL_SECTION accessPush;
	public:
		ProcessingQueueTimeLimited(){
			this->queueIsUse = 0;
			InitializeCriticalSection(&this->accessPush);
		}
		int addTask(TTask *task, int type) {
			EnterCriticalSection(&this->accessPush);
			if (this->queueIsUse == 1) {
				this->essentialTasks2.push(task);
			}else{
				this->essentialTasks1.push(task);
			}
			LeaveCriticalSection(&this->accessPush);
			return true;
		}
		int performTasks() {
			EnterCriticalSection(&this->accessPush);
			this->queueIsUse = !this->queueIsUse;
			LeaveCriticalSection(&this->accessPush);
			if (this->queueIsUse == 1) {
				//printf("performTasks %i\n", this->essentialTasks1.size());
				while ( !this->essentialTasks1.empty() ) {
					this->essentialTasks1.front()->processExecute();
					delete this->essentialTasks1.front();
					this->essentialTasks1.pop();
				}
			}else{
				//printf("performTasks %i\n", this->essentialTasks2.size());
				while ( !this->essentialTasks2.empty() ) {
					this->essentialTasks2.front()->processExecute();
					delete this->essentialTasks2.front();
					this->essentialTasks2.pop();
				}
			}
			
			
			return true;
		}
	};
};
#endif	/* GRPROCESSINGQUEUE_H */

