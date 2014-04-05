#include "grBaseTypes.h"
#include "grProcessingQueue.h"

using namespace Graphonichk;
EachFrameTask::EachFrameTask() :info(0) {
	
}
int EachFrameTask::processExecute() {
	fprintf(stderr, "call int EachFrameTask::processExecute(%i)", this->info);
	return false;
}
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