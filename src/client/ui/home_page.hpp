#pragma once

#include "client/ui/page.hpp"
#include "client/GL/texture.hpp"

namespace cybrion::ui
{
    class HomePage : public Page
    {
    public:
        HomePage();
        void onOpen() override;
        void onRender() override;
        void onClose() override;

    private:
        GL::Texture m_titleTexture;
        GL::Texture m_backgroundTexture;
        GL::Texture m_playTexture;
        GL::Texture m_hoveredPlayTexture;
        GL::Texture m_quitTexture;
        GL::Texture m_hoveredQuitTexture;
    };
}