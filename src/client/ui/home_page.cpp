#include "client/ui/home_page.hpp"
#include "client/application.hpp"

namespace cybrion::ui
{
    void HomePage::onOpen()
    {
        m_backgroundTexture.load("ui/background.jpg");
    }

    void HomePage::onRender()
    {
        // render background
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(m_io.DisplaySize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::Begin("Home Page Background", NULL,
                     ImGuiWindowFlags_NoDecoration |
                         ImGuiWindowFlags_NoScrollbar |
                         ImGuiWindowFlags_NoBringToFrontOnFocus |
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoScrollWithMouse);

        i32 bgSize = std::max(m_io.DisplaySize.x, m_io.DisplaySize.y);
        ImGui::Image((ImTextureID)m_backgroundTexture.getId(),
                     ImVec2(bgSize, bgSize));

        ImGui::PopStyleVar(2);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(m_io.DisplaySize.x * 0.5f, m_io.DisplaySize.y - 200), ImGuiCond_Always, ImVec2(0.5f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::Begin("Home Page Menu", NULL,
                     ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoScrollbar |
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_AlwaysAutoResize);

        if (ImGui::Button("Play"))
        {
            Application::Get().currentGame = "lmao";
            Application::Get().goToPage("game");
        }

        ImGui::PopStyleVar(2);
        ImGui::End();
    }

    void HomePage::onClose()
    {
    }
}