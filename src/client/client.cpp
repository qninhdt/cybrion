#include "client/client.hpp"

namespace cybrion
{
    Client* Client::s_client = nullptr;

    Client::Client():
        m_rootPath(CYBRION_ROOT_PATH),
        m_window(800, 600, "Cybrion")
    {
        s_client = this;
    }

    void Client::loadResources()
    {
        CYBRION_CLIENT_TRACE("Start loading resources ({})", getResourcePath(""));

        m_shaderManager.loadShaders();
    }

    Window& Client::getWindow()
    {
        return m_window;
    }

    ShaderManager& Client::getShaderManager()
    {
        return m_shaderManager;
    }

    string Client::getResourcePath(const string& path) const
    {
        return m_rootPath + "resources/" + path;
    }

    Client& Client::Get()
    {
        return *s_client;
    }
}