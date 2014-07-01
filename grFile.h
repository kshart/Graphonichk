#ifndef GRFILE_H
#define	GRFILE_H

#include "grMain.h"
using namespace std;

namespace Graphonichk {
	class FileLoad;
	class EventFileLoad;
	template class EventDispatcher<EventFileLoad>;
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
		static THREAD loaderThread(void*);
		#ifdef WIN32
		HANDLE fileHandle;
		OVERLAPPED  ovl;
		#endif
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
		unsigned long int size, progres;
		void *data;
		string name, path;
		static vector<FileLoad*> buffer;
		static void init();
		static void trace();
	};
	
	class FileResource {
	private:
		//void* type;
	public:
		//void* hash, size;
		string name;
		
	};
}

#endif	/* GRFILE_H */

