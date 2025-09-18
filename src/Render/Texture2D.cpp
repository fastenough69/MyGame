#include "Texture2D.h"

namespace Render {
	Texture2D::Texture2D(const GLuint width,
						const GLuint height,
						unsigned char* data,
						unsigned int chanels,
						const GLenum filter,
						const GLenum wrapMode): width{ width }, height{height}
	{
		switch (chanels)
		{
		case 4:
			t_mode = GL_RGBA;
			break;
		case 3:
			t_mode = GL_RGB;
			break;
		default:
			t_mode = GL_RGBA;
			break;
		}
		glGenTextures(1, &t_id);
		glBindTexture(GL_TEXTURE_2D, t_id);
		glTexImage2D(GL_TEXTURE_2D, 0, t_mode, width, height, 0, t_mode, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, wrapMode);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	/*void Texture2D::swap(Texture2D obj)
	{
		std::swap(this->t_id, (unsigned int)obj.t_id);
		std::swap(this->width, obj.width);
		std::swap(this->height, obj.height);
		GLenum temp = obj.t_mode;
		obj.t_mode = this->t_mode;
		this->t_mode = temp;
	}*/

	Texture2D::Texture2D(Texture2D&& right) noexcept
	{
		if (this == &right)
		{
			return;
		}

		t_id = right.t_id;
		width = right.width;
		height = right.height;
		t_mode = right.t_mode;
		right.t_id = 0;
		right.width = 0;
		right.height = 0;
		right.t_mode = 0;
	}

	Texture2D& Texture2D::operator=(Texture2D&& right) noexcept
	{
		glDeleteTextures(1, &t_id);
		if (this == &right)
		{
			return *this;
		}

		t_id = right.t_id;
		width = right.width;
		height = right.height;
		t_mode = right.t_mode;
		right.t_id = 0;
		right.width = 0;
		right.height = 0;
		right.t_mode = 0;
		return *this;
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &t_id);
	}

	void Texture2D::bind() const
	{

	}
}