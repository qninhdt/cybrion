#pragma once

namespace cybrion::GL
{
    class TextureArray
    {
    public:
        TextureArray(u32 width, u32 height, u32 layers, u32 numLevels, GLenum internalFormat, GLenum format, GLenum type);

        void setSubImage(u32 layer, void* data);

        void bind(u32 unit) const;

        ~TextureArray();

    private:
        GLuint m_id;
        u32 m_width;
        u32 m_height;
        u32 m_layers;
        u32 m_numLevels;
        GLuint m_type;
        GLuint m_format;
    };

}