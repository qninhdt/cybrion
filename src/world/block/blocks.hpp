#pragma once

#include "world/block/block.hpp"

#include "world/block/nature/soil_block.hpp"
#include "world/block/nature/air_block.hpp"
#include "world/block/nature/log_block.hpp"
#include "world/block/nature/sand_block.hpp"
#include "world/block/nature/rock_block.hpp"
#include "world/block/nature/brick_block.hpp"

namespace cybrion
{
    // REGISTER NEW BLOCK HERE
    //           |
    //           |
    //           V
    class Blocks : public detail::BaseBlocks<
        AirBlock,
        SoilBlock,
        LogBlock,
        SandBlock,
        RockBlock,
        BrickBlock
    >
    {
    public:

        static AirBlock& AIR;
        static SoilBlock& DIRT;
        static SoilBlock& GRASS;
        static LogBlock& OAK_LOG;
        static RockBlock& COBBLESTONE;
        static RockBlock& STONE;
        static BrickBlock& STONE_BRICK;

        static Blocks& Get();
    private:
        static RockBlock& ROCK;
        static SoilBlock& SOIL;
        static LogBlock& LOG;
        static BrickBlock& BRICK;

        static Blocks s_blocks;
    };
}

#include "world/block/block_state.inl"