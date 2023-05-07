#include "client/ui/home_page.hpp"
#include "client/application.hpp"
#include "client/ui/controls.hpp"

namespace cybrion::ui
{
    HomePage::HomePage()
    {
        m_backgroundTexture.load("ui/background.png");

        m_titleTexture.load("ui/title.png");

        m_playTexture.load("ui/play_button.png");
        m_hoveredPlayTexture.load("ui/hovered_play_button.png");

        m_quitTexture.load("ui/quit_button.png");
        m_hoveredQuitTexture.load("ui/hovered_quit_button.png");
    }

    void HomePage::onOpen()
    {
    }

    void HomePage::onRender()
    {
        // render background
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(m_io.DisplaySize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::Begin("Home Page Background", NULL,
                     ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoBringToFrontOnFocus |
                         ImGuiWindowFlags_NoDecoration |
                         ImGuiWindowFlags_NoScrollWithMouse);

        f32 bgRatio = 1.0f * m_backgroundTexture.getWidth() / m_backgroundTexture.getHeight();
        ImGui::Image((ImTextureID)(intptr_t)m_backgroundTexture.getId(),
                     (m_io.DisplaySize.x / m_io.DisplaySize.y > bgRatio)
                         ? ImVec2(m_io.DisplaySize.x, m_io.DisplaySize.x / bgRatio)
                         : ImVec2(m_io.DisplaySize.y * bgRatio, m_io.DisplaySize.y));

        ImGui::PopStyleVar(2);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(m_io.DisplaySize.x * 0.5f, m_io.DisplaySize.y * 0.05 + 20), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 15));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::Begin("Home Title", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Image((ImTextureID)(intptr_t)m_titleTexture.getId(), ImVec2(600, 600 / 4));

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(1);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(m_io.DisplaySize.x * 0.5f, m_io.DisplaySize.y * 0.95f - 50), ImGuiCond_Always, ImVec2(0.5f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.3f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 30));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::Begin("Home Page Menu", NULL,
                     ImGuiWindowFlags_NoDecoration |
                         ImGuiWindowFlags_AlwaysAutoResize);

        static bool playButton = false;
        if (HoverableImageButton(m_playTexture, m_hoveredPlayTexture, 300, 300 / 4, playButton))
        {
            Application::Get().goToPage("world_list");
        }

        static bool quitButton = false;
        if (HoverableImageButton(m_quitTexture, m_hoveredQuitTexture, 300, 300 / 4, quitButton))
        {
            Application::Get().close();
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(1);
        ImGui::End();
    }

    void HomePage::onClose()
    {
    }
}