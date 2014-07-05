#include "grMain.h"
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
	if (OpenGL::ver==OpenGL::VER_CORE_100) {
		return true;
	}else if (ShaderF3D::prog->shaderProgram!=0) {
		SET_SHADER(ShaderF3D);

		glGenVertexArrays(1, &this->model->vao);
		if (this->model->vao==0) puts("VAO_NULL");
		glBindVertexArray(this->model->vao);
		printf("ShaderF3D::prog %i\n", ShaderF3D::prog->position);
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
	return false;
}

void ProcessingSupSuspend::setTime(float time) {
	this->_time = time;
}
float ProcessingSupSuspend::getTime() {
	return this->_time;
}
int ProcessingSupSuspend::performTasks() {
	LARGE_INTEGER time1, time2;
	LARGE_INTEGER frequencyStruct;
	float frequency, time;
	ThreadData data;
	DWORD exitCode;
	QueryPerformanceFrequency(&frequencyStruct);
	frequency = (float)frequencyStruct.QuadPart;
	
	pthread_mutex_lock(&this->_accessPush);
	this->_queueIsUse = !this->_queueIsUse;
	pthread_mutex_unlock(&this->_accessPush);
	if (this->_queueIsUse == 1) {
		while ( !this->_essentialTasks1.empty() ) {
			QueryPerformanceCounter(&time1);
			if (this->_threadHandle==nullptr) {
				data.sush = &this->_suspendHandle;
				data.task = this->_essentialTasks1.front();
				pthread_create(&_threadHandle, NULL, ProcessingSupSuspend::threadFunction, &data);
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
			pthread_cancel(_threadHandle);
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
				pthread_create(&_threadHandle, NULL, ProcessingSupSuspend::threadFunction, &data);
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
			pthread_cancel(_threadHandle);
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
void* ProcessingSupSuspend::threadFunction (void* dataArg) {
	//puts("ProcessingSupSuspend::threadFunction");
	((ThreadData*)dataArg)->task->processExecute(((ThreadData*)dataArg)->sush);
	return 0; 
}


Array<ProcessingThread*> ProcessingThread::threads(3);

void ProcessingThread::init() {
	for (int i=0; i<ProcessingThread::threads.size; i++) {
		ProcessingThread::threads[i] = new ProcessingThread();
	}
}
ProcessingThread::ProcessingThread() {
	pthread_create(&thread, NULL, ProcessingThread::threadProcess, this);
}
ProcessingThread::~ProcessingThread() {
	pthread_cancel(thread);
}
int ProcessingThread::performTasks() {
	pthread_mutex_lock(&this->_accessPush);
	this->_queueIsUse = !this->_queueIsUse;
	pthread_mutex_unlock(&this->_accessPush);
	if (this->_queueIsUse == 1) {
		//printf("performTasks %i\n", this->essentialTasks1.size());
		while ( !this->_essentialTasks1.empty() ) {
			if (  this->_essentialTasks1.front()->processExecute()  ) {
				delete this->_essentialTasks1.front();
			}else{
				this->addTask(this->_essentialTasks1.front());
			}
			this->_essentialTasks1.pop();
		}
	}else{
		//printf("performTasks %i\n", this->essentialTasks2.size());
		while ( !this->_essentialTasks2.empty() ) {
			if (  this->_essentialTasks2.front()->processExecute()  ) {
				delete this->_essentialTasks2.front();
			}else{
				this->addTask(this->_essentialTasks2.front());
			}
			this->_essentialTasks2.pop();
		}
	}
	return true;
}
void* ProcessingThread::threadProcess(void* sys) {
	ProcessingThread *thread = (ProcessingThread*)sys;
	
	while(true) {
		thread->performTasks();
		Sleep(100);
	}
}