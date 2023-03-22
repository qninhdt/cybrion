#pragma once

#include "world/block/block.hpp"

#include "world/block/nature/soil_block.hpp"
#include "world/block/nature/air_block.hpp"
#include "world/block/nature/log_block.hpp"
#include "world/block/nature/sand_block.hpp"
#include "world/block/nature/rock_block.hpp"
#include "world/block/nature/leaf_block.hpp"
#include "world/block/nature/water_block.hpp"
#include "world/block/nature/plant_block.hpp"

#include "world/block/building/brick_block.hpp"
#include "world/block/building/fence_block.hpp"
#include "world/block/building/fence_gate_block.hpp"

#include "world/block/decoration/tablecloth_block.hpp"
#include "world/block/decoration/chess_block.hpp"

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
        FenceBlock,
        FenceGateBlock,
        PlantBlock,
        TableclothBlock,
        ChessBlock
    >
    {
    public:

        static AirBlock& AIR;
        static SoilBlock& DIRT;
        static SoilBlock& GRASS_BLOCK;
        static LogBlock& OAK_LOG;
        static RockBlock& COBBLESTONE;
        static RockBlock& STONE;
        static SandBlock& SAND;
        static LeafBlock& OAK_LEAF;
        static WaterBlock& WATER;
        static PlantBlock& GRASS;
        static PlantBlock& FERN;
        static PlantBlock& DANDELION;
        static PlantBlock& POPPY;
        static PlantBlock& BLUE_ORCHID;
        static PlantBlock& ALLIUM;
        static PlantBlock& AZURE;
        static PlantBlock& RED_TULIP;
        static PlantBlock& WHITE_TULIP;
        static PlantBlock& PINK_TULIP;
        static PlantBlock& OXEYE_DAISY;
        static PlantBlock& CORNFLOWER;
        static PlantBlock& LILY_OF_THE_VALLEY;

        static BrickBlock& STONE_BRICK;
        static FenceBlock& OAK_FENCE;
        static FenceGateBlock& OAK_FENCE_GATE;

        static TableclothBlock& TABLECLOTH;
        static ChessBlock& WHITE_KING;
        static ChessBlock& WHITE_QUEEN;
        static ChessBlock& WHITE_ROOK;
        static ChessBlock& WHITE_BISHOP;
        static ChessBlock& WHITE_KNIGHT;
        static ChessBlock& WHITE_PAWN;
        static ChessBlock& BLACK_KING;
        static ChessBlock& BLACK_QUEEN;
        static ChessBlock& BLACK_ROOK;
        static ChessBlock& BLACK_BISHOP;
        static ChessBlock& BLACK_KNIGHT;
        static ChessBlock& BLACK_PAWN;

        static Blocks& Get();
    private:
        static RockBlock& ROCK;
        static SoilBlock& SOIL;
        static LogBlock& LOG;
        static BrickBlock& BRICK;
        static LeafBlock& LEAF;
        static FenceBlock& FENCE;
        static FenceGateBlock& FENCE_GATE;
        static PlantBlock& PLANT;
        static ChessBlock& CHESS;

        static Blocks s_blocks;
    };
}

#include "world/block/block_state.inl"