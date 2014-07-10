#ifndef GRLIBRARY_H
#define	GRLIBRARY_H
//gzip

#include "grMain.h"
#include "lzlib.h"
using namespace std;

namespace Graphonichk {
	class FileLibrary;
	class FileLibraryTask {
	public:
		FileLibraryTask() {};
		virtual int processExecute() {};
		int info;
	};
	class EventFileLibrary;
	
	class FileLibrary :protected ProcessingQueue<FileLibraryTask>  {
		typedef struct __attribute__((packed)) {
			uint16_t	signature;//1f 8B
			uint8_t		compressionMethod;
			uint8_t		flags;
			uint32_t	modificationTime;
			uint8_t		extraFlags;
			uint8_t		operatingSystem;
			///
		} gzipHead;
		typedef struct __attribute__((packed)) {
			uint8_t		IDstring[4];//1f 8B
			uint8_t		version;
			uint8_t		dictionarySize;
			///Lzma stream
		} lzipHead;
		typedef union {
			uint8_t raw[512];
			struct __attribute__((packed)) {
				uint8_t	name[100];
				uint8_t	mode[8];
				uint8_t	uid[8];
				uint8_t	gid[8];
				uint8_t	size[12];
				uint8_t	mtime[12];
				uint8_t	chksum[8];
				uint8_t	typeflag;
				uint8_t	linkname[100];
				///
				uint8_t	magic[6];
				uint8_t	version[2];
				uint8_t	uname[32];
				uint8_t	gname[32];
				uint8_t	devmajor[8];
				uint8_t	devminor[8];
				uint8_t	prefix[155];
				//
			}; 
		} tarHead;
	public:
		class Resource {
		public:
			Resource(string name, string path, size_t size) :name(name), path(path), size(size) {};
			uint8_t *data = nullptr;
			string name, path;
			size_t size=0, dataLoad=0, chunkBegin=0, chunkCount=0;
			time_t modifyTime;
		};
		enum STATUS:char {
			ERROR_OUTMEM,
			ERROR_FILE_NOT_OPEN,
			UNABLE,
			BEGIN,
			HEAD_READ,
			FILE_LIST_READ,
			READY_WRITE
		};
		enum OPEN_OPTION:char {
			READ,
			READ_WRITE,
			READ_ALL_FILES,
			
		};
		FileLibrary(string filename, OPEN_OPTION option=OPEN_OPTION::READ_ALL_FILES, bool synchronize=true);
		bool flush();
		
		void addFile();
		void getFile();
		void deleteFIle();
		void renameFIle();
		void rewriteFIle();
		
		void addResource();
		Resource* getResource(string name, string type="");
		void deleteResource();
		void renameResource();
		void rewriteResource();
		
		time_t modificationTime;
		string libraryName;
		size_t filesCount = 0;
		size_t filesSize = 0;
		
	private:
		class ResourceDirectory {
			vector<Resource> recources;
			vector<ResourceDirectory> derictories;
		public:
			ResourceDirectory(string name) :name(name) {};
			Resource* addResouce(string path, size_t size);
			ResourceDirectory* addDirectory(string path);

			ResourceDirectory *getDirectory(string name);
			Resource *getResource(string name, bool tree=false);

			void trace();

			string name;
		};
		
		ResourceDirectory mainDirectory;
		
		OPEN_OPTION option;
		size_t *chunkBegin=nullptr, *chunkCount=nullptr;
		size_t fileSize, fileBufferApply=0, chunkSize, chunkBufferSize, chunkBufferFullness = 0, skipBlock = 0;
		uint8_t *chunkBuffer;
		Resource *lastRes = nullptr;
		
		STATUS status = STATUS::UNABLE;
		size_t readBlockHead(uint8_t *data, size_t *&chunkBegin, size_t *&chunkCount);//data = 512
		size_t readBlocks(uint8_t *data, size_t count);
		size_t readChunk(struct LZ_Decoder* decoder);
		size_t writeBuffer(struct LZ_Decoder* decoder, uint8_t *data, size_t size);
	
		pthread_t procHeader;
		
	protected:
		int performTasks();
		static void* FileLibraryProc(void*);
	};

	class EventFileLibrary :public Event {
	  public:
		enum :int {
			RESOURCE_LOAD
		};
		EventFileLibrary(int type, FileLibrary *lib, FileLibrary::Resource *res);
		FileLibrary *library;
		FileLibrary::Resource *res;
	};

}

#endif	/* GRLIBRARY_H */

