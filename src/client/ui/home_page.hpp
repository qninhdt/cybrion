#pragma once

#include "client/ui/page.hpp"
#include "client/GL/texture.hpp"

namespace cybrion::ui
{
    class HomePage : public Page
    {
    public:
        void onOpen() override;
        void onRender() override;
        void onClose() override;
    private:
        GL::Texture m_backgroundTexture;
    };
}