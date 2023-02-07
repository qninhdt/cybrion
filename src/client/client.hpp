#pragma once

#include "client/window.hpp"
#include "client/camera.hpp"
#include "client/frame_profiler.hpp"
#include "client/resource/shader_manager.hpp"

namespace cybrion
{
    class Client
    {
    public:
        Client();

        void loadResources();

        void start();

        Camera& getCamera();
        Window& getWindow();
        ShaderManager& getShaderManager();
        string getResourcePath(const string& path) const;
        
        f32 getFPS() const;
        f32 getDeltaTime() const;

        static Client& Get();

    private:
        string m_rootPath;

        FrameProfiler m_frameProfiler;
        Window m_window;
        Camera m_camera;
        ShaderManager m_shaderManager;

        static Client* s_client;
    };
}