#pragma once

#include "client/GL/texture.hpp"

namespace cybrion::GL
{
    class Framebuffer
    {
    public:
        Framebuffer();
        
        void alloc(i32 width, i32 height);
        void bind();
        void unbind();

        Texture& getTexture();

        ~Framebuffer();
    private:

        GLuint m_id;
        Texture m_texture;
        GLuint m_renderId;
    };
}