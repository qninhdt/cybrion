#include "client/local_game.hpp"

#include "client/application.hpp"
#include "client/GL/mesh.hpp"
#include "client/graphic/basic_mesh_generator.hpp"

namespace cybrion
{
    LocalGame* LocalGame::s_LocalGame = nullptr;

    LocalGame::LocalGame():
        m_camera(Application::Get().getAspect(), glm::radians(50.0f), 0.2f, 1200.0f),
        m_showWireframe(false),
        m_showEntityBorder(false),
        m_showChunkBoder(false),
        m_worldRenderer(getWorld())
    {
        s_LocalGame = this;
    }

    void LocalGame::load()
    {
        Game::load();
        createBlockRenderers();

        m_camera.setTarget(getPlayer().getEntity());

        m_basicShader = ShaderManager::Get().getShader<BasicShader>("basic");

        BasicMeshGenerator::LineCubeMesh(m_chunkBorderMesh, Chunk::CHUNK_SIZE, { 1, 0, 0 });
        BasicMeshGenerator::LineCubeMesh(m_targetBlockMesh, 1.005f, { 1, 1, 1 });

        m_player.setEntity(m_world.spawnEntity({ 16, 100, 16 }));
        m_camera.setTarget(m_player.getEntity());
    }

    void LocalGame::tick()
    {
        Game::tick();
    }

    void LocalGame::render(f32 delta)
    {
        m_camera.tick(delta);

        m_worldRenderer.render(delta, m_showEntityBorder);

        if (m_showChunkBoder)
            renderChunkBorder();
        
        renderSelecingBlock();
    }

    void LocalGame::renderChunkBorder()
    {
        m_chunkBorderMesh.setPos({ 16, 16, 16 }
            //m_player.getEntity().get<EntityData>().getChunkWorldPos()
        );

        m_chunkBorderMesh.updateModelMat();

        m_basicShader.use();
        m_basicShader.setUniform<"MVP">(
            m_camera.getProjViewMat() *
            m_chunkBorderMesh.getModelMat()
        );

        m_chunkBorderMesh.drawLines();
    }

    void LocalGame::renderSelecingBlock()
    {
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(2.0);

        if (m_player.getTargetBlock() != nullptr)
        {
            ivec3 pos = m_player.getTargetPos();

            m_targetBlockMesh.setPos({ pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f });
            m_targetBlockMesh.updateModelMat();

            m_basicShader.use();
            m_basicShader.setUniform<"MVP">(
                m_camera.getProjViewMat() *
                m_targetBlockMesh.getModelMat()
            );

            m_targetBlockMesh.drawLines();
        }
    }

    void LocalGame::onChunkLoaded(const ref<Chunk>& chunk)
    {
        m_worldRenderer.addChunk(chunk);
    }

    void LocalGame::onChunkUnloaded(const ref<Chunk>& chunk)
    {
        m_worldRenderer.removeChunk(chunk);
    }

    void LocalGame::onEntitySpawned(const ref<Entity>& entity)
    {
        m_worldRenderer.addEntity(entity);
    }

    void LocalGame::onBlockChanged(const BlockModifyResult& result)
    {
        m_worldRenderer.updateBlock(result);
    }

    void LocalGame::onPlaceBlock(const BlockModifyResult& result)
    {
        CYBRION_GAME_TRACE("Player places block {} at ({}, {}, {})", result.block.toString(), result.pos.x, result.pos.y, result.pos.z);
    }

    void LocalGame::onBreakBlock(const BlockModifyResult& result)
    {
        CYBRION_GAME_TRACE("Player breaks block {}", result.block.toString());
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

    void LocalGame::toggleChunkBorder()
    {
        m_showChunkBoder = !m_showChunkBoder;
    }

    bool LocalGame::showChunkBorder() const
    {
        return m_showChunkBoder;
    }

    Player& LocalGame::getPlayer()
    {
        return m_player;
    }

    void LocalGame::onKeyPressed(KeyCode key, bool isRepeated)
    {
        static int n = 7;
        if (!isRepeated)
        {
            switch (key)
            {
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
            input.deltaRot += vec3(-delta.y, -delta.x, 0);
        }
    }

    void LocalGame::onWindowResized(u32 width, u32 height)
    {
        if (width == 0 || height == 0) return;

        m_camera.setAspect(width * 1.0f / height);
        m_camera.updateProjMat();
    }

    LocalGame& LocalGame::Get()
    {
        return *s_LocalGame;
    }

    void LocalGame::createBlockRenderers()
    {
        for (u32 i = 0; i < Blocks::StateCount(); ++i)
        {
            Block& block = Blocks::Get().getBlock(i);
            m_blockRenderers[block.getId()].m_block = &block;
            m_blockRenderers[block.getId()].generateCubeTexture();
        }
    }
}