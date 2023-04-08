#pragma once

#include "client/GL/framebuffer.hpp"
#include "world/block/block.hpp"

namespace cybrion
{
    class HUD
    {
    public:

        void load();
        
        void render(f32 deltaTime);
        
        

        ~HUD();


    private:
        
        SDL_Window* m_window;
        void* m_context;
        ImGuiIO* m_io;
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        

    };
}