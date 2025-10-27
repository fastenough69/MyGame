#include "VertexArr.h"

Render::VertexArr::~VertexArr()
{
	glDeleteVertexArrays(1, &id);
}

Render::VertexArr::VertexArr(VertexArr &&right) noexcept
{
    if (this == &right)
        return;
    id = right.id;
    index_buffers = right.index_buffers;
    right.id = 0;
    right.index_buffers = 0;
}

Render::VertexArr &Render::VertexArr::operator=(VertexArr &&right) noexcept
{
    if (this == &right)
        return *this;
    id = right.id;
    index_buffers = right.index_buffers;
    right.id = 0;
    right.index_buffers = 0;
    return *this;
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