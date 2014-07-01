#ifndef GRLIBRARY_H
#define	GRLIBRARY_H
//gzip

#include "grMain.h"
using namespace std;

namespace GraphonichkFileLibrary {
	class Resource;
	class ResourceDirectory;
	typedef struct __attribute__((packed)) {
		uint16_t	signature;//1f 8B
		uint8_t		compressionMethod;
		uint8_t		flags;
		uint32_t	modificationTime;
		uint8_t		extraFlags;
		uint8_t		operatingSystem;
		///
	} gzipHead;
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
	
	class Resource {
	public:
		Resource(string name, string path, size_t size) :name(name), path(path), size(size) {};
		string name, path;
		size_t size;
	};
	class ResourceDirectory {
		vector<Resource> recources;
		vector<ResourceDirectory> derictories;
	public:
		ResourceDirectory(string name) :name(name) {};
		Resource* addResouce(string path, size_t size);
		ResourceDirectory* addDirectory(string path);
		
		ResourceDirectory *getDirectory(string name);
		Resource *getResource(string name);
		
		void trace();
		
		string name;
	};
}
namespace Graphonichk {
	class FileLibrary;
	
	class FileLibrary {
	public:
		enum STATUS:char {
			ERROR_OUTMEM,
			UNABLE,
			BEGIN,
			HEAD_READ,
			FILE_LIST_READ,
			READY_WRITE
		};
		enum OPEN_OPTION:char {
			ONLY_READ
		};
		FileLibrary(string filename, OPEN_OPTION option=OPEN_OPTION::ONLY_READ, bool synchronize=false);
		void addFile();
		void getFile();
		void deleteFIle();
		void renameFIle();
		void rewriteFIle();
		
		static THREAD FileLibraryProc(void*);
		
		time_t modificationTime;
		
		string libraryName;
		size_t filesCount;
		size_t filesSize;
		
		GraphonichkFileLibrary::ResourceDirectory mainDirectory;
	private:
		STATUS status = STATUS::UNABLE;
		size_t readBlock(uint8_t *data);//data = 512
	};
}

#endif	/* GRLIBRARY_H */

