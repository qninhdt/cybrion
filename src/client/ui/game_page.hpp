#pragma once

#include "client/GL/framebuffer.hpp"
#include "client/ui/page.hpp"
#include "core/stopwatch.hpp"
#include "world/block/block.hpp"

namespace cybrion::ui
{
    class GamePage : public Page
    {
    public:
        static constexpr i32 ITEM_FRAME_SIZE = 64;

        void onOpen() override;
        void onRender() override;
        void onClose() override;

        void InventoryWidget();
        void BlockMenuWidget();
        bool BlockButton(Block* block);

        bool showBlockMenu = false;

    private:
        GL::Texture m_crosshairTex;
        umap<i32, ref<GL::Framebuffer>> m_itemFrameMap;
        vector<tuple<string, vector<Block*>>> m_blockMenu;
    };
}