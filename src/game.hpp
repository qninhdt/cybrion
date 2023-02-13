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

        BlockRegistry& getBlockRegistry();
        BlockLoader& getBlockLoader();

        World& getWorld();

        void loadPlayer(Player& player);

        virtual void onChunkLoaded(Object chunk) = 0;
        virtual void onChunkUnloaded(Object chunk) = 0;
        virtual void onEntitySpawned(Object entity) = 0;
        virtual void onBlockChanged(Object chunk, const ivec3& pos, Block& to, Block& from) = 0;
        virtual void onBreakBlock(Object chunk, const ivec3& pos, Block& block) = 0;
        virtual void onPlaceBlock(Object chunk, const ivec3& pos, Block& block, BlockFace face) = 0;

        static Game& Get();

    private:
        static Game* s_game;

        BlockLoader m_blockLoader;
        BlockRegistry m_blockRegistry;
        World m_world;

        Player* m_player;
    };
}

#include "world/block/block_state.inl"