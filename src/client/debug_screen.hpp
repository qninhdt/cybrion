#pragma once

#include "client/window.hpp"

namespace cybrion
{
    class DebugScreen
    {
    public:
        DebugScreen(Window& window);

        void init();

        ~DebugScreen();

        void render();
    private:
        Window* m_window;
        ImGuiIO* m_io;
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    };
}