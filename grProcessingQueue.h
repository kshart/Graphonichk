#ifndef GRPROCESSINGQUEUE_H
#define	GRPROCESSINGQUEUE_H
#include <stack>
#include <queue>
#include <pthread.h>
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
		virtual int processExecute(pthread_mutex_t *sect) {
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
		VBOUpdateTask(GLuint vbo, void* data, size_t size);
		int processExecute();
		GLuint bufferID;
		void* data;
		size_t dataSize;
	};
	class Model3D;
	class Model3DLoadTask :public EachFrameTask {
	public:
		Model3DLoadTask(Model3D *model) :model(model) {};
		int processExecute();
		Model3D *model;
	};
	
	template<class TTask> class ProcessingQueue {
	protected:
		queue<TTask*> _essentialTasks1;
		queue<TTask*> _essentialTasks2;
		char _queueIsUse;
		pthread_mutex_t _accessPush;
	public:
		ProcessingQueue() :_queueIsUse(0) {
			this->_accessPush = PTHREAD_MUTEX_INITIALIZER;
		}
		~ProcessingQueue() {
			pthread_mutex_destroy(&this->_accessPush);
		}
		virtual int addTask(TTask *task, int type=0) {
			pthread_mutex_lock(&this->_accessPush);
			if (this->_queueIsUse == 1) {
				this->_essentialTasks2.push(task);
			}else{
				this->_essentialTasks1.push(task);
			}
			pthread_mutex_unlock(&this->_accessPush);
			return true;
		}
		virtual int performTasks() {fputs("ERROR 3", stderr);return false;}
	};
	template<class TTask> class ProcessingQueueTimeLimited {
		queue<TTask*> _essentialTasks1;
		queue<TTask*> _essentialTasks2;
		char _queueIsUse;
		pthread_mutex_t _accessPush;
	public:
		ProcessingQueueTimeLimited() :_queueIsUse(0) {
			this->_accessPush = PTHREAD_MUTEX_INITIALIZER;
		}
		~ProcessingQueueTimeLimited() {
			pthread_mutex_destroy(&this->_accessPush);
		}
		int addTask(TTask *task, int type) {
			pthread_mutex_lock(&this->_accessPush);
			if (this->_queueIsUse == 1) {
				this->_essentialTasks2.push(task);
			}else{
				this->_essentialTasks1.push(task);
			}
			pthread_mutex_unlock(&this->_accessPush);
			return true;
		}
		int performTasks() {
			pthread_mutex_lock(&this->_accessPush);
			this->_queueIsUse = !this->_queueIsUse;
			pthread_mutex_unlock(&this->_accessPush);
			if (this->_queueIsUse == 1) {
				while ( !this->_essentialTasks1.empty() ) {
					if (  this->_essentialTasks1.front()->processExecute()  ) {
						delete this->_essentialTasks1.front();
					}else{
						this->addTask(this->_essentialTasks1.front());
					}
					this->_essentialTasks1.pop();
				}
			}else{
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
			pthread_mutex_lock(&this->_accessPush);
			this->_queueIsUse = !this->_queueIsUse;
			pthread_mutex_unlock(&this->_accessPush);
			if (this->_queueIsUse == 1) {
				while ( !this->_essentialTasks1.empty() ) {
					if (  this->_essentialTasks1.front()->processExecute()  ) {
						delete this->_essentialTasks1.front();
					}else{
						this->addTask(this->_essentialTasks1.front());
					}
					this->_essentialTasks1.pop();
				}
			}else{
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
		pthread_mutex_t _suspendHandle;
		pthread_t _threadHandle;
		float _time;
	public:
		typedef struct {
			pthread_mutex_t *sush;
			SupSuspendTask *task;
		} ThreadData;
		ProcessingSupSuspend() :_threadHandle(nullptr) {
			this->_suspendHandle = PTHREAD_MUTEX_INITIALIZER;
		}
		~ProcessingSupSuspend() {
			pthread_mutex_destroy(&this->_accessPush);
		}
		
		void setTime(float time);
		float getTime();
		
		int performTasks();
		
		static void* threadFunction (void* dataArg);
	};
	class ProcessingThread :private ProcessingQueue<ThreadTask> {
	private:
		ProcessingThread();
		~ProcessingThread();
		int performTasks();
		static Array<ProcessingThread*> threads;
		static void* threadProcess(void* sys);
		pthread_t thread;
	public:
		static void init();
		static int addThreadTask(ThreadTask *task, int type=0) {
			ProcessingThread::threads[0]->addTask(task);
			return true;
		}
	};
};
#endif	/* GRPROCESSINGQUEUE_H */

