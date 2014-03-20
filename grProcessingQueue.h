#ifndef GRPROCESSINGQUEUE_H
#define	GRPROCESSINGQUEUE_H
#include "grBaseTypes.h"

using namespace std;

namespace Graphonichk {
	class ProcessingTask {
	public:
		int processExecute();
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
	//template class ProcessingQueue<VBOUpdateTask>;
	// class ProcessingQueue<VBOUpdateTask>;
	template<class TTask> class ProcessingQueue {
		queue<TTask> essentialTasks;
		CRITICAL_SECTION accessPush;
	public:
		ProcessingQueue() {
			InitializeCriticalSection(&this->accessPush);
		}
		int addTask(TTask task, int type) {
			//int size = 0;
			EnterCriticalSection(&this->accessPush);
			this->essentialTasks.push(task);
			//size = this->essentialTasks.size();
			LeaveCriticalSection(&this->accessPush);
			return true;
		}
		int performTasks() {
			EnterCriticalSection(&this->accessPush);
			printf("performTasks %i\n", this->essentialTasks.size());
			while ( !this->essentialTasks.empty() ) {
				this->essentialTasks.front().processExecute();
				this->essentialTasks.pop();
			}
			LeaveCriticalSection(&this->accessPush);
			return true;
		}
	};
	
};
#endif	/* GRPROCESSINGQUEUE_H */

