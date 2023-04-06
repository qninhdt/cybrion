#pragma once

#include "client/GL/framebuffer.hpp"
#include "world/block/block.hpp"

namespace cybrion
{
    class HUD
    {
    public:
        static constexpr i32 ITEM_FRAME_SIZE = 64;

        void load();
        
        void render(f32 deltaTime);
        
        void renderInventory();
        void renderBlockMenu();
        bool renderBlock(Block* block);

        ~HUD();

        bool showBlockMenu = false;

    private:
        vector<tuple<string, vector<Block*>>> m_blockMenu;
        
        SDL_Window* m_window;
        void* m_context;
        ImGuiIO* m_io;
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        GL::Texture m_crosshairTex;
        umap<i32, ref<GL::Framebuffer>> m_itemFrameMap;
    };
}