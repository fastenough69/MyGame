#include "IndexBuff.h"

Render::IndexBuff::~IndexBuff()
{
	glDeleteBuffers(1, &id);
}

Render::IndexBuff::IndexBuff(IndexBuff&& right) noexcept
{
	if (this == &right) return;
	id = right.id;
	count = right.count;
	right.id = 0;
	right.count = 0;
}

Render::IndexBuff& Render::IndexBuff::operator=(IndexBuff&& right) noexcept
{
	if (this == &right) return *this;
	id = right.id;
	count = right.count;
	right.id = 0;
	right.count = 0;
	return *this;
}

void Render::IndexBuff::init(const void* data, unsigned int new_count)
{
	count = new_count;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

void Render::IndexBuff::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void Render::IndexBuff::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int Render::IndexBuff::get_count() const
{
	return count;
}