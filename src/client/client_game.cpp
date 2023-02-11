#include "client/client_game.hpp"

#include "client/application.hpp"
#include "client/GL/mesh.hpp"

namespace cybrion
{
    ClientGame* ClientGame::s_clientGame = nullptr;

    ClientGame::ClientGame():
        m_camera(Application::Get().getAspect(), glm::radians(50.0f), 0.2f, 1200.0f),
        m_showWireframe(false),
        m_game(nullptr),
        m_worldRenderer(getWorld())
    {
        s_clientGame = this;
    }

    void ClientGame::load()
    {
        Game::load();
        createBlockRenderers();

    }

    void ClientGame::tick()
    {
        Game::tick();
    }

    void ClientGame::render(f32 deltaTime)
    {
        m_worldRenderer.buildChunkMeshes(1); // allow build meshes in 1 second

        m_worldRenderer.render(deltaTime);
    }

    void ClientGame::run()
    {
        while (!Application::Get().isClosed())
        {
            m_stopwatch.reset();

            tick();

            auto sleepTime = std::chrono::milliseconds(
                u64(GAME_TICK - m_stopwatch.getDeltaTime() * 1000)
            );

            std::this_thread::sleep_for(sleepTime);
        }
    }

    Camera& ClientGame::getCamera()
    {
        return m_camera;
    }

    BlockRenderer& ClientGame::getBlockRenderer(u32 id)
    {
        return m_blockRenderers[id];
    }

    void ClientGame::toggleWireframe()
    {
        m_showWireframe = !m_showWireframe;

        if (m_showWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void ClientGame::onKeyPressed(KeyCode key, bool isRepeated)
    {
        if (!isRepeated)
        {
            switch (key)
            {

            // toggle cursor
            case KeyCode::F1:
                Application::Get().toggleCursor();
                break;

                // close window
            case KeyCode::ESCAPE:
                Application::Get().close();
                break;

                // reload shaders
            case KeyCode::R:
                Application::Get().getShaderManager().reloadShaders();
                break;

                // toggle wireframe
            case KeyCode::F2:
                ClientGame::Get().toggleWireframe();
                break;
            }
        }
    }

    void ClientGame::onKeyReleased(KeyCode key)
    {
    }

    void ClientGame::onMouseMoved(const vec2& delta)
    {
        if (!Application::Get().isCursorEnable())
        {
             m_camera.rotate(vec3(- delta.y, - delta.x, 0) * Application::Get().getDeltaTime() * 1.0f);
             auto r = m_camera.getRotation();

             if (r.x > pi / 2 - 0.001f && r.x < pi * 3 / 2 + 0.001f)
             {
                 if (r.x - pi / 2 - 0.001f < pi * 3 / 2 + 0.001f - r.x)
                     r.x = pi / 2 - 0.001f;
                 else
                     r.x = pi * 3 / 2 + 0.001f;
             }

             m_camera.setRotation(r);
             m_camera.updateViewMatrix();
        }
    }

    void ClientGame::onWindowResized(u32 width, u32 height)
    {
        if (width == 0 || height == 0) return;

        m_camera.setAspect(width * 1.0f / height);
        m_camera.updateProjectionMatrix();
    }

    ClientGame& ClientGame::Get()
    {
        return *s_clientGame;
    }

    void ClientGame::createBlockRenderers()
    {
        for (u32 i = 0; i < BlockRegistry::BlockStateCount(); ++i)
        {
            Block& block = Game::Get().getBlockRegistry().getBlock(i);
            m_blockRenderers[block.getId()].m_block = &block;
            m_blockRenderers[block.getId()].generateCubeTexture();
        }
    }
}