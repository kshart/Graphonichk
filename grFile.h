/* 
 * File:   grFile.h
 * Author: Артем
 *
 * Created on 20 Декабрь 2013 г., 21:49
 */

#ifndef GRFILE_H
#define	GRFILE_H

#include <vector>
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
		EventFileLoad();
		FileLoad *file;
	};
	class FileLoad:public EventDispatcher<EventFileLoad> {
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
		HANDLE fileHandle;
		OVERLAPPED  ovl;
		
		static vector<FileLoad*> buffer;
		static void init();
		static DWORD WINAPI loaderThread(void*);
		static void trace();
	};
}

#endif	/* GRFILE_H */

