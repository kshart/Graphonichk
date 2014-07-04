#include "grMain.h"
#include "grLibrary.h"

#include <limits>
#include <cstddef>
using namespace Graphonichk;
using namespace GraphonichkFileLibrary;

Resource* ResourceDirectory::addResouce(string path, size_t size) {
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
					dir = &(dir->derictories[dir->derictories.size()-1]);\
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
ResourceDirectory* ResourceDirectory::addDirectory(string path) {
	ResourceDirectory *dir = this;
	ResourceDirectory *tempDir = nullptr;
	size_t startPos = 0;
	string dirName;
	for(size_t i=0; i<path.size(); i++) {
		if (path[i] == 0) {
			break;
		}else if (path[i] == '/') {
			dirName = path.substr(startPos, i-startPos);
			tempDir = dir->getDirectory( dirName );
			if (tempDir==nullptr) {
				dir->derictories.push_back(ResourceDirectory( dirName ));
				dir = &(dir->derictories[dir->derictories.size()-1]);\
			}else{
				dir = tempDir;
			}
			startPos = i+1;
		} 
	}
	dirName = path.substr(startPos);
	tempDir = dir->getDirectory( dirName );
	if (tempDir==nullptr) {
		dir->derictories.push_back(ResourceDirectory( dirName ));
		dir = &(dir->derictories[dir->derictories.size()-1]);\
	}else{
		dir = tempDir;
	}
	return dir;
}
Resource* ResourceDirectory::getResource(string name) {
	for(size_t i=0; i<recources.size(); i++) {
		if (recources[i].name == name) return &recources[i];
	}
	return nullptr;
}
ResourceDirectory* ResourceDirectory::getDirectory(string name) {
	for(size_t i=0; i<derictories.size(); i++) {
		if (derictories[i].name == name) return &derictories[i];
	}
	return nullptr;
}
void ResourceDirectory::trace() {
	printf("<directory name='%s'>\n", this->name.c_str());
	for(auto res : recources) {
		printf("\t<recources name='%s' path='%s' size='%i' chunkBegin='%i' chunkCount='%i'/>\n", res.name.c_str(), res.path.c_str(), res.size, res.chunkBegin, res.chunkCount);
	}
	for(auto dir : derictories) {
		dir.trace();
	}
	puts("</directory>");
}


FileLibrary::FileLibrary(string filename, OPEN_OPTION option, bool synchronize) :mainDirectory("") {
	/*gzipHead header;
	FILE *file = fopen(filename.c_str(), "rb");
	if (file==nullptr) {
		
	}else{
		this->status = STATUS::BEGIN;
		fread(&header, sizeof(header), 1, file);
		fclose(file);
	}
	if (header.signature!=0x8B1F) {
		printf("error\n");
	}
	this->modificationTime = header.modificationTime;
	
	this->status = STATUS::HEAD_READ;*/
	
	FILE *file = fopen("test.tar.lz", "rb");
	struct LZ_Decoder* decoder = LZ_decompress_open();
	if( !decoder || LZ_decompress_errno( decoder ) != LZ_ok ) {
		//Pp_show_msg( pp, "Not enough memory. Find a machine with more memory" );
		//retval = 1;
		puts("FileLibrary::STATUS::ERROR_OUTMEM");
		this->status = STATUS::ERROR_OUTMEM;
	}else{
		//retval = do_decompress( decoder, infd, pp, testing );
	}
	this->chunkSize = min(65536, LZ_decompress_write_size(decoder));
	this->chunkBufferSize = 512*10;
	this->chunkBuffer = (uint8_t*)malloc(chunkBufferSize);
	fseek(file, 0, SEEK_END);
	size_t readBufferSize = chunkSize*10,
			readBufferApply = 0,
			fileSize = ftell(file),
			fileBufferApply = 0;
	uint8_t *fileBuffer = (uint8_t*)malloc(readBufferSize);
	fseek(file, 0, SEEK_SET);
	printf("buffer size %i\n", chunkSize);
	while(fileBufferApply < fileSize) {
		if (fileBufferApply+readBufferSize > fileSize) {
			fread(fileBuffer, fileSize-fileBufferApply, 1, file);
			readBufferApply = this->readBuffer(decoder, fileBuffer, fileSize-fileBufferApply);
			
		}else{
			fread(fileBuffer, readBufferSize, 1, file);
			readBufferApply = this->readBuffer(decoder, fileBuffer, readBufferSize);
			
		}
		fileBufferApply += readBufferApply;
		
	}
	/*while(readBytesInFile < sizeFile) {
		printf("readChunkCount %i\n", readChunkCount);
		if (leaveWriteBytes > 0) {
			//memmove(buffer, buffer+(lengthReadBuffer-leaveWriteBytes), leaveWriteBytes);
			printf("leaveWriteBytes %i\n", leaveWriteBytes);
			fread(buffer+leaveWriteBytes, lengthReadBuffer-leaveWriteBytes, 1, file);
			
		}else{
			fread(buffer, lengthReadBuffer, 1, file);
		}
		readBufferBytes = ftell(file)-readBytesInFile;
		readBytesInFile = ftell(file);
		readChunkCount = readBytesInFile/lengthReadBuffer;
		
		
		lengthReadBuffer = readBufferBytes;
		int ii = LZ_decompress_write( decoder, buffer, lengthReadBuffer );
		leaveWriteBytes = lengthReadBuffer - ii;
		printf("ii %i\n", ii);
		if (sizeFile == readBytesInFile) LZ_decompress_finish( decoder );
		while ( true ) {
			if (blockReadBytes > 0) {
				readBytesRes = LZ_decompress_read(decoder, bufferRe+blockReadBytes, 512*countChunkRead-blockReadBytes);

			}else{
				readBytesRes = LZ_decompress_read(decoder, bufferRe, 512*countChunkRead);
			}
			
			if (readBytesRes < 0) {
				break;
			}else if (readBytesRes == 0) {
				break;
			}else if (LZ_decompress_finished( decoder ) == 1) {
				break;
			}
			
			blockReadBytes += readBytesRes;
			if (blockReadBytes >= 512*countChunkRead) {
				for(int block=0; block<countChunkRead; block++) {
					if (skipBlock > 0) {
						skipBlock--;
					}else{
						if (chunkCount!=nullptr) (*chunkCount) = readChunkCount-(*chunkBegin)+1;
						chunkBegin = nullptr;
						chunkCount = nullptr;
						size_t blockSize = this->readBlockHead(bufferRe+512*block, chunkBegin, chunkCount);
						if (chunkBegin!=nullptr) (*chunkBegin) = readChunkCount;
						skipBlock = (size_t)ceil(blockSize/512.0f);
					}
				}
				
				blockReadBytes = 0;
			}
			readBytes += readBytesRes;
		}
		
		
		if (readBytesRes < 0) {
			printf("error: %s\n", LZ_strerror(LZ_decompress_errno(decoder)) );
			break;
		}
		readPosition = LZ_decompress_member_position( decoder );
		if( LZ_decompress_finished( decoder ) == 1 ) break;
		
	}
	*/
	this->mainDirectory.trace();
	free(fileBuffer);
	fclose(file);
	LZ_decompress_close( decoder );
}

size_t FileLibrary::readBlockHead(uint8_t* data, size_t *&chunkBegin, size_t *&chunkCount) {
	Resource *res;
	tarHead *head = (tarHead*)data;
	head->name[99] = 0;
	head->mode[7] = 0;
	head->uid[7] = 0;
	head->gid[7] = 0;
	head->size[11] = 0;
	head->mtime[11] = 0;
	string path((char*)head->name);
	size_t resourceSize = strtol((char*)head->size, nullptr, 8);
	printf("name + resourceSize %s %i\n", head->name, resourceSize);
	switch (head->typeflag) {
		case 0:
		case '0'://Regular file
			if (resourceSize == 0) break;
			puts("Regular file");
			res = this->mainDirectory.addResouce(path, resourceSize);
			chunkBegin = &res->chunkBegin;
			chunkCount = &res->chunkCount;
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
			//puts("Unvaliable typeflag");
			break;
	}
	
	return resourceSize;
}
size_t FileLibrary::readBuffer(struct LZ_Decoder* decoder, uint8_t* data, size_t size) {
	size_t dataApplyBytes = 0,
			readChunkBytes = 0;
	int decompressWriteRes = 0;
	while(dataApplyBytes < size) {
		if (dataApplyBytes+this->chunkSize > size) {
			decompressWriteRes = LZ_decompress_write( decoder, data+dataApplyBytes, size-dataApplyBytes );
			readChunkBytes = this->readChunk(decoder, size-dataApplyBytes);
		}else{
			decompressWriteRes = LZ_decompress_write( decoder, data+dataApplyBytes, this->chunkSize );
			readChunkBytes = this->readChunk(decoder, this->chunkSize);
		}
		if (decompressWriteRes > 0) {
			dataApplyBytes += decompressWriteRes;
		}else{
			break;
		}
	}
	return dataApplyBytes;
}
size_t FileLibrary::readChunk(struct LZ_Decoder* decoder, size_t size) {
	size_t chunkApplyBytes = 0;
	int chunkReadBytes = 0;
	while(chunkApplyBytes < size) {
		while(this->chunkBufferFullness < this->chunkBufferSize) {
			if (size-chunkApplyBytes > this->chunkBufferSize) {
				chunkReadBytes = LZ_decompress_read(decoder, this->chunkBuffer, this->chunkBufferSize);
			}else{
				chunkReadBytes = LZ_decompress_read(decoder, this->chunkBuffer, size-chunkApplyBytes);
			}
			if (chunkReadBytes > 0) {
				chunkApplyBytes += chunkReadBytes;
				this->chunkBufferFullness += chunkReadBytes;
			}else{
				break;
			}
		}
		this->chunkBufferFullness = 0;
		
	}
	
	
	return chunkApplyBytes;
}