#include "client/local_game.hpp"

#include "client/application.hpp"
#include "client/GL/mesh.hpp"

namespace cybrion
{
    LocalGame* LocalGame::s_LocalGame = nullptr;

    LocalGame::LocalGame():
        m_camera(Application::Get().getAspect(), glm::radians(50.0f), 0.2f, 1200.0f),
        m_showWireframe(false),
        m_worldRenderer(getWorld())
    {
        s_LocalGame = this;
    }

    void LocalGame::load()
    {
        Game::load();
        createBlockRenderers();

        loadPlayer(m_player);
        m_camera.setTarget(m_player.getEntity());
    }

    void LocalGame::tick()
    {
        Game::tick();
    }

    void LocalGame::render(f32 lerpFactor)
    {

        m_worldRenderer.updateEntityTransforms(lerpFactor);
        m_camera.tick();

        m_worldRenderer.buildChunkMeshes(1); // allow build meshes in 1 second
        m_worldRenderer.rebuildChunkMeshes(1); // allow rebuild meshes in 1 second

        m_worldRenderer.render(lerpFactor);
    }

    void LocalGame::onChunkLoaded(Object chunk)
    {
        m_worldRenderer.setupChunk(chunk);
    }

    void LocalGame::onChunkUnloaded(Object chunk)
    {
    }

    void LocalGame::onEntitySpawned(Object entity)
    {
        m_worldRenderer.setupEntity(entity);
    }

    Camera& LocalGame::getCamera()
    {
        return m_camera;
    }

    BlockRenderer& LocalGame::getBlockRenderer(u32 id)
    {
        return m_blockRenderers[id];
    }

    void LocalGame::toggleWireframe()
    {
        m_showWireframe = !m_showWireframe;

        if (m_showWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    Player& LocalGame::getPlayer()
    {
        return m_player;
    }

    void LocalGame::onKeyPressed(KeyCode key, bool isRepeated)
    {
        static int n = 1;
        if (!isRepeated)
        {
            switch (key)
            {

            case KeyCode::B:
                m_worldRenderer.rebuildChunkMeshes(1);
                break;

            case KeyCode::N:
                getWorld().loadChunk({ n++, 0, 0 });
                break;

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
                ShaderManager::Get().reloadShaders();
                break;

                // toggle wireframe
            case KeyCode::F2:
                LocalGame::Get().toggleWireframe();
                break;
            }
        }
    }

    void LocalGame::onKeyReleased(KeyCode key)
    {
    }

    void LocalGame::onMouseMoved(const vec2& delta)
    {
        if (!Application::Get().isCursorEnable())
        {
            auto& input = m_player.getInput();
            input.deltaRotation += vec3(-delta.y, -delta.x, 0) * Application::Get().getDeltaTime() * 1.0f;
            
        }
    }

    void LocalGame::onWindowResized(u32 width, u32 height)
    {
        if (width == 0 || height == 0) return;

        m_camera.setAspect(width * 1.0f / height);
        m_camera.updateProjectionMatrix();
    }

    LocalGame& LocalGame::Get()
    {
        return *s_LocalGame;
    }

    void LocalGame::createBlockRenderers()
    {
        for (u32 i = 0; i < BlockRegistry::BlockStateCount(); ++i)
        {
            Block& block = BlockRegistry::Get().getBlock(i);
            m_blockRenderers[block.getId()].m_block = &block;
            m_blockRenderers[block.getId()].generateCubeTexture();
        }
    }
}