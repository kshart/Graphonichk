#include "grProcessingQueue.h"

using namespace Graphonichk;

int ProcessingTask::processExecute() {
	printf("%i \n", this->info);
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