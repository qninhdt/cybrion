#include "client/ui/world_list_page.hpp"
#include "client/application.hpp"

namespace cybrion::ui
{
    WorldListPage::WorldListPage()
    {
        m_backgroundTexture.load("ui/background.png");
    }

    void WorldListPage::onOpen()
    {
        loadWorldList();
    }

    void WorldListPage::onRender()
    {
        // render background
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(m_io.DisplaySize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::Begin("World List Background", NULL,
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

        // list
        ImGui::SetNextWindowPos(ImVec2(m_io.DisplaySize.x * 0.5f, 200), ImGuiCond_Always, ImVec2(0.5f, 0.0f));

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 25));

        ImGui::Begin("World List", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

        ImGui::PopStyleColor(1);
        ImGui::PopStyleVar(1);

        if (ImGui::Button("Back"))
        {
            Application::Get().goToPage("home");
        }

        ImGui::SameLine();

        ImGui::PushID("ncct");
        ImGui::InputText("", m_worldInput, 64);
        ImGui::PopID();

        ImGui::SameLine();

        if (ImGui::Button("Create world"))
        {
            if (isValidName(m_worldInput))
            {
                World::createNewWorld(m_worldInput);
                loadWorldList();
                std::strcpy(m_worldInput, "");
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Reload"))
        {
            loadWorldList();
        }

        for (i32 i = 0; i < m_worldList.size(); ++i)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 5));
            ImGui::BeginChildFrame(i + 140302, ImVec2(0, 40));
            ImGui::PopStyleVar(1);

            ImGui::Text("%s", m_worldList[i].name.c_str());

            ImGui::SameLine(350);
            ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), "%.1f Mb", m_worldList[i].size);

            ImGui::SameLine(ImGui::GetWindowWidth() - 50);

            ImGui::PushID(i + 999);
            if (ImGui::Button("Play"))
            {
                Application::Get().currentGame = m_worldList[i].name;
                Application::Get().goToPage("game");
            }
            ImGui::PopID();
            ImGui::EndChildFrame();
        }

        ImGui::End();
    }

    bool WorldListPage::isValidName(const string &name) const
    {
        if (name.length() == 0)
            return false;

        for (char c : name)
            if (!(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '-' || c == '_' || c == ' '))
                return false;

        for (auto &info : m_worldList)
            if (info.name == name)
                return false;

        return true;
    }

    void WorldListPage::loadWorldList()
    {
        m_worldList.clear();

        string worldFolder = Application::Get().getSavePath("");

        for (const auto &entry : std::filesystem::directory_iterator(worldFolder))
        {
            string configFile = entry.path().string() + "/world.jbt";

            if (!std::filesystem::exists(configFile))
                continue;

            jbt::tag config;
            jbt::open_tag(config, configFile);

            i32 totalSize = 0;
            for (const auto &region0 : std::filesystem::directory_iterator(entry.path().string() + "/region"))
            {
                totalSize += std::filesystem::file_size(region0.path().string());
            }

            WorldInfo info;
            info.name = config.get_string("name");
            info.size = totalSize / 1024.0f / 1024.0f;

            m_worldList.push_back(info);
        }
    }

    void WorldListPage::onClose()
    {
    }

}