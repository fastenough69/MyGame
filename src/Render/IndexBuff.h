#pragma once

#include <glad/glad.h>

namespace Render {
	class IndexBuff {
		GLuint id{ 0 };
		unsigned int count{ 0 };
	public:
		IndexBuff() = default;
		~IndexBuff();
		IndexBuff(const IndexBuff&) = delete;
		IndexBuff& operator=(const IndexBuff&) = delete;
		IndexBuff(IndexBuff&& right) noexcept;
		IndexBuff& operator=(IndexBuff&& right) noexcept;
		void init(const void* data, unsigned int new_count);
		void bind();
		void unbind();
		unsigned int get_count() const;
	};
}// namespace Render