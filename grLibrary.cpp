#include "grMain.h"
#include "grLibrary.h"

#include <limits>
#include <cstddef>
using namespace Graphonichk;

FileLibrary::Resource* FileLibrary::ResourceDirectory::addResouce(string path, size_t size) {
	size_t startPos = 0;
	ResourceDirectory *dir = this;
	{
		ResourceDirectory *tempDir = nullptr;
		string dirName;
		for(size_t i=0; i<path.size(); i++) {
			if (path[i] == 0) {
				break;
			}else if (path[i] == '/') {
				dirName = path.substr(startPos, i-startPos);
				tempDir = dir->getDirectory( dirName );
				if (tempDir==nullptr) {
					dir->derictories.push_back(ResourceDirectory( dirName ));
					dir = &(dir->derictories[dir->derictories.size()-1]);
				}else{
					dir = tempDir;
				}
				startPos = i+1;
			} 
		}
	}
	string resName = path.substr(startPos);
	Resource* res = dir->getResource( resName );
	if (res==nullptr) {
		dir->recources.push_back(Resource( resName, path, size ));
		res = &(dir->recources[dir->recources.size()-1]);
	}else{
	}
	return res;
}
FileLibrary::ResourceDirectory* FileLibrary::ResourceDirectory::addDirectory(string path) {
	ResourceDirectory *dir = this;
	ResourceDirectory *tempDir = nullptr;
	size_t startPos = 0;
	string dirName;
	path.pop_back();
	for(size_t i=0; i<path.size(); i++) {
		if (path[i] == 0) {
			break;
		}else if (path[i] == '/') {
			dirName = path.substr(startPos, i-startPos);
			tempDir = dir->getDirectory( dirName );
			if (tempDir==nullptr || dirName.empty()) {
				dir->derictories.push_back(ResourceDirectory( dirName ));
				dir = &(dir->derictories[dir->derictories.size()-1]);
			}else{
				dir = tempDir;
			}
			startPos = i+1;
		} 
	}
	dirName = path.substr(startPos);
	tempDir = dir->getDirectory( dirName );
	if (tempDir==nullptr || dirName.empty()) {
		dir->derictories.push_back(ResourceDirectory( dirName ));
		dir = &(dir->derictories[dir->derictories.size()-1]);
	}else{
		dir = tempDir;
	}
	return dir;
}
FileLibrary::Resource* FileLibrary::ResourceDirectory::getResource(string resourceName, bool tree) {
	for(Resource& res : recources) {
		if (res.name.compare(resourceName) == 0) return &res;
	}
	if (tree) {
		for(ResourceDirectory& dir : derictories) {
			Resource* result = dir.getResource(resourceName, true);
			if (result != nullptr) return result;
		}
	}
	return nullptr;
}
FileLibrary::ResourceDirectory* FileLibrary::ResourceDirectory::getDirectory(string name) {
	for(size_t i=0; i<derictories.size(); i++) {
		if (derictories[i].name == name) return &derictories[i];
	}
	return nullptr;
}
void FileLibrary::ResourceDirectory::trace() {
	printf("<directory name='%s'>\n", this->name.c_str());
	for(auto res : recources) {
		printf("\t<recources name='%s' path='%s' size='%i' chunkBegin='%i' chunkCount='%i'/>\n", res.name.c_str(), res.path.c_str(), res.size, res.chunkBegin, res.chunkCount);
	}
	for(auto dir : derictories) {
		dir.trace();
	}
	puts("</directory>");
}


