#pragma once

#include "client/resource/block_loader.hpp"
#include "world/world.hpp"

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

        static Game& Get();

    private:
        static Game* s_game;

        BlockLoader m_blockLoader;
        BlockRegistry m_blockRegistry;
        World m_world;
    };
}

#include "world/block/block_state.inl"