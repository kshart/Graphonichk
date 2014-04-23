#ifndef GRFILE_H
#define	GRFILE_H

#include "grBaseTypes.h"
using namespace std;

namespace Graphonichk {
	class FileLoad;
	class EventFileLoad;
	//template<class TEvent> class EventDispatcher;//handler
	template class EventDispatcher<EventFileLoad>;
	class EventFileLoad :public Event {
	  public:
		enum :int{
			FILE_SUCCESS,
			FILE_ERROR,
			FILE_PROCESS
		};
		EventFileLoad(int type, FileLoad *file);
		FileLoad *file;
	};
	class FileLoad:public EventDispatcher<EventFileLoad> {
	private:
		#ifdef WIN32
		HANDLE fileHandle;
		OVERLAPPED  ovl;
		static DWORD WINAPI loaderThread(void*);
		#endif
	public:
		FileLoad(string path);
		~FileLoad();
		enum STATUS:char {
			SUCCESS,
			PROCESS,
			STOP,
			FILE_NOT_FOUND,
			READ_FILE_ERROR
		};
		//void trace();
		
		STATUS status;
		unsigned long int size, progres;
		void *data;
		string name, path;
		static vector<FileLoad*> buffer;
		static void init();
		static void trace();
	};
}

#endif	/* GRFILE_H */