FileLibrary::FileLibrary(string filename, OPEN_OPTION option, bool synchronize) :mainDirectory(""), option(option), libraryName(filename) {
	if (synchronize) {
		FILE *file = fopen(filename.c_str(), "rb");
		if (file==nullptr) {
			if (option==OPEN_OPTION::READ_WRITE) {	this->status = STATUS::READY_WRITE;
			}else{	this->status = STATUS::ERROR_FILE_NOT_OPEN;}
			return;
		}
		struct LZ_Decoder* decoder = LZ_decompress_open();
		if( !decoder || LZ_decompress_errno( decoder ) != LZ_ok ) {
			puts("FileLibrary::STATUS::ERROR_OUTMEM");
			this->status = STATUS::ERROR_OUTMEM;
		}else{
			this->status = STATUS::BEGIN;
			this->status = STATUS::HEAD_READ;
			fseek(file, 0, SEEK_END);
			this->chunkSize = min(65536, LZ_decompress_write_size(decoder));
			this->chunkBufferSize = 512*8*8*8;
			this->chunkBuffer = (uint8_t*)malloc(chunkBufferSize);
			this->fileSize = ftell(file);
			this->fileBufferApply = 0;
			size_t readBufferSize = chunkSize*10, readBufferBytes = 0;
			uint8_t *fileBuffer = (uint8_t*)malloc(readBufferSize);
			while(fileBufferApply < fileSize) {
				fseek(file, fileBufferApply, SEEK_SET);
				if ( fileBufferApply+readBufferSize > fileSize ) {
					fread(fileBuffer, fileSize-fileBufferApply, 1, file);
					readBufferBytes = this->writeBuffer(decoder, fileBuffer, fileSize-fileBufferApply);
				}else{
					fread(fileBuffer, readBufferSize, 1, file);
					readBufferBytes = this->writeBuffer(decoder, fileBuffer, readBufferSize);
				}
				fileBufferApply += readBufferBytes;
			}
			free(fileBuffer);
			if (option==OPEN_OPTION::READ) {	this->status = STATUS::FILE_LIST_READ;
			}else{	this->status = STATUS::READY_WRITE;}
		}
		fclose(file);
		LZ_decompress_close( decoder );
	}
}
bool FileLibrary::flush() {
	FILE *file = fopen(libraryName.c_str(), "wb");
	printf("file %i %s\n", file, libraryName.c_str());
	if (file==nullptr) return false;
	
	printf("FileLibrary::flush %i %i %i %i\n", LZ_min_dictionary_size(), LZ_max_dictionary_size(), LZ_min_dictionary_bits(), LZ_max_dictionary_bits());
	struct LZ_Encoder* encoder = LZ_compress_open( LZ_min_dictionary_size(), LZ_min_match_len_limit (), 100*1024 );
	if( !encoder || LZ_compress_errno( encoder ) != LZ_ok ) {
		puts("FileLibrary::STATUS::ERROR_OUTMEM");
		//this->status = STATUS::ERROR_OUTMEM;
	}else{
		char *str = "This Is String!!";
		int res, ddd = 0x1F&LZ_min_dictionary_bits();
		//LZ_min_dictionary_size()- pow(2, ddd);
		printf("№0 %i %i\n", ddd, (int)pow(2, ddd));
		printf("№1 %i\n", LZ_compress_write ( encoder, (uint8_t*)str, strlen(str) ));
		printf("№2 %i\n", res=LZ_compress_read ( encoder, chunkBuffer, chunkBufferSize ));
		fwrite(chunkBuffer, res, 1, file);
		LZ_compress_finish(encoder);
		printf("№2 %i\n", res=LZ_compress_read ( encoder, chunkBuffer, chunkBufferSize ));
		fwrite(chunkBuffer, res, 1, file);
		printf("№2 %i\n", res=LZ_compress_read ( encoder, chunkBuffer, chunkBufferSize ));
		fwrite(chunkBuffer, res, 1, file);
	}
	LZ_compress_close(encoder);
	
	
	fclose(file);
}
int FileLibrary::performTasks() {
	//THREAD_START_H(, FileLibrary::FileLibraryProc, this);
	return true;
}
void* FileLibrary::FileLibraryProc(void *data) {
	FileLibrary *lib = (FileLibrary*)data;
	//lib->performTasks();
	pthread_mutex_lock(&lib->_accessPush);
	lib->_queueIsUse = !lib->_queueIsUse;
	pthread_mutex_unlock(&lib->_accessPush);
	if (lib->_queueIsUse == 1) {
		while ( !lib->_essentialTasks1.empty() ) {
			if (  lib->_essentialTasks1.front()->processExecute()  ) {
				delete lib->_essentialTasks1.front();
			}else{
				lib->addTask(lib->_essentialTasks1.front());
			}
			lib->_essentialTasks1.pop();
		}
	}else{
		while ( !lib->_essentialTasks2.empty() ) {
			if (  lib->_essentialTasks2.front()->processExecute()  ) {
				delete lib->_essentialTasks2.front();
			}else{
				lib->addTask(lib->_essentialTasks2.front());
			}
			lib->_essentialTasks2.pop();
		}
	}
}
FileLibrary::Resource* FileLibrary::getResource(string name, string type) {
	if (this->option == OPEN_OPTION::READ_ALL_FILES) {
		if (type.empty()) {
			return this->mainDirectory.getResource(name, true);
		}else{
			
		}
		
		
	}
	return nullptr;
}


