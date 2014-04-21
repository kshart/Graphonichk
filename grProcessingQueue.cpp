#include "grBaseTypes.h"
#include "grProcessingQueue.h"

using namespace Graphonichk;
EachFrameTask::EachFrameTask() :info(0) {
	
}
int EachFrameTask::processExecute() {
	fputs("EachFrameTask::processExecute\n", iovir);
	return false;
}
int ProcessingTask::processExecute() {
	fputs("ProcessingTask::processExecute\n", iovir);
	return true;
}
int HeapFreeTask::processExecute() {
	free(data);
	return true;
}
int VBOUpdateTask::processExecute() {
	glBindBuffer(GL_ARRAY_BUFFER, this->bufferID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->dataSize, this->data);
	return true;
}
int Model3DLoadTask::processExecute() {
	if (GLShader::shader->crc32!=ShaderF3D::CRC32) GLShader::setShader(ShaderF3D::prog);
	
	glGenVertexArrays(1, &this->model->vao);
	if (this->model->vao==0) printf("VAO_NULL");
	glBindVertexArray(this->model->vao);
	
	glGenBuffers(1, &this->model->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->model->vbo);
	glBufferData(GL_ARRAY_BUFFER, this->model->vertex.size()*3*sizeof(float), (GLvoid*)&this->model->vertex.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(ShaderF3D::prog->position, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(ShaderF3D::prog->position);
	
	glGenBuffers(1, &this->model->vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->model->vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->model->polygon.size()*3*sizeof(uint), (GLvoid*)&this->model->polygon.front(), GL_STATIC_DRAW);
	return true;
}

ProcessingSupSuspend::ProcessingSupSuspend() :_threadHandle(nullptr) {
	this->_suspendHandle = CreateSemaphore(NULL, 1, 1, NULL);
}
void ProcessingSupSuspend::setTime(float time) {
	this->_time = time;
}
float ProcessingSupSuspend::getTime() {
	return this->_time;
}
int ProcessingSupSuspend::addTask(SupSuspendTask *task, int type) {
	CRITICAL_SECTION_INTER(this->_accessPush);
	if (this->_queueIsUse == 1) {
		this->_essentialTasks2.push(task);
	}else{
		this->_essentialTasks1.push(task);
	}
	CRITICAL_SECTION_LEAVE(this->_accessPush);
	return true;
};
int ProcessingSupSuspend::performTasks() {
	LARGE_INTEGER time1, time2;
	LARGE_INTEGER frequencyStruct;
	float frequency, time;
	ThreadData data;
	DWORD exitCode;
	QueryPerformanceFrequency(&frequencyStruct);
	frequency = (float)frequencyStruct.QuadPart;
	
	CRITICAL_SECTION_INTER(this->_accessPush);
	this->_queueIsUse = !this->_queueIsUse;
	CRITICAL_SECTION_LEAVE(this->_accessPush);
	if (this->_queueIsUse == 1) {
		while ( !this->_essentialTasks1.empty() ) {
			QueryPerformanceCounter(&time1);
			if (this->_threadHandle==nullptr) {
				data.sush = &this->_suspendHandle;
				data.task = this->_essentialTasks1.front();
				this->_threadHandle = CreateThread(NULL, 0, ProcessingSupSuspend::threadFunction, &data, 0, 0);
			}
			GetExitCodeThread(this->_threadHandle, &exitCode);
			while (exitCode == STILL_ACTIVE ) {
				if (WaitForSingleObject(this->_suspendHandle, this->_time) == WAIT_TIMEOUT) {
					WaitForSingleObject(this->_suspendHandle, INFINITE);
					this->_time = 0;
					puts("WAIT_TIMEOUT");
					return false;
				}else{
					puts("ITERATION");
					QueryPerformanceCounter(&time2);
					time = (float)(time2.QuadPart-time1.QuadPart)/frequency;
					this->_time -= time;
					QueryPerformanceCounter(&time1);
					ReleaseSemaphore(this->_suspendHandle, 1, NULL);
				}
				GetExitCodeThread(this->_threadHandle, &exitCode);
			}
			puts("CLOSE");
			CloseHandle(this->_threadHandle);
			this->_threadHandle = nullptr;
			delete this->_essentialTasks1.front();
			this->_essentialTasks1.pop();
			QueryPerformanceCounter(&time2);
			time = (float)(time2.QuadPart-time1.QuadPart)/frequency;
			this->_time -= time;
		}
	}else{
		while ( !this->_essentialTasks2.empty() ) {
			QueryPerformanceCounter(&time1);
			if (this->_threadHandle==nullptr) {
				data.sush = &this->_suspendHandle;
				data.task = this->_essentialTasks2.front();
				this->_threadHandle = CreateThread(NULL, 0, ProcessingSupSuspend::threadFunction, &data, 0, 0);
			}
			GetExitCodeThread(this->_threadHandle, &exitCode);
			while (exitCode == STILL_ACTIVE ) {
				if (WaitForSingleObject(this->_suspendHandle, this->_time) == WAIT_TIMEOUT) {
					WaitForSingleObject(this->_suspendHandle, INFINITE);
					this->_time = 0;
					puts("WAIT_TIMEOUT");
					return false;
				}else{
					puts("ITERATION");
					QueryPerformanceCounter(&time2);
					time = (float)(time2.QuadPart-time1.QuadPart)/frequency;
					this->_time -= time;
					QueryPerformanceCounter(&time1);
					ReleaseSemaphore(this->_suspendHandle, 1, NULL);
				}
				GetExitCodeThread(this->_threadHandle, &exitCode);
			}
			puts("CLOSE");
			CloseHandle(this->_threadHandle);
			this->_threadHandle = nullptr;
			delete this->_essentialTasks2.front();
			this->_essentialTasks2.pop();
			QueryPerformanceCounter(&time2);
			time = (float)(time2.QuadPart-time1.QuadPart)/frequency;
			this->_time -= time;
		}
	}
	return true;
};
THREAD ProcessingSupSuspend::threadFunction (void* dataArg) {
	((ThreadData*)dataArg)->task->processExecute(((ThreadData*)dataArg)->sush);
	return 0;
}