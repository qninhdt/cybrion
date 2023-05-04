#include "client/local_game.hpp"

#include "client/application.hpp"
#include "client/GL/mesh.hpp"
#include "client/graphic/basic_mesh_generator.hpp"
#include "client/ui/game_page.hpp"

namespace cybrion
{
    LocalGame *LocalGame::s_LocalGame = nullptr;

    LocalGame::LocalGame(const string &worldPath) : Game(worldPath),
                                                    m_camera(Application::Get().getAspect(), glm::radians(50.0f), 0.0001f, 1500.0f),
                                                    m_showWireframe(false),
                                                    m_showEntityBorder(false),
                                                    m_showChunkBoder(false),
                                                    m_showDebugUI(false),
                                                    m_showUI(true),
                                                    m_worldRenderer(getWorld())
    {
        s_LocalGame = this;
    }

    void LocalGame::load()
    {
        Game::load();
        createBlockRenderers();

        m_basicShader = ShaderManager::Get().getShader<BasicShader>("basic");

        BasicMeshGenerator::LineCubeMesh(m_chunkBorderMesh, Chunk::CHUNK_SIZE, {1, 0, 0});
        BasicMeshGenerator::LineCubeMesh(m_targetBlockMesh, 1.00f, {1, 1, 1});

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
        m_chunkBorderMesh.updateModelMat();

        m_basicShader.use();
        m_basicShader.setUniform<"MVP">(
            m_camera.getProjViewMat() *
            m_chunkBorderMesh.getModelMat());

        m_chunkBorderMesh.drawLines();
    }

    void LocalGame::renderSelecingBlock()
    {
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(2.0);

        if (m_player.getTargetBlock() != nullptr)
        {
            ivec3 pos = m_player.getTargetPos();
            Block &block = m_world->getBlock(pos);
            AABB bound = block.getBound();

            m_targetBlockMesh.setPos(vec3(pos) + bound.getPos() + vec3(0.5f, 0.5f, 0.5f));
            // std::cout << bound.getSize().x << ' ' << bound.getSize().y << ' ' << bound.getSize().z << '\n';
            m_targetBlockMesh.setScale(bound.getSize());
            m_targetBlockMesh.updateModelMat();

            m_basicShader.use();
            m_basicShader.setUniform<"MVP">(
                LocalGame::Get().getCamera().getProjViewMat() * m_targetBlockMesh.getModelMat());

            m_targetBlockMesh.drawLines();
        }
    }

    void LocalGame::onChunkLoaded(const ref<Chunk> &chunk)
    {
        m_worldRenderer.addChunk(chunk);
    }

    void LocalGame::onChunkUnloaded(const ref<Chunk> &chunk)
    {
        m_worldRenderer.removeChunk(chunk);
    }

    void LocalGame::onEntitySpawned(const ref<Entity> &entity)
    {
        m_worldRenderer.addEntity(entity);
    }

    void LocalGame::onChunkChanged(const ref<Chunk> &chunk)
    {
        m_worldRenderer.updateChunk(chunk);
    }

    void LocalGame::onPlaceBlock(const BlockModifyResult &result)
    {
        Application::Get().playSound("blocks/" + result.block.getSound());
    }

    void LocalGame::onPlaySound(const string &name)
    {
        Application::Get().playSound(name);
    }

    void LocalGame::onBreakBlock(const BlockModifyResult &result)
    {
        Application::Get().playSound("blocks/" + result.oldBlock.getSound());
    }

    Camera &LocalGame::getCamera()
    {
        return m_camera;
    }

    BlockRenderer &LocalGame::getBlockRenderer(u32 id)
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

    Player &LocalGame::getPlayer()
    {
        return m_player;
    }

    void LocalGame::onKeyPressed(SDL_Scancode key, bool isRepeated)
    {
        if (!isRepeated)
        {
            switch (key)
            {
            // toggle cursor
            case SDL_SCANCODE_F1:
                Application::Get().toggleCursor();
                break;

            case SDL_SCANCODE_F2:
                m_showUI = !m_showUI;
                break;

            case SDL_SCANCODE_F3:
                m_showDebugUI = !m_showDebugUI;
                break;

            // close window
            case SDL_SCANCODE_ESCAPE:
                Application::Get().close();
                break;
                // toggle block menu
            case SDL_SCANCODE_E:
                auto page = (ui::GamePage *)Application::Get().getCurrentPage().get();

                page->showBlockMenu = !page->showBlockMenu;

                if (page->showBlockMenu)
                    Application::Get().enableCursor();
                else
                    Application::Get().disableCursor();

                break;
            }
        }
    }

    void LocalGame::onKeyReleased(SDL_Scancode key)
    {
    }

    void LocalGame::onMouseMoved(const vec2 &delta)
    {
        if (!Application::Get().isCursorEnable())
        {
            vec3 rot = m_camera.getRot() + vec3(-delta.y, -delta.x, 0) * Application::Get().getDeltaTime() * 0.2f;

            f32 eps = 0.01f;
            if (rot.x > pi / 2 - eps && rot.x < pi * 3 / 2 + eps)
            {
                if (rot.x - pi / 2 - eps < pi * 3 / 2 + eps - rot.x)
                    rot.x = pi / 2 - eps;
                else
                    rot.x = pi * 3 / 2 + eps;
            }

            m_camera.setRot(rot);

            m_camera.updateViewMat();
            m_player.getInput().rot = m_camera.getRot();
        }
    }

    void LocalGame::onWindowResized(u32 width, u32 height)
    {
        if (width == 0 || height == 0)
            return;

        m_camera.setAspect(width * 1.0f / height);
        m_camera.updateProjMat();
    }

    void LocalGame::onMouseScrolled(f32 delta)
    {
        m_player.getInput().scroll = delta;
    }

    LocalGame &LocalGame::Get()
    {
        return *s_LocalGame;
    }

    void LocalGame::createBlockRenderers()
    {
        for (u32 i = 0; i < Blocks::StateCount(); ++i)
        {
            Block &block = Blocks::Get().getBlock(i);
            m_blockRenderers[block.getId()].m_block = &block;
            m_blockRenderers[block.getId()].generateCubeTexture();
        }
    }
}