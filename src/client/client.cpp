#include "client/client.hpp"
#include "client/GL/mesh.hpp"
#include "core/entity.hpp"
#include "world/chunk/chunk.hpp"
#include "client/graphic/block_renderer.hpp"
#include "world/block/nature/log_block.hpp"

namespace cybrion
{
    Client* Client::s_client = nullptr;

    Client::Client():
        m_rootPath(CYBRION_ROOT_PATH),
        m_debugScreen(m_window),   
        m_window(1600, 800, "Cybrion"),
        m_camera(m_window.getAspect(), glm::radians(50.0f), 0.2f, 1000.0f),
        m_showWireframe(false),
        m_game(nullptr)
    {
        s_client = this;
    }

    void Client::loadResources()
    {
        CYBRION_CLIENT_TRACE("Start loading resources ({})", getResourcePath(""));
        m_shaderManager.loadShaders();
    }

    void Client::init()
    {
        GL::Mesh::GenerateGlobalIBO();
        m_debugScreen.init();
    }

    void Client::start()
    {
        m_game = new Game();
        m_game->load();
        initBlockRenderers();

        using BasicShader = GL::Shader<"MVP">;
        auto shader = m_shaderManager.getShader<BasicShader>("basic");

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
        m_camera.updateViewMatrix();

        while (!m_window.isClosed())
        {
            m_window.tick(getDeltaTime());

            m_window.beginLoop();

            shader.use();
            shader.setUniform<"MVP">(
                m_camera.getProjectionViewMatrix()
            );

            mesh.drawTriangles();

            // render debug screen
            m_debugScreen.render();

            m_window.endLoop();

            m_frameProfiler.tick();
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

    BlockRenderer& Client::getBlockRenderer(u32 id)
    {
        return m_blockRenderers[id];
    }

    void Client::toggleWireframe()
    {
        m_showWireframe = !m_showWireframe;

        if (m_showWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    Client& Client::Get()
    {
        return *s_client;
    }

    void Client::initBlockRenderers()
    {
        for (u32 i = 0; i < GameBlockRegistry::BlockStateCount(); ++i)
        {
            Block& block = Game::Get().getBlockRegistry().getBlock(i);
            m_blockRenderers[block.getId()].m_block = &block;
        }
    }
}