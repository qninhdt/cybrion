#pragma once

#include "game.hpp"
#include "client/camera.hpp"
#include "client/resource/block_loader.hpp"
#include "client/graphic/block_renderer.hpp"
#include "client/key_code.hpp"
#include "client/stopwatch.hpp"
#include "client/graphic/world_renderer.hpp"

namespace cybrion
{
    constexpr u32 TICKS_PER_SECOND = 20;
    constexpr f32 GAME_TICK = 1000.0f / TICKS_PER_SECOND;
 
    class ClientGame : public Game
    {
    public:
        ClientGame();

        void load();
        void tick();
        void render(f32 deltaTime);

        void run();

        void onKeyPressed(KeyCode key, bool isRepeated);
        void onKeyReleased(KeyCode key);
        void onMouseMoved(const vec2& delta);
        void onWindowResized(u32 width, u32 height);

        Camera& getCamera();

        BlockRenderer& getBlockRenderer(u32 id);

        void toggleWireframe();

        static ClientGame& Get();

    private:

        void createBlockRenderers();

        Camera m_camera;
        
        Game* m_game;
        bool m_showWireframe;
        Stopwatch m_stopwatch;

        WorldRenderer m_worldRenderer;
        BlockRenderer m_blockRenderers[BlockRegistry::BlockStateCount()];

        static ClientGame* s_clientGame;
    };
}