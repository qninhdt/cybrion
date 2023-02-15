#pragma once

#include "client/resource/block_loader.hpp"
#include "world/world.hpp"
#include "player.hpp"

namespace cybrion
{
    class Game
    {
    public:

        Game();

        void load();
        void tick();

        World& getWorld();
        Player& getPlayer();

        virtual void onChunkLoaded(const ref<Chunk>& chunk) = 0;
        virtual void onChunkUnloaded(const ref<Chunk>& chunk) = 0;
        virtual void onEntitySpawned(const ref<Entity>& entity) = 0;
        virtual void onBlockChanged(const BlockModifyResult& result) = 0;
        virtual void onBreakBlock(const BlockModifyResult& result) = 0;
        virtual void onPlaceBlock(const BlockModifyResult& result) = 0;

        static Game& Get();

    protected:
        static Game* s_game;

        BlockLoader m_blockLoader;
        World m_world;
        Player m_player;
    };
}
