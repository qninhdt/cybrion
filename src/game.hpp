#pragma once

#include "world/chunk/chunk.hpp"
#include "client/resource/block_loader.hpp"
#include "world/block/nature/soil_block.hpp"
#include "world/block/nature/air_block.hpp"
#include "world/block/nature/log_block.hpp"

namespace cybrion
{
    using GameBlockRegistry = BlockRegistry<AirBlock, SoilBlock, LogBlock>;

    class Game
    {
    public:

        Game();

        void load();

        GameBlockRegistry& getBlockRegistry();
        BlockLoader& getBlockLoader();

        static Game& Get();

    private:
        static Game* s_game;

        BlockLoader m_blockLoader;
        GameBlockRegistry m_blockRegistry;
    };
}

#include "world/block/block_state.inl"