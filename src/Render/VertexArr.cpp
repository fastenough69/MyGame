#include "VertexArr.h"

Render::VertexArr::~VertexArr()
{
	glDeleteVertexArrays(1, &id);
}

void Render::VertexArr::init()
{
	glGenVertexArrays(1, &id);
}

void Render::VertexArr::bind()
{
	glBindVertexArray(id);
}

void Render::VertexArr::unbind()
{
	glBindVertexArray(0);
}

void Render::VertexArr::add_buffer(const VertexBuffArr& vbo, 
									int size, GLenum type, 
									GLboolean norm, 
									GLsizei stride, const void* ptr)
{
	bind();
	glEnableVertexAttribArray(index_buffers);
	vbo.bind();
	glVertexAttribPointer(index_buffers, (GLint)size, type, norm, stride, ptr);
	index_buffers++;
}

void Render::VertexArr::add_buffer(IndexBuff& emo, int size, GLenum type, GLboolean norm, GLsizei stride, const void* ptr)
{
	bind();
	glEnableVertexAttribArray(index_buffers);
	emo.bind();
	glVertexAttribPointer(index_buffers, (GLint)size, type, norm, stride, ptr);
	index_buffers++;
}