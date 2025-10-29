#include "Texture2D.h"

namespace Render
{
Texture2D::Texture2D(const GLuint width, const GLuint height, unsigned char *data, unsigned int chanels,
                     const GLenum filter, const GLenum wrapMode)
    : width{width}, height{height}
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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t_id);
    glTexImage2D(GL_TEXTURE_2D, 0, t_mode, width, height, 0, t_mode, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, wrapMode);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(Texture2D &&right) noexcept
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

Texture2D &Texture2D::operator=(Texture2D &&right) noexcept
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
    glBindTexture(GL_TEXTURE_2D, t_id);
}

void Texture2D::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::clear_alpha_chanel(unsigned char *data, int width, int height)
{
    int min_x = width, max_x = 0, min_y = height, max_y = 0;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned char *pixels = data + (y * width + x) * 4;
            if (pixels[3] > 50)
            {
                if (min_x > x)
                    min_x = x;
                if (max_x < x)
                    max_x = x;
                if (min_y > y)
                    min_y = y;
                if (max_y < y)
                    max_y = y;
            }
        }
    }
    cords = {(float)min_x / width, (float)max_x / width, (float)min_y / height, (float)max_y / height};
}
} // namespace Render