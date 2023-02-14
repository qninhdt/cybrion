#pragma once

#include "game.hpp"
#include "client/camera.hpp"
#include "client/resource/block_loader.hpp"
#include "client/graphic/block_renderer.hpp"
#include "client/key_code.hpp"
#include "core/stopwatch.hpp"
#include "client/graphic/world_renderer.hpp"
#include "client/local_player.hpp"

namespace cybrion
{
    constexpr u32 TICKS_PER_SECOND = 20;
    constexpr f32 GAME_TICK = 1000.0f / TICKS_PER_SECOND;
 
    using BasicShader = GL::Shader<"MVP">;

    class LocalGame : public Game
    {
    public:
        LocalGame();

        void load();
        void tick();

        void render(f32 lerpFactor);
        void renderChunkBorder();
        void renderSelecingBlock();

        void onChunkLoaded(Object chunk) override;
        void onChunkUnloaded(Object chunk) override;
        void onEntitySpawned(Object entity) override;
        void onBlockChanged(Object chunk, const ivec3& pos, Block& to, Block& from) override;
        void onPlaceBlock(Object chunk, const ivec3& pos, Block& block, BlockFace face) override;
        void onBreakBlock(Object chunk, const ivec3& pos, Block& block) override;

        void onKeyPressed(KeyCode key, bool isRepeated);
        void onKeyReleased(KeyCode key);
        void onMouseMoved(const vec2& delta);
        void onWindowResized(u32 width, u32 height);

        Camera& getCamera();

        BlockRenderer& getBlockRenderer(u32 id);

        void toggleWireframe();
        void toggleChunkBorder();
        bool showChunkBorder() const;

        Player& getPlayer();

        static LocalGame& Get();

    private:

        friend class HUD;

        void createBlockRenderers();

        Camera m_camera;
        LocalPlayer m_player;
        
        bool m_showWireframe;
        bool m_showChunkBoder;
        bool m_showEntityBorder;
        Stopwatch m_stopwatch;

        BasicShader m_basicShader;
        GL::Mesh m_chunkBorderMesh;
        GL::Mesh m_selectingBlockMesh;

        WorldRenderer m_worldRenderer;
        BlockRenderer m_blockRenderers[BlockRegistry::BlockStateCount()];

        static LocalGame* s_LocalGame;
    };
}