size_t FileLibrary::writeBuffer(struct LZ_Decoder* decoder, uint8_t* data, size_t size) {
	size_t writeApplyBytes = 0;
	int decompressWriteRes = 0;
	while(writeApplyBytes < size) {
		if (writeApplyBytes+this->chunkSize > size) {
			decompressWriteRes = LZ_decompress_write( decoder, data+writeApplyBytes, size-writeApplyBytes );
		}else{
			decompressWriteRes = LZ_decompress_write( decoder, data+writeApplyBytes, this->chunkSize );
		}
		
		if (decompressWriteRes > 0) {
			writeApplyBytes += decompressWriteRes;
			if (this->fileBufferApply+writeApplyBytes >= this->fileSize) LZ_decompress_finished( decoder );
			this->readChunk(decoder);
		}else if (decompressWriteRes < 0) {
			printf("write error: %s\n", LZ_strerror(LZ_decompress_errno(decoder)) );
			break;
		}else{
			break;
		}
	}
	return writeApplyBytes;
}
size_t FileLibrary::readChunk(struct LZ_Decoder* decoder) {
	size_t chunkApplyBytes = 0;
	int chunkReadBytes = 0;
	while (true) {
		
		
		while(this->chunkBufferFullness < this->chunkBufferSize) {
			chunkReadBytes = LZ_decompress_read(decoder, this->chunkBuffer+this->chunkBufferFullness, this->chunkBufferSize-this->chunkBufferFullness);
			if (chunkReadBytes > 0) {
				chunkApplyBytes += chunkReadBytes;
				this->chunkBufferFullness += chunkReadBytes;
			}else{
				break;
			}
			if (LZ_decompress_finished( decoder ) == 1) break;
		}
		
		
		if (chunkReadBytes < 0) {
			printf("error: %s\n", LZ_strerror(LZ_decompress_errno(decoder)) );
			break;
		}
		
		size_t countBlock = this->chunkBufferFullness/512;
		if (countBlock > 0) this->readBlocks(this->chunkBuffer, countBlock);
		
		if (this->chunkBufferFullness > countBlock*512) {
			this->chunkBufferFullness -= countBlock*512;
			memmove(this->chunkBuffer, this->chunkBuffer+countBlock*512, this->chunkBufferFullness);
		}else{
			this->chunkBufferFullness = 0;
		}
		if( LZ_decompress_finished( decoder ) == 1 ) {
			puts("finish");
			break;
		}
		if( chunkReadBytes == 0 ) break;
	}
	
	
	return chunkApplyBytes;
}
size_t FileLibrary::readBlocks(uint8_t* data, size_t count) {
	//printf("readBlocks %i\n", count);
	for(size_t i=0; i<count; i++) {
		if (this->skipBlock == 0) {
			//if (this->chunkCount!=nullptr) (*this->chunkCount) = readChunkCount-(*this->chunkBegin)+1;
			//this->chunkBegin = nullptr;
			//this->chunkCount = nullptr;
			size_t skipBytesCount = this->readBlockHead(data+512*i, this->chunkBegin, this->chunkCount);
			//if (this->chunkBegin!=nullptr) (*this->chunkBegin) = readChunkCount;
			this->skipBlock = (size_t)ceil(skipBytesCount/512.0f);
		}else{
			this->skipBlock--;
			if (this->option == OPEN_OPTION::READ_ALL_FILES && this->lastRes!=nullptr && this->lastRes->dataLoad<this->lastRes->size) {
				if (lastRes->dataLoad+512 > lastRes->size) {
					memcpy(lastRes->data+lastRes->dataLoad, data+512*i, lastRes->size-lastRes->dataLoad);
					lastRes->dataLoad = lastRes->size;
				}else{
					memcpy(lastRes->data+lastRes->dataLoad, data+512*i, 512);
					lastRes->dataLoad += 512;
				}
				
				
			}
		}
	}
	
	return 0;
}
size_t FileLibrary::readBlockHead(uint8_t* data, size_t *&chunkBegin, size_t *&chunkCount) {
	Resource *res;
	tarHead *head = (tarHead*)data;
	if (head->name[0] == 0) return 0; 
	
	head->name[99] = 0;
	head->mode[7] = 0;
	head->uid[7] = 0;
	head->gid[7] = 0;
	head->size[11] = 0;
	head->mtime[11] = 0;
	string path((char*)head->name);
	size_t resourceSize = strtol((char*)head->size, nullptr, 8);
	this->lastRes = nullptr;
	printf("'%s' %i ", head->name, resourceSize);
	switch (head->typeflag) {
		case 0:
		case '0'://Regular file
			puts("Regular file");
			if (resourceSize == 0) break;
			res = this->mainDirectory.addResouce(path, resourceSize);
			if (this->option == OPEN_OPTION::READ_ALL_FILES) res->data = (uint8_t*)malloc(resourceSize);
			chunkBegin = &res->chunkBegin;
			chunkCount = &res->chunkCount;
			this->filesCount++;
			this->filesSize += resourceSize;
			this->lastRes = res;
			break;
		case '1'://Link to another file already archived
			puts("Link to another file already archived");
			break;
		case '2'://Symbolic link
			puts("Symbolic link");
			break;
		case '3'://Character special device
			puts("Character special device");
			break;
		case '4'://Block special device
			puts("Block special device");
			break;
		case '5'://Directory
			puts("Directory");
			this->mainDirectory.addDirectory(path);
			break;
		case '6'://FIFO special file
			puts("FIFO special file");
			break;
		case '7'://Reserved
			puts("Reserved");
			break;
		case 'A'://File Description
			puts("File Description");
			break;
		case 'B'://File Description
			puts("File end");
			return numeric_limits<size_t>::max();
			break;
		default://File Description
			puts("Unvaliable typeflag");
			break;
	}
	
	return resourceSize;
}
