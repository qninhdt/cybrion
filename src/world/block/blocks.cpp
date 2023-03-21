#include "world/block/blocks.hpp"

namespace cybrion
{
    Blocks Blocks::s_blocks;

    AirBlock& Blocks::AIR = s_blocks.get<AirBlock>();

    SoilBlock& Blocks::SOIL        = s_blocks.get<SoilBlock>();
    SoilBlock& Blocks::GRASS_BLOCK = SOIL.set<"type">(SoilType::GRASS);
    SoilBlock& Blocks::DIRT        = SOIL.set<"type">(SoilType::DIRT);

    LogBlock& Blocks::LOG     = s_blocks.get<LogBlock>();
    LogBlock& Blocks::OAK_LOG = LOG.set<"type">(WoodType::OAK);

    RockBlock& Blocks::ROCK        = s_blocks.get<RockBlock>();
    RockBlock& Blocks::STONE       = ROCK.set<"type">(RockType::STONE);
    RockBlock& Blocks::COBBLESTONE = ROCK.set<"type">(RockType::COBBLESTONE);

    BrickBlock& Blocks::BRICK       = s_blocks.get<BrickBlock>();
    BrickBlock& Blocks::STONE_BRICK = BRICK.set<"type">(BrickType::STONE);

    SandBlock& Blocks::SAND = s_blocks.get<SandBlock>();
    
    LeafBlock& Blocks::LEAF     = s_blocks.get<LeafBlock>();
    LeafBlock& Blocks::OAK_LEAF = LEAF.set<"type">(WoodType::OAK);

    WaterBlock& Blocks::WATER = s_blocks.get<WaterBlock>();

    FenceBlock& Blocks::FENCE     = s_blocks.get<FenceBlock>().set<"east">(0).set<"north">(0).set<"west">(0).set<"south">(0);
    FenceBlock& Blocks::OAK_FENCE = FENCE.set<"type">(WoodType::OAK);

    PlantBlock& Blocks::PLANT       = s_blocks.get<PlantBlock>();
    PlantBlock& Blocks::GRASS       = PLANT.set<"type">(PlantType::GRASS);
    PlantBlock& Blocks::FERN        = PLANT.set<"type">(PlantType::FERN);
    PlantBlock& Blocks::DANDELION   = PLANT.set<"type">(PlantType::DANDELION);
    PlantBlock& Blocks::POPPY       = PLANT.set<"type">(PlantType::POPPY);
    PlantBlock& Blocks::BLUE_ORCHID = PLANT.set<"type">(PlantType::BLUE_ORCHID);
    PlantBlock& Blocks::ALLIUM      = PLANT.set<"type">(PlantType::ALLIUM);
    PlantBlock& Blocks::AZURE       = PLANT.set<"type">(PlantType::AZURE);
    PlantBlock& Blocks::RED_TULIP   = PLANT.set<"type">(PlantType::RED_TULIP);
    PlantBlock& Blocks::WHITE_TULIP = PLANT.set<"type">(PlantType::WHITE_TULIP);
    PlantBlock& Blocks::PINK_TULIP  = PLANT.set<"type">(PlantType::PINK_TULIP);
    PlantBlock& Blocks::OXEYE_DAISY = PLANT.set<"type">(PlantType::OXEYE_DAISY);
    PlantBlock& Blocks::CORNFLOWER  = PLANT.set<"type">(PlantType::CORNFLOWER);
    PlantBlock& Blocks::LILY_OF_THE_VALLEY = PLANT.set<"type">(PlantType::LILY_OF_THE_VALLEY);

    TableclothBlock& Blocks::TABLECLOTH = s_blocks.get<TableclothBlock>();

    ChessBlock& Blocks::CHESS        = s_blocks.get<ChessBlock>();
    ChessBlock& Blocks::WHITE_KING   = CHESS.set<"color">(ChessColor::WHITE).set<"type">(ChessType::KING);
    ChessBlock& Blocks::WHITE_QUEEN  = CHESS.set<"color">(ChessColor::WHITE).set<"type">(ChessType::QUEEN);
    ChessBlock& Blocks::WHITE_ROOK   = CHESS.set<"color">(ChessColor::WHITE).set<"type">(ChessType::ROOK);
    ChessBlock& Blocks::WHITE_BISHOP = CHESS.set<"color">(ChessColor::WHITE).set<"type">(ChessType::BISHOP);
    ChessBlock& Blocks::WHITE_KNIGHT = CHESS.set<"color">(ChessColor::WHITE).set<"type">(ChessType::KNIGHT);
    ChessBlock& Blocks::WHITE_PAWN   = CHESS.set<"color">(ChessColor::WHITE).set<"type">(ChessType::PAWN);
    ChessBlock& Blocks::BLACK_KING   = CHESS.set<"color">(ChessColor::BLACK).set<"type">(ChessType::KING);
    ChessBlock& Blocks::BLACK_QUEEN  = CHESS.set<"color">(ChessColor::BLACK).set<"type">(ChessType::QUEEN);
    ChessBlock& Blocks::BLACK_ROOK   = CHESS.set<"color">(ChessColor::BLACK).set<"type">(ChessType::ROOK);
    ChessBlock& Blocks::BLACK_BISHOP = CHESS.set<"color">(ChessColor::BLACK).set<"type">(ChessType::BISHOP);
    ChessBlock& Blocks::BLACK_KNIGHT = CHESS.set<"color">(ChessColor::BLACK).set<"type">(ChessType::KNIGHT);
    ChessBlock& Blocks::BLACK_PAWN   = CHESS.set<"color">(ChessColor::BLACK).set<"type">(ChessType::PAWN);

    Blocks& Blocks::Get()
    {
        return s_blocks;
    }

}
