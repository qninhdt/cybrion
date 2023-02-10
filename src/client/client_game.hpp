#pragma once

#include "game.hpp"
#include "client/camera.hpp"
#include "client/resource/block_loader.hpp"
#include "client/graphic/block_renderer.hpp"

namespace cybrion
{
    class ClientGame : public Game
    {
    public:
        ClientGame();

        void load();
        void tick();
        void render(f32 deltaTime);

        Camera& getCamera();

        BlockRenderer& getBlockRenderer(u32 id);

        void toggleWireframe();

        static ClientGame& Get();

    private:

        void createBlockRenderers();

        Camera m_camera;
        
        Game* m_game;
        bool m_showWireframe;

        BlockRenderer m_blockRenderers[BlockRegistry::BlockStateCount()];

        static ClientGame* s_clientGame;
    };
}