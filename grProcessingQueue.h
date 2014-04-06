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

	#define THREAD_H HANDLE
	#define THREAD DWORD WINAPI
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
	
	#define THREAD_H pthread_t
	#define THREAD void*
#endif
using namespace std;
extern FILE *iovir;
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
	class SupSuspendTask {
	public:
		virtual int processExecute(HANDLE *sect) {
			fputs("SupSuspendTask::processExecute\n", iovir);
			return false;
		}
	};
	class ThreadTask {
	public:
		virtual int processExecute() {
			fputs("ThreadTask::processExecute\n", iovir);
			return false;
		}
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
	protected:
		queue<TTask*> _essentialTasks1;
		queue<TTask*> _essentialTasks2;
		char _queueIsUse;
		CRITICAL_SECTION _accessPush;
	public:
		ProcessingQueue() :_queueIsUse(0) {
			CRITICAL_SECTION_INIT(this->_accessPush);
		}
		virtual int addTask(TTask *task, int type) {fputs("ERROR 2", stderr);return false;}
		virtual int performTasks() {fputs("ERROR 3", stderr);return false;}
	};
	template<class TTask> class ProcessingQueueTimeLimited {
		queue<TTask*> _essentialTasks1;
		queue<TTask*> _essentialTasks2;
		char _queueIsUse;
		CRITICAL_SECTION _accessPush;
	public:
		ProcessingQueueTimeLimited() :_queueIsUse(0) {
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
	class ProcessingEachFrame :public ProcessingQueue<EachFrameTask> {
	public:
		//ProcessingEachFrame() {}
		int addTask(EachFrameTask *task, int type) {
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
	class ProcessingSupSuspend :public ProcessingQueue<SupSuspendTask> {
		HANDLE _suspendHandle;
		THREAD_H _threadHandle;
		float _time;
	public:
		typedef struct {
			HANDLE *sush;
			SupSuspendTask *task;
		} ThreadData;
		ProcessingSupSuspend();
		
		void setTime(float time);
		float getTime();
		
		int addTask(SupSuspendTask *task, int type);
		int performTasks();
		
		static THREAD threadFunction (void* dataArg);
	};
	class ProcessingThread :public ProcessingQueue<ThreadTask> {
	private:
		//ThreadData *threads;
	public:
		typedef struct {
			HANDLE *sush;
			SupSuspendTask *task;
		} ThreadData;
		ProcessingThread();
		int addTask(ThreadTask *task, int type);
		int performTasks();
	};
};
#endif	/* GRPROCESSINGQUEUE_H */

