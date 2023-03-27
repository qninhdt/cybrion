#pragma once

namespace cybrion::GL
{
    class Texture
    {
    public:
        Texture();
        void load(const string& path);
        void alloc(i32 width, i32 height);
        void bind(u32 n) const;

        u32 getWidth() const;
        u32 getHeight() const;

        GLuint getId() const;

        ~Texture();

    private:
        void create();

        GLuint m_id;
        u32 m_width;
        u32 m_height;
        u32 m_nChannels;
    };
}