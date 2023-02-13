#pragma once

namespace cybrion::GL
{
    class Texture
    {
    public:
        void load(const string& path);
        void bind(u32 n) const;

        u32 getWidth() const;
        u32 getHeight() const;

        GLuint getId() const;

        ~Texture();

    private:
        GLuint m_id;
        u32 m_width;
        u32 m_height;
        u32 m_nChannels;
    };
}