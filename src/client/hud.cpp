#include "client/hud.hpp"
#include "client/application.hpp"

namespace cybrion
{

    void HUD::load()
    {
        m_window = Application::Get().getWindow();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        m_io = &ImGui::GetIO();

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init("#version 430");

        m_crosshairTex.load("hud/crosshair.png");
    }

    void HUD::render(f32 deltaTime)
    {
        auto& app = Application::Get();
        u32 width = app.getWidth();
        u32 height = app.getHeight();

        auto& game = LocalGame::Get();
        auto& camera = game.getCamera();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // render crosshair
        u32 crosshairSize = 32;
        ImGui::SetNextWindowPos(ImVec2(width / 2 - crosshairSize / 2, height / 2 - crosshairSize / 2));
        ImGui::SetNextWindowSize(ImVec2(crosshairSize, crosshairSize));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        ImGui::Begin("Crosshair", nullptr, 
            ImGuiWindowFlags_NoDecoration | 
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoBackground
        );

        ImGui::Image((void*)(intptr_t)m_crosshairTex.getId(), ImVec2(crosshairSize, crosshairSize));

        ImGui::End();
        ImGui::PopStyleVar();

        // render debug info
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Debug", nullptr,
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse
        );
        ImGui::Text("FPS: %.0f", 1.0f/deltaTime);

        vec3 pos = camera.getPos();
        ImGui::Text("Pos: %.2f %.2f %.2f", pos.x, pos.y, pos.z);

        Block* block = game.getPlayer().getTargetBlock();

        if (block)
        {
            vec3 pos = game.getPlayer().getTargetPos();
            ImGui::Text("Target: %.0f %.0f %.0f", pos.x, pos.y, pos.z);
            ImGui::Text("Block: %s", block ? block->getDisplayName().c_str() : "None");
            ImGui::Text("%s", block->toString().c_str());
        }

        ImGui::End();

        // render toolbox
        ImGui::Begin("Toolbox");
        
        if (ImGui::Button("Reload shader"))
            ShaderManager::Get().reloadShaders();

        static bool _ = false;
        if (ImGui::Checkbox("Wireframe", &_))
            LocalGame::Get().toggleWireframe();

        ImGui::Checkbox("Chunk border", &game.m_showChunkBoder);
        ImGui::Checkbox("Entity border", &game.m_showEntityBorder);

        ImGui::Checkbox("Enable diffuse", &game.m_worldRenderer.m_enableDiffuse);
        ImGui::Checkbox("Enable AO", &game.m_worldRenderer.m_enableAO);


        ImGui::End();

        // render everything to window
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    HUD::~HUD()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}