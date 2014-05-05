#include <string>
#include "grMain.h"
#include "grFile.h"

using namespace std;
using namespace Graphonichk;


vector<FileLoad*> FileLoad::buffer;


#ifdef WIN32
DWORD WINAPI FileLoad::loaderThread (void* sys) {
	FileLoad *fl = nullptr;
	EventFileLoad *eventFile = nullptr;
	ReleaseMutex(*(HANDLE*)sys);
	while(true) {
		for(int i=0; i<FileLoad::buffer.size(); i++) {
			fl = FileLoad::buffer[i];
			switch (fl->status) {
				case FileLoad::STOP:// <editor-fold defaultstate="collapsed" desc="FileLoad::STOP">
					//printf("FileLoad::STOP\n");
					fl->data = malloc(fl->size);
					if ( fl->data != NULL ) {
						fl->status = FileLoad::PROCESS;
						if (!ReadFile( fl->fileHandle, fl->data, fl->size, NULL, &(fl->ovl) )) {
							switch(GetLastError()) {
								case ERROR_IO_PENDING:
									fl->status = FileLoad::PROCESS;
									//printf("Read file pending.\n");
									break;
								case ERROR_HANDLE_EOF:
									fl->status = FileLoad::SUCCESS;
									fl->progres = fl->size;
									//printf("End of the file. The file is read.\n");
									CloseHandle(fl->fileHandle);
									CloseHandle(fl->ovl.hEvent);
									break;
								default:
									fl->status = FileLoad::READ_FILE_ERROR;
									fl->progres = 0;
									free(fl->data);
									//printf("Read file failed.\n");
									CloseHandle(fl->fileHandle);
									CloseHandle(fl->ovl.hEvent);
									break;
							}
						}else{
							//printf("readFile %i\n", GetLastError());
						}
					}
					break;// </editor-fold>
				case FileLoad::PROCESS:// <editor-fold defaultstate="collapsed" desc="FileLoad::PROCESS">
					GetOverlappedResult(fl->fileHandle, &(fl->ovl), &(fl->progres), false);
					switch (WaitForSingleObject(fl->ovl.hEvent, 0)) {
						case WAIT_ABANDONED:
							fl->status = FileLoad::READ_FILE_ERROR;
							fl->progres = 0;
							free(fl->data);
							CloseHandle(fl->fileHandle);
							CloseHandle(fl->ovl.hEvent);
							printf("WAIT_ABANDONED\n");
							break;
						case WAIT_OBJECT_0:
							printf("<FileLoad size='%iKB'/>\n", fl->progres/1024);
							fl->status = STATUS::SUCCESS;
							FileLoad::buffer.erase(FileLoad::buffer.begin()+i);
							i--;
							eventFile = new EventFileLoad( EventFileLoad::FILE_SUCCESS, fl );
							fl->callEvent(eventFile);
							delete eventFile;
							break;
						case WAIT_TIMEOUT:
							printf("<FileLoad size='%iKB'/>\n", fl->progres/1024);
							break;
						case WAIT_FAILED:
							fl->status = FileLoad::READ_FILE_ERROR;
							fl->progres = 0;
							free(fl->data);
							CloseHandle(fl->fileHandle);
							CloseHandle(fl->ovl.hEvent);
							puts("WAIT_FAILED\n");
							break;
					}
					printf("FileLoad::PROCESS\n");
					break;// </editor-fold>
			}
			
		}
		Sleep(100);
	}
}

void FileLoad::init () {
	HANDLE mutex = CreateMutex(NULL, TRUE, NULL);
	THREAD_START(FileLoad::loaderThread, &mutex);
	WaitForSingleObject(mutex, INFINITE);
	CloseHandle(mutex);
}
FileLoad::FileLoad(const string path) {
	this->progres = 0.0;
	this->fileHandle = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (this->fileHandle == INVALID_HANDLE_VALUE) {
		printf("Could not open file (error %d)\n", GetLastError());
		this->status = STATUS::FILE_NOT_FOUND;
		this->size = 0;
		this->path = "";
		this->name = "";
		this->data = NULL;
	}else{
		this->status = STATUS::STOP;
		this->ovl.Offset = 0;
		this->ovl.OffsetHigh = 0;
		this->ovl.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (this->ovl.hEvent == NULL) GetLastError();
		this->size = GetFileSize(this->fileHandle, NULL);
		this->path = path;
		//this->name = get_name(path);
		this->data = NULL;
		FileLoad::buffer.push_back(this);
	}
	printf("<FileLoad size='%i' path='%s' fileHandle='%i' />\n", this->size, this->path.c_str(), this->fileHandle);
}
#endif