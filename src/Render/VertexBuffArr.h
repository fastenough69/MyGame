#pragma once

#include <glad/glad.h>

namespace Render
{
	class VertexBuffArr {
		GLuint id{ 0 };
	public:
		VertexBuffArr();
		~VertexBuffArr();
		VertexBuffArr(const VertexBuffArr&) = delete;
		VertexBuffArr& operator=(const VertexBuffArr&) = delete;
		VertexBuffArr(VertexBuffArr&& right) noexcept;
		VertexBuffArr& operator=(VertexBuffArr&& right) noexcept;

		void init(const void* data, unsigned int size);
		void update_data(const void* data, unsigned int size);
		void bind() const;
		void unbind() const;
	};
}//namespace Render