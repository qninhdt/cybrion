#include "client/GL/framebuffer.hpp"

namespace cybrion::GL
{
    Framebuffer::Framebuffer()
    {
    }

    void Framebuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    }

    void Framebuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Texture& Framebuffer::getTexture()
    {
        return m_texture;
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &m_id);
        glDeleteRenderbuffers(1, &m_renderId);
    }

    void Framebuffer::alloc(i32 width, i32 height)
    {
        glGenFramebuffers(1, &m_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        glViewport(0, 0, width, height);

        m_texture.alloc(width, height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.getId(), 0);

        glGenRenderbuffers(1, &m_renderId);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderId);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            CYBRION_CLIENT_ERROR("Framebuffer is not complete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}