#include "client/GL/texture_array.hpp"

namespace cybrion::GL
{
    TextureArray::TextureArray(u32 width, u32 height, u32 layers, u32 numLevels, GLenum internalFormat, GLenum format, GLenum type) :
        m_width(width),
        m_height(height),
        m_layers(layers),
        m_numLevels(numLevels),
        m_format(format),
        m_type(type)
    {
        glGenTextures(1, &m_id);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);

        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, width, height, layers, 0, format, type, nullptr);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    void TextureArray::setSubImage(u32 layer, void* data)
    {
        u32 height = m_height;
        u32 width = m_width;

        for (u32 level = 0; level <= m_numLevels; ++level, height /= 2, width /= 2)
        {
            glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, layer, width, height, 1, m_format, m_type, data);
            glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
            glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        }
    }

    void TextureArray::bind(u32 unit) const
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
    }

    TextureArray::~TextureArray()
    {
        glDeleteTextures(1, &m_id);
    }
}