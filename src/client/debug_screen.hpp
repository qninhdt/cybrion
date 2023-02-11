#pragma once

namespace cybrion
{
    class Application;

    class DebugScreen
    {
    public:

        void load();

        ~DebugScreen();

        void render(f32 deltaTime);
    private:
        GLFWwindow* m_window;
        ImGuiIO* m_io;
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    };
}