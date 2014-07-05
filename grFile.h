#ifndef GRFILE_H
#define	GRFILE_H

#include "grMain.h"
using namespace std;

namespace Graphonichk {
	class EventFileLoad;
	class FileLoad;
	class FileLoadThreadTask;
	
	class EventFileLoad :public Event {
	  public:
		enum :int {
			FILE_SUCCESS,
			FILE_ERROR,
			FILE_PROCESS
		};
		EventFileLoad(int type, FileLoad *file);
		FileLoad *file;
	};
	class FileLoad:public EventDispatcher<EventFileLoad> {
	private:
		#if defined(WIN32)
			HANDLE fileHandle;
			OVERLAPPED  ovl;
		#else
			FILE *fileHandle;
		#endif
		friend FileLoadThreadTask;
	public:
		FileLoad(const string path);
		~FileLoad();
		enum STATUS:char {
			SUCCESS,
			PROCESS,
			STOP,
			FILE_NOT_FOUND,
			READ_FILE_ERROR
		};
		STATUS status;
		long unsigned int size, progres;
		uint8_t *data = nullptr;
		string name, path;
	};
	
	class FileResource {
	private:
		//void* type;
	public:
		//void* hash, size;
		size_t size;
		string name;
		
	};
	
	class FileLoadThreadTask :public ThreadTask {
	public:
		FileLoadThreadTask(FileLoad *file) :file(file) {};
		int processExecute();
		FileLoad *file;
	};
}

#endif	/* GRFILE_H */

