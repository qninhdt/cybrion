#include "client/client_game.hpp"

#include "client/application.hpp"
#include "client/GL/mesh.hpp"

namespace cybrion
{
    ClientGame* ClientGame::s_clientGame = nullptr;

    ClientGame::ClientGame():
        m_camera(Application::Get().getAspect(), glm::radians(50.0f), 0.2f, 1000.0f),
        m_showWireframe(false),
        m_game(nullptr)
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

       /* using BasicShader = GL::Shader<"MVP">;
        auto shader = Application::Get().getShaderManager().getShader<BasicShader>("basic");

        GL::Mesh mesh(true);

        mesh.setAttributes({
            { GL::Type::VEC3 },
            { GL::Type::VEC2 },
            { GL::Type::UINT }
        });

        u32 size = 0;
        CubeVertex data[1000];

        LogBlock& log = (LogBlock&)Game::Get().getBlockRegistry().getBlock(BlockType::LOG);
        log = log.set<"type">(WoodType::OAK);
        log = log.set<"axis">(LogAxis::Y);

        auto& renderer = getBlockRenderer(log.getId());
        renderer.generateCubeTexture();

        bool cull[6] = { 0 };
        renderer.generateCubeMesh(cull, data, size);

        mesh.setVertices(data, 24);
        mesh.setDrawCount(36);

        Game::Get().getBlockLoader().bindTextureArray();

        m_camera.setPosition({ 0, 1, 2 });
        m_camera.setDirection({ 0, 0, -1 });
        m_camera.updateViewMatrix();*/

        
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
        }
    }
}