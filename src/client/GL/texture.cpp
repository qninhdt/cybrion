#include "client/GL/texture.hpp"
#include "client/application.hpp"

namespace cybrion::GL
{
    Texture::Texture()
    {
    }

    void Texture::load(const string &path)
    {
        string realPath = Application::Get().getResourcePath("textures/" + path);

        i32 width, height, nChannels;
        u8 *data = stbi_load(realPath.c_str(), &width, &height, &nChannels, STBI_default);

        if (!data)
        {
            CYBRION_ERROR("Cannot load file: {}", realPath);
            return;
        }

        m_width = width;
        m_height = height;
        m_nChannels = nChannels;

        create();

        GLint type = nChannels == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    void Texture::alloc(i32 width, i32 height)
    {
        create();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
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

    void Texture::create()
    {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}
