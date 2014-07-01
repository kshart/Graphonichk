#include "grMain.h"
#include "grLibrary.h"


#include "lzlib.h"
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
	printf("%s\n", this->name.c_str());
	for(auto res : recources) {
		printf("\tname='%s' path='%s' size='%i'\n", res.name.c_str(), res.path.c_str(), res.size);
	}
	for(auto dir : derictories) {
		dir.trace();
	}
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
	
	FILE *file = fopen("dir1.tar.lz", "rb");
	
	
	struct LZ_Decoder* decoder = LZ_decompress_open();
	
	
	if( !decoder || LZ_decompress_errno( decoder ) != LZ_ok ) {
		//Pp_show_msg( pp, "Not enough memory. Find a machine with more memory" );
		//retval = 1;
		puts("FileLibrary::STATUS::ERROR_OUTMEM");
		this->status = STATUS::ERROR_OUTMEM;
	}else{
		//retval = do_decompress( decoder, infd, pp, testing );
	}
	fseek(file, 0, SEEK_END);
	size_t lengthReadBuffer = 100,//LZ_decompress_write_size(decoder), 
			sizeFile = ftell(file), 
			readBytesInFile = 0,
			leaveWriteBytes = 0,
			leaveReadBytes = 0,
			readPosition = 0,
			readBytes = 0,
			readBytesApply = 0,
			blockReadBytes = 0,
			skipBlock = 0;
	int readBytesRes = 0;
	uint8_t *buffer = (uint8_t*)malloc(lengthReadBuffer);
	uint8_t *bufferRe = (uint8_t*)malloc(512*2);
	fseek(file, 0, SEEK_SET);
	while(readBytesInFile < sizeFile) {
		if (leaveWriteBytes > 0) {
			//memmove(buffer, buffer+(lengthReadBuffer-leaveWriteBytes), leaveWriteBytes);
			fread(buffer+leaveWriteBytes, lengthReadBuffer-leaveWriteBytes, 1, file);
			
		}else{
			fread(buffer, lengthReadBuffer, 1, file);
		}
		readBytesInFile = ftell(file);
		
		leaveWriteBytes = lengthReadBuffer - LZ_decompress_write( decoder, buffer, lengthReadBuffer );
		//puts("read");
		while ( true ) {
			if (blockReadBytes > 0) {
				readBytesRes = LZ_decompress_read(decoder, bufferRe+blockReadBytes, 512*2-blockReadBytes);

			}else{
				readBytesRes = LZ_decompress_read(decoder, bufferRe, 512*2);
			}
			if (readBytesRes < 0) {
				break;
			}else if (readBytesRes == 0) {
				break;
			}
			
			blockReadBytes += readBytesRes;
			//printf("blockReadBytes %i\n", blockReadBytes);
			if (blockReadBytes >= 512*2) {
				for(int block=0; block<2; block++) {
					if (skipBlock > 0) {
						skipBlock--;
					}else{
						size_t blockSize = this->readBlock(bufferRe+512*block);
						skipBlock = (size_t)ceil(blockSize/512.0f);
						//printf("skipBlock %i\n", skipBlock);
					}
				}
				
				blockReadBytes = 0;
			}
			readBytes += readBytesRes;
		}
		//puts("read end");
		if (readBytesRes < 0) {
			//printf(" %s\n", LZ_strerror(LZ_decompress_errno(decoder)) );
			break;
		}
		readPosition = LZ_decompress_member_position( decoder );
		//printf(" %i %i %i %i %i\n", readBytesInFile, leaveWriteBytes, readBytes, readPosition, LZ_decompress_data_position(decoder) );
		
	}
	//printf(" %i\n", LZ_decompress_read(decoder, bufferRe, 100000));
	//printf(" %i\n", LZ_decompress_member_position( decoder ));
	////size_t blockSize = this->readResouce(bufferRe);
	//blockSize = (size_t)ceil(blockSize/200.0f)*200;
	
	this->mainDirectory.trace();
	fclose(file);
	LZ_decompress_close( decoder );
}

size_t FileLibrary::readBlock(uint8_t* data) {
	tarHead *head = (tarHead*)data;
	head->name[99] = 0;
	head->mode[7] = 0;
	head->uid[7] = 0;
	head->gid[7] = 0;
	head->size[11] = 0;
	head->mtime[11] = 0;
	string path((char*)head->name);
	size_t resourceSize = atoi((char*)head->size);
	printf("%i\n", resourceSize);
	switch (head->typeflag) {
		case 0:
		case '0'://Regular file
			if (resourceSize == 0) break;
			puts("Regular file");
			this->mainDirectory.addResouce(path, resourceSize);
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
	}
	
	return resourceSize;
}