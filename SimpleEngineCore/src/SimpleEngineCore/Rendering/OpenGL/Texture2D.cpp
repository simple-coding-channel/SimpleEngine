#include "Texture2D.hpp"

#include <algorithm>
#include <cmath>
#include <glad/glad.h>

namespace SimpleEngine
{
    Texture2D::Texture2D(const unsigned char* data, const unsigned int width, const unsigned int height)
        : m_width(width)
        , m_height(height)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        const GLsizei mip_levels = static_cast<GLsizei>(std::log2(std::max(m_width, m_height))) + 1;
        glTextureStorage2D(m_id, mip_levels, GL_RGB8, m_width, m_height);
        glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateTextureMipmap(m_id);
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &m_id);
    }

    Texture2D& Texture2D::operator=(Texture2D&& texture) noexcept
    {
        glDeleteTextures(1, &m_id);
        m_id = texture.m_id;
        m_width = texture.m_width;
        m_height = texture.m_height;
        texture.m_id = 0;
        return *this;
    }

    Texture2D::Texture2D(Texture2D&& texture) noexcept
    {
        m_id = texture.m_id;
        m_width = texture.m_width;
        m_height = texture.m_height;
        texture.m_id = 0;
    }

    void Texture2D::bind(const unsigned int unit) const
    {
        glBindTextureUnit(unit, m_id);
    }
}
