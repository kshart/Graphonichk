#ifndef GRPROCESSINGQUEUE_H
#define	GRPROCESSINGQUEUE_H
#include <stack>
#include <queue>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "grMain.h"
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
		virtual int processExecute(MUTEX *sect) {
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
		virtual int addTask(TTask *task, int type=0) {
			CRITICAL_SECTION_INTER(this->_accessPush);
			if (this->_queueIsUse == 1) {
				this->_essentialTasks2.push(task);
			}else{
				this->_essentialTasks1.push(task);
			}
			CRITICAL_SECTION_LEAVE(this->_accessPush);
			return true;
		}
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
					if (  this->_essentialTasks1.front()->processExecute()  ) {
						delete this->_essentialTasks1.front();
					}else{
						this->addTask(this->_essentialTasks1.front());
					}
					this->_essentialTasks1.pop();
				}
			}else{
				//printf("performTasks %i\n", this->essentialTasks2.size());
				while ( !this->_essentialTasks2.empty() ) {
					if (  this->_essentialTasks2.front()->processExecute()  ) {
						delete this->_essentialTasks2.front();
					}else{
						this->addTask(this->_essentialTasks2.front());
					}
					this->_essentialTasks2.pop();
				}
			}
			return true;
		}
	};
	class ProcessingEachFrame :public ProcessingQueue<EachFrameTask> {
	public:
		//ProcessingEachFrame() {}
		int performTasks() {
			CRITICAL_SECTION_INTER(this->_accessPush);
			this->_queueIsUse = !this->_queueIsUse;
			CRITICAL_SECTION_LEAVE(this->_accessPush);
			if (this->_queueIsUse == 1) {
				//printf("performTasks %i\n", this->essentialTasks1.size());
				while ( !this->_essentialTasks1.empty() ) {
					if (  this->_essentialTasks1.front()->processExecute()  ) {
						delete this->_essentialTasks1.front();
					}else{
						this->addTask(this->_essentialTasks1.front());
					}
					this->_essentialTasks1.pop();
				}
			}else{
				//printf("performTasks %i\n", this->essentialTasks2.size());
				while ( !this->_essentialTasks2.empty() ) {
					if (  this->_essentialTasks2.front()->processExecute()  ) {
						delete this->_essentialTasks2.front();
					}else{
						this->addTask(this->_essentialTasks2.front());
					}
					this->_essentialTasks2.pop();
				}
			}
			return true;
		}
	};
	class ProcessingSupSuspend :public ProcessingQueue<SupSuspendTask> {
		MUTEX _suspendHandle;
		THREAD_H _threadHandle;
		float _time;
	public:
		typedef struct {
			MUTEX *sush;
			SupSuspendTask *task;
		} ThreadData;
		ProcessingSupSuspend();
		
		void setTime(float time);
		float getTime();
		
		int performTasks();
		
		static THREAD threadFunction (void* dataArg);
	};
	class ProcessingThread :public ProcessingQueue<ThreadTask> {
	private:
		static Array<ProcessingThread*> threads;
		static THREAD threadProcess(void* sys);
		
		THREAD_H thread;
	public:
		static void init();
		ProcessingThread();
		~ProcessingThread();
		int performTasks();
	};
};
#endif	/* GRPROCESSINGQUEUE_H */

