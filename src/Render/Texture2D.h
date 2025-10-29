#pragma once

#include <glad/glad.h>
#include <iostream>
#include <string>

namespace Render {
	struct uvCords{
		float u_min, u_max;
		float v_min, v_max;
	};

	class Texture2D {
		GLuint t_id{ 0 };
		unsigned int width{ 0 };
		unsigned int height{ 0 };
		GLenum t_mode{ 0 };
		uvCords cords{};
	public:
		Texture2D(const GLuint width, 
					const GLuint height, 
					unsigned char* data, 
					unsigned int chanels=4, 
					const GLenum filter=GL_LINEAR, 
					const GLenum wrapMode=GL_REPEAT);
		Texture2D() = delete;
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		~Texture2D();
		Texture2D(Texture2D&& right) noexcept;
		Texture2D& operator=(Texture2D&& right) noexcept;
		void bind() const;
		void unbind();
		void clear_alpha_chanel(unsigned char* data, int width, int height);
		uvCords get_uv_cord() const
		{
			return cords;
		}
	};

}