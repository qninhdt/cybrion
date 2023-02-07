#include "client/client.hpp"
#include "client/GL/mesh.hpp"

namespace cybrion
{
    Client* Client::s_client = nullptr;

    Client::Client():
        m_rootPath(CYBRION_ROOT_PATH),
        m_window(1600, 800, "Cybrion"),
        m_camera(m_window.getAspect(), glm::radians(50.0f), 0.2f, 1000.0f)
    {
        s_client = this;
    }

    void Client::loadResources()
    {
        CYBRION_CLIENT_TRACE("Start loading resources ({})", getResourcePath(""));
        m_shaderManager.loadShaders();
    }

    void Client::start()
    {
        using BasicShader = GL::Shader<"MVP">;
        auto shader = m_shaderManager.getShader<BasicShader>("basic");

        GL::Mesh mesh(true);

        mesh.setAttributes({
            { GL::Type::VEC3 }
        });

        mesh.setScale({ 2, 2, 2 });
        mesh.setPosition({ 0, 0, 2 });
        mesh.updateModelMatrix();

        f32 vertices[] = {
            +0.5f, -0.5f, 0,
            -0.5f, -0.5f, 0,
            -0.5f, +0.5f, 0,
            +0.5f, +0.5f, 0,
        };

        mesh.setVertices(vertices, 12);
        mesh.setDrawCount(6);

        while (!m_window.isClosed())
        {
            m_window.tick(getDeltaTime());

            m_window.beginLoop();

            shader.use();
            shader.setUniform<"MVP">(
                m_camera.getProjectionViewMatrix() *
                mesh.getModelMatrix()
            );

            mesh.drawTriangles();

            m_window.endLoop();

            m_frameProfiler.tick();
            std::cout << m_frameProfiler.getDeltaTime() << '\n';
        }
    }

    Camera& Client::getCamera()
    {
        return m_camera;
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

    f32 Client::getFPS() const
    {
        return m_frameProfiler.getFPS();
    }

    f32 Client::getDeltaTime() const
    {
        return m_frameProfiler.getDeltaTime();
    }

    Client& Client::Get()
    {
        return *s_client;
    }
}