#include "client/hud.hpp"
#include "client/application.hpp"
#include "client/graphic/block_renderer.hpp"

namespace cybrion
{
    void HUD::load()
    {
        

        m_window = Application::Get().getWindow();
        m_context = Application::Get().getContext();

        

    }

    void HUD::render(f32 deltaTime)
    {
        auto& app = Application::Get();
        u32 width = app.getWidth();
        u32 height = app.getHeight();

        auto& game = LocalGame::Get();
        auto& camera = game.getCamera();

        

        

        // render everything to window
        
    }

    

    HUD::~HUD()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }
}