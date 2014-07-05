#include "grMain.h"
#include "grFile.h"

using namespace std;
using namespace Graphonichk;


#if defined(WIN32)
	FileLoad::FileLoad(const string path) {
		this->progres = 0.0;
		this->fileHandle = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		if (this->fileHandle == INVALID_HANDLE_VALUE) {
			printf("Could not open file (error %d)\n", GetLastError());
			this->status = STATUS::FILE_NOT_FOUND;
			this->size = 0;
			this->path = "";
			this->name = "";
		}else{
			this->status = STATUS::STOP;
			this->ovl.Offset = 0;
			this->ovl.OffsetHigh = 0;
			this->ovl.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			if (this->ovl.hEvent == NULL) GetLastError();
			this->size = GetFileSize(this->fileHandle, NULL);
			this->path = path;
			//this->name = get_name(path);
			ProcessingThread::addThreadTask(new FileLoadThreadTask(this));
		}
		printf("<FileLoad size='%i' path='%s' fileHandle='%i' />\n", this->size, this->path.c_str(), this->fileHandle);
	}
	int FileLoadThreadTask::processExecute() {
		EventFileLoad *eventFile = nullptr;
		switch (file->status) {
			case FileLoad::STOP:// <editor-fold defaultstate="collapsed" desc="FileLoad::STOP">
				file->data = (uint8_t*)malloc(file->size);
				if ( file->data != nullptr ) {
					file->status = FileLoad::PROCESS;
					if (!ReadFile( file->fileHandle, file->data, file->size, NULL, &(file->ovl) )) {
						switch(GetLastError()) {
							case ERROR_IO_PENDING:
								file->status = FileLoad::PROCESS;
								break;
							case ERROR_HANDLE_EOF:
								file->status = FileLoad::SUCCESS;
								file->progres = file->size;
								CloseHandle(file->fileHandle);
								CloseHandle(file->ovl.hEvent);
								break;
							default:
								file->status = FileLoad::READ_FILE_ERROR;
								file->progres = 0;
								free(file->data);
								CloseHandle(file->fileHandle);
								CloseHandle(file->ovl.hEvent);
								break;
						}
					}else{
						//printf("readFile %i\n", GetLastError());
					}
				}
				break;// </editor-fold>
			case FileLoad::PROCESS:// <editor-fold defaultstate="collapsed" desc="FileLoad::PROCESS">
				GetOverlappedResult(file->fileHandle, &(file->ovl), &(file->progres), false);
				switch (WaitForSingleObject(file->ovl.hEvent, 0)) {
					case WAIT_ABANDONED:
						file->status = FileLoad::READ_FILE_ERROR;
						file->progres = 0;
						free(file->data);
						CloseHandle(file->fileHandle);
						CloseHandle(file->ovl.hEvent);
						printf("WAIT_ABANDONED\n");
						break;
					case WAIT_OBJECT_0:
						printf("<FileLoad size='%fKB'/>\n", (float)file->progres/1024.0f);
						file->status = FileLoad::SUCCESS;
						eventFile = new EventFileLoad( EventFileLoad::FILE_SUCCESS, file );
						file->callEvent(eventFile);
						delete eventFile;
						return true;
						break;
					case WAIT_TIMEOUT:
						printf("<FileLoad size='%fKB'/>\n", (float)file->progres/1024.0f);
						break;
					case WAIT_FAILED:
						file->status = FileLoad::READ_FILE_ERROR;
						file->progres = 0;
						free(file->data);
						CloseHandle(file->fileHandle);
						CloseHandle(file->ovl.hEvent);
						puts("WAIT_FAILED\n");
						break;
				}
				break;// </editor-fold>
		}
		return false;
	}
#else
	FileLoad::FileLoad(const string path) {
		this->progres = 0.0;
		this->fileHandle = fopen(path.c_str(), "rb");
		if (this->fileHandle == nullptr) {
			perror("Could not open file");
			this->status = STATUS::FILE_NOT_FOUND;
			this->size = 0;
			this->path = "";
			this->name = "";
		}else{
			this->status = STATUS::STOP;
			this->path = path;
			//this->name = get_name(path);
			ProcessingThread::addThreadTask(new FileLoadThreadTask(this));
		}
		printf("<FileLoad size='%i' path='%s' fileHandle='%i' />\n", this->size, this->path.c_str(), this->fileHandle);
	}
	int FileLoadThreadTask::processExecute() {
		if (file->fileHandle == nullptr) return false;
		printf("file %i %s\n", file->fileHandle, file->path.c_str());
		
		fseek(file->fileHandle, 0, SEEK_END);
		file->size = ftell(file->fileHandle);
		file->data = (uint8_t*)malloc(file->size);
		fseek(file->fileHandle, 0, SEEK_SET);
		fread(file->data, file->size, 1, file->fileHandle);
		fclose(file->fileHandle);
		
		EventFileLoad *eventFile = new EventFileLoad( EventFileLoad::FILE_SUCCESS, file );
		file->callEvent(eventFile);
		delete eventFile;
		return true;
	}
#endif