#pragma once

#include "client/window.hpp"
#include "client/resource/shader_manager.hpp"

namespace cybrion
{
    class Client
    {
    public:
        Client();

        void loadResources();

        void start();

        Window& getWindow();
        ShaderManager& getShaderManager();
        string getResourcePath(const string& path) const;

        static Client& Get();

    private:
        string m_rootPath;

        Window m_window;
        ShaderManager m_shaderManager;

        static Client* s_client;
    };
}