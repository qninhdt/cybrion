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
#include "world/block/building/wool_block.hpp"
#include "world/block/building/concrete_block.hpp"
#include "world/block/building/mineral_block.hpp"

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
        ChessBlock,
        WoolBlock,
        ConcreteBlock,
        MineralBlock
    >
    {
    public:

        static AirBlock& AIR;
        static SoilBlock& DIRT;
        static SoilBlock& CLAY;
        static SoilBlock& GRASS_BLOCK;
        static LogBlock& ACACIA_LOG;
        static LogBlock& BIRCH_LOG;
        static LogBlock& DARK_OAK_LOG;
        static LogBlock& JUNGLE_LOG;
        static LogBlock& MANGROVE_LOG;
        static LogBlock& OAK_LOG;
        static LogBlock& SPRUCE_LOG;
        static RockBlock& COBBLESTONE;
        static RockBlock& STONE;
        static SandBlock& SAND;
        static LeafBlock& ACACIA_LEAF;
        static LeafBlock& BIRCH_LEAF;
        static LeafBlock& DARK_OAK_LEAF;
        static LeafBlock& JUNGLE_LEAF;
        static LeafBlock& MANGROVE_LEAF;
        static LeafBlock& OAK_LEAF;
        static LeafBlock& SPRUCE_LEAF;
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
        static BrickBlock& MOSSY_STONE_BRICK;
        static BrickBlock& CLAY_BRICK;
        static BrickBlock& QUARTZ_BRICK;

        static FenceBlock& ACACIA_FENCE;
        static FenceBlock& BIRCH_FENCE;
        static FenceBlock& DARK_OAK_FENCE;
        static FenceBlock& JUNGLE_FENCE;
        static FenceBlock& MANGROVE_FENCE;
        static FenceBlock& OAK_FENCE;
        static FenceBlock& SPRUCE_FENCE;

        static FenceGateBlock& ACACIA_FENCE_GATE;
        static FenceGateBlock& BIRCH_FENCE_GATE;
        static FenceGateBlock& DARK_OAK_FENCE_GATE;
        static FenceGateBlock& JUNGLE_FENCE_GATE;
        static FenceGateBlock& MANGROVE_FENCE_GATE;
        static FenceGateBlock& OAK_FENCE_GATE;
        static FenceGateBlock& SPRUCE_FENCE_GATE;

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

        static WoolBlock& YELLOW_WOOL;
        static WoolBlock& WHITE_WOOL;
        static WoolBlock& RED_WOOL;
        static WoolBlock& PURPLE_WOOL;
        static WoolBlock& PINK_WOOL;
        static WoolBlock& ORANGE_WOOL;
        static WoolBlock& MAGENTA_WOOL;
        static WoolBlock& LIME_WOOL;
        static WoolBlock& LIGHT_GRAY_WOOL;
        static WoolBlock& LIGHT_BLUE_WOOL;
        static WoolBlock& GREEN_WOOL;
        static WoolBlock& GRAY_WOOL;
        static WoolBlock& CYAN_WOOL;
        static WoolBlock& BROWN_WOOL;
        static WoolBlock& BLUE_WOOL;
        static WoolBlock& BLACK_WOOL;

        static ConcreteBlock& YELLOW_CONCRETE;
        static ConcreteBlock& WHITE_CONCRETE;
        static ConcreteBlock& RED_CONCRETE;
        static ConcreteBlock& PURPLE_CONCRETE;
        static ConcreteBlock& PINK_CONCRETE;
        static ConcreteBlock& ORANGE_CONCRETE;
        static ConcreteBlock& MAGENTA_CONCRETE;
        static ConcreteBlock& LIME_CONCRETE;
        static ConcreteBlock& LIGHT_GRAY_CONCRETE;
        static ConcreteBlock& LIGHT_BLUE_CONCRETE;
        static ConcreteBlock& GREEN_CONCRETE;
        static ConcreteBlock& GRAY_CONCRETE;
        static ConcreteBlock& CYAN_CONCRETE;
        static ConcreteBlock& BROWN_CONCRETE;
        static ConcreteBlock& BLUE_CONCRETE;
        static ConcreteBlock& BLACK_CONCRETE;

        static MineralBlock& COPPER_BLOCK;
        static MineralBlock& IRON_BLOCK;
        static MineralBlock& LAPIS_BLOCK;
        static MineralBlock& EMERALD_BLOCK;
        static MineralBlock& GOLD_BLOCK;
        static MineralBlock& DIAMOND_BLOCK;

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
        static WoolBlock& WOOL;
        static ConcreteBlock& CONCRETE;
        static MineralBlock& MINERAL;

        static Blocks s_blocks;
    };
}

#include "world/block/block_state.inl"