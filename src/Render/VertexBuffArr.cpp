#include "VertexBuffArr.h"
#include "IndexBuff.h"

Render::VertexBuffArr::VertexBuffArr(): id{0}
	{	}

Render::VertexBuffArr::~VertexBuffArr()
{
	glDeleteBuffers(1, &id);
}

Render::VertexBuffArr::VertexBuffArr(VertexBuffArr&& right) noexcept
{
	if (this == &right)
	{
		return;
	}
	id = right.id;
	right.id = 0;
}

Render::VertexBuffArr& Render::VertexBuffArr::operator=(VertexBuffArr&& right) noexcept
{
	if (this == &right)
	{
		return *this;
	}
	id = right.id;
	right.id = 0;
	return *this;
}

void Render::VertexBuffArr::init(const void* data, unsigned int size)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void Render::VertexBuffArr::update_data(const void* data, unsigned int size)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void Render::VertexBuffArr::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void Render::VertexBuffArr::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}