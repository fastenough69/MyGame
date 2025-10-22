#pragma once

#include <glad/glad.h>
#include "VertexBuffArr.h"
#include "IndexBuff.h"

namespace Render 
{
	class VertexArr {
		GLuint id{ 0 };
		unsigned int index_buffers{ 0 };
	public:
		VertexArr() = default;
		~VertexArr();
		VertexArr(const VertexArr&) = delete;
		VertexArr& operator=(const VertexArr&) = delete;
		VertexArr(VertexArr&& right) noexcept;
		VertexArr& operator=(VertexArr&& right) noexcept;
		void init();
		void bind();
		void unbind();
		void add_buffer(const VertexBuffArr& vbo, int size, GLenum type = GL_FLOAT, GLboolean norm = GL_FALSE, GLsizei stride = 0, const void* ptr = nullptr);
		void add_buffer(IndexBuff& emo, int size, GLenum type = GL_FLOAT, GLboolean norm = GL_FALSE, GLsizei stride = 0, const void* ptr = nullptr);

	};
}// namespace Render