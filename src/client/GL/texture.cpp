#include "client/GL/texture.hpp"
#include "client/application.hpp"

namespace cybrion::GL
{
    void Texture::load(const string& path)
    {
        string realPath = Application::Get().getResourcePath("textures/" + path);

        i32 width, height, nChannels;
        u8* data = stbi_load(realPath.c_str(), &height, &width, &nChannels, STBI_rgb_alpha);

        if (!data)
        {
            CYBRION_ERROR("Cannot load file: {}", realPath);
            return;
        }

        m_width = width;
        m_height = height;
        m_nChannels = nChannels;

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    void Texture::bind(u32 n) const
    {
        glBindTexture(GL_TEXTURE0 + n, m_id);
    }

    u32 Texture::getWidth() const
    {
        return m_width;
    }

    u32 Texture::getHeight() const
    {
        return m_height;
    }

    GLuint Texture::getId() const
    {
        return m_id;
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_id);
    }
}
