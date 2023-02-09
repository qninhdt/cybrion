#include "client/debug_screen.hpp"
#include "client/client.hpp"

namespace cybrion
{
    DebugScreen::DebugScreen(Window& window) :
        m_window(&window)
    {
    }

    void DebugScreen::init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        m_io = &ImGui::GetIO();

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(m_window->getNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 430");
    }

    void DebugScreen::render()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("FPS: %.0f", Client::Get().getFPS());
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_window->getNativeWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    DebugScreen::~DebugScreen()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}