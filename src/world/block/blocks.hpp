#pragma once

#include "world/block/block.hpp"

#include "world/block/nature/soil_block.hpp"
#include "world/block/nature/air_block.hpp"
#include "world/block/nature/log_block.hpp"
#include "world/block/nature/sand_block.hpp"
#include "world/block/nature/rock_block.hpp"
#include "world/block/nature/leaf_block.hpp"
#include "world/block/nature/water_block.hpp"

#include "world/block/building/brick_block.hpp"
#include "world/block/building/fence_block.hpp"

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
        BrickBlock,
        LeafBlock,
        WaterBlock,
        FenceBlock
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
        static SandBlock& SAND;
        static LeafBlock& OAK_LEAF;
        static WaterBlock& WATER;
        static FenceBlock& OAK_FENCE;

        static Blocks& Get();
    private:
        static RockBlock& ROCK;
        static SoilBlock& SOIL;
        static LogBlock& LOG;
        static BrickBlock& BRICK;
        static LeafBlock& LEAF;
        static FenceBlock& FENCE;

        static Blocks s_blocks;
    };
}

#include "world/block/block_state.inl"