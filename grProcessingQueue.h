#ifndef GRPROCESSINGQUEUE_H
#define	GRPROCESSINGQUEUE_H
#include <stack>
#include <queue>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

#if defined(WIN32)
	#include <windows.h>
	#include <windowsx.h>
	#include <GL/wglext.h>
	#include <GL/glext.h>

	//#define CRITICAL_SECTION 
	#define CRITICAL_SECTION_INIT(cs) InitializeCriticalSection(&cs);
	#define CRITICAL_SECTION_INTER(cs) EnterCriticalSection(&cs);
	#define CRITICAL_SECTION_LEAVE(cs) LeaveCriticalSection(&cs);
#elif defined(X11)
	#include<stdio.h>
	#include<stdlib.h>
	#include<X11/X.h>
	#include<X11/Xlib.h>
	#include<GL/glx.h>

	#define CRITICAL_SECTION pthread_mutex_t
	#define CRITICAL_SECTION_INIT(cs) cs = PTHREAD_MUTEX_INITIALIZER;
	#define CRITICAL_SECTION_INTER(cs) pthread_mutex_lock(&cs);
	#define CRITICAL_SECTION_LEAVE(cs) pthread_mutex_unlock(&cs);
#endif
using namespace std;

namespace Graphonichk {
	
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
	
//#ifdef WIN32
	template<class TTask> class ProcessingQueue {
		queue<TTask*> _essentialTasks1;
		queue<TTask*> _essentialTasks2;
		char _queueIsUse;
		CRITICAL_SECTION _accessPush;
	public:
		ProcessingQueue() :_queueIsUse(0) {
			CRITICAL_SECTION_INIT(this->_accessPush);
		}
		int addTask(TTask *task, int type) {
			CRITICAL_SECTION_INTER(this->_accessPush);
			if (this->_queueIsUse == 1) {
				this->_essentialTasks2.push(task);
			}else{
				this->_essentialTasks1.push(task);
			}
			CRITICAL_SECTION_LEAVE(this->_accessPush);
			return true;
		}
		int performTasks() {
			CRITICAL_SECTION_INTER(this->_accessPush);
			this->_queueIsUse = !this->_queueIsUse;
			CRITICAL_SECTION_LEAVE(this->_accessPush);
			if (this->_queueIsUse == 1) {
				//printf("performTasks %i\n", this->essentialTasks1.size());
				while ( !this->_essentialTasks1.empty() ) {
					this->_essentialTasks1.front()->processExecute();
					delete this->_essentialTasks1.front();
					this->_essentialTasks1.pop();
				}
			}else{
				//printf("performTasks %i\n", this->essentialTasks2.size());
				while ( !this->_essentialTasks2.empty() ) {
					this->_essentialTasks2.front()->processExecute();
					delete this->_essentialTasks2.front();
					this->_essentialTasks2.pop();
				}
			}
			return true;
		}
	};
	template<class TTask> class ProcessingQueueTimeLimited {
		queue<TTask*> _essentialTasks1;
		queue<TTask*> _essentialTasks2;
		char _queueIsUse;
		#if defined(WIN32)
			CRITICAL_SECTION _accessPush;
		#elif defined(X11)
			pthread_mutex_t _accessPush;
		#endif
	public:
		ProcessingQueueTimeLimited() :_queueIsUse(0) {
			#if defined(WIN32)
				InitializeCriticalSection(&this->_accessPush);
			#elif defined(X11)
				this->_accessPush = PTHREAD_MUTEX_INITIALIZER;//PTHREAD_RECURSIVE_MUTEX_INITIALIZER
			#endif
		}
		int addTask(TTask *task, int type) {
			#if defined(WIN32)
				EnterCriticalSection(&this->_accessPush);
			#elif defined(X11)
				pthread_mutex_lock(&this->_accessPush);
			#endif
			if (this->_queueIsUse == 1) {
				this->_essentialTasks2.push(task);
			}else{
				this->_essentialTasks1.push(task);
			}
			#if defined(WIN32)
				LeaveCriticalSection(&this->_accessPush);
			#elif defined(X11)
				pthread_mutex_unlock(&this->_accessPush);
			#endif
			return true;
		}
		int performTasks() {
			#if defined(WIN32)
				EnterCriticalSection(&this->_accessPush);
			#elif defined(X11)
				pthread_mutex_lock(&this->_accessPush);
			#endif
			this->_queueIsUse = !this->_queueIsUse;
			#if defined(WIN32)
				LeaveCriticalSection(&this->_accessPush);
			#elif defined(X11)
				pthread_mutex_unlock(&this->_accessPush);
			#endif
			if (this->_queueIsUse == 1) {
				//printf("performTasks %i\n", this->essentialTasks1.size());
				while ( !this->_essentialTasks1.empty() ) {
					this->_essentialTasks1.front()->processExecute();
					delete this->_essentialTasks1.front();
					this->_essentialTasks1.pop();
				}
			}else{
				//printf("performTasks %i\n", this->essentialTasks2.size());
				while ( !this->_essentialTasks2.empty() ) {
					this->_essentialTasks2.front()->processExecute();
					delete this->_essentialTasks2.front();
					this->_essentialTasks2.pop();
				}
			}
			return true;
		}
	};
//#else
	//#include <pthread.h>
//#endif
	
};
#endif	/* GRPROCESSINGQUEUE_H */

