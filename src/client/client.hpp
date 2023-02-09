#pragma once

#include "client/window.hpp"
#include "client/camera.hpp"
#include "client/frame_profiler.hpp"
#include "client/resource/shader_manager.hpp"
#include "client/debug_screen.hpp"
#include "client/resource/block_loader.hpp"
#include "game.hpp"

namespace cybrion
{
    class Client
    {
    public:
        Client();

        void loadResources();

        void init();

        void start();

        Camera& getCamera();
        Window& getWindow();
        ShaderManager& getShaderManager();
        string getResourcePath(const string& path) const;
        
        f32 getFPS() const;
        f32 getDeltaTime() const;

        void toggleWireframe();

        static Client& Get();

    private:
        string m_rootPath;

        FrameProfiler m_frameProfiler;
        DebugScreen m_debugScreen;
        Window m_window;
        Camera m_camera;
        ShaderManager m_shaderManager;
        Game* m_game;
        bool m_showWireframe;

        static Client* s_client;
    };
}