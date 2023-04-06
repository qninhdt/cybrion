#include "world/block/blocks.hpp"

namespace cybrion
{
    Blocks Blocks::s_blocks;

    AirBlock& Blocks::AIR = s_blocks.get<AirBlock>();

    SoilBlock& Blocks::SOIL        = s_blocks.get<SoilBlock>();
    SoilBlock& Blocks::GRASS_BLOCK = SOIL.set<"type">(SoilType::GRASS);
    SoilBlock& Blocks::DIRT        = SOIL.set<"type">(SoilType::DIRT);
    SoilBlock& Blocks::CLAY        = SOIL.set<"type">(SoilType::CLAY);

    LogBlock& Blocks::LOG  = s_blocks.get<LogBlock>().set<"axis">(LogAxis::Y);
    LogBlock& Blocks::ACACIA_LOG   = LOG.set<"type">(WoodType::ACACIA  );
    LogBlock& Blocks::BIRCH_LOG    = LOG.set<"type">(WoodType::BIRCH   );
    LogBlock& Blocks::DARK_OAK_LOG = LOG.set<"type">(WoodType::DARK_OAK);
    LogBlock& Blocks::JUNGLE_LOG   = LOG.set<"type">(WoodType::JUNGLE  );
    LogBlock& Blocks::MANGROVE_LOG = LOG.set<"type">(WoodType::MANGROVE);
    LogBlock& Blocks::OAK_LOG      = LOG.set<"type">(WoodType::OAK     );
    LogBlock& Blocks::SPRUCE_LOG   = LOG.set<"type">(WoodType::SPRUCE  );

    RockBlock& Blocks::ROCK        = s_blocks.get<RockBlock>();
    RockBlock& Blocks::STONE       = ROCK.set<"type">(RockType::STONE);
    RockBlock& Blocks::COBBLESTONE = ROCK.set<"type">(RockType::COBBLESTONE);

    BrickBlock& Blocks::BRICK       = s_blocks.get<BrickBlock>();
    BrickBlock& Blocks::STONE_BRICK = BRICK.set<"type">(BrickType::STONE);
    BrickBlock& Blocks::MOSSY_STONE_BRICK = BRICK.set<"type">(BrickType::MOSSY_STONE);
    BrickBlock& Blocks::CLAY_BRICK = BRICK.set<"type">(BrickType::CLAY);
    BrickBlock& Blocks::QUARTZ_BRICK = BRICK.set<"type">(BrickType::QUARTZ);

    SandBlock& Blocks::SAND = s_blocks.get<SandBlock>();
    
    LeafBlock& Blocks::LEAF     = s_blocks.get<LeafBlock>();
    LeafBlock& Blocks::ACACIA_LEAF   = LEAF.set<"type">(WoodType::ACACIA);
    LeafBlock& Blocks::BIRCH_LEAF    = LEAF.set<"type">(WoodType::BIRCH);
    LeafBlock& Blocks::DARK_OAK_LEAF = LEAF.set<"type">(WoodType::DARK_OAK);
    LeafBlock& Blocks::JUNGLE_LEAF   = LEAF.set<"type">(WoodType::JUNGLE);
    LeafBlock& Blocks::MANGROVE_LEAF = LEAF.set<"type">(WoodType::MANGROVE);
    LeafBlock& Blocks::OAK_LEAF      = LEAF.set<"type">(WoodType::OAK);
    LeafBlock& Blocks::SPRUCE_LEAF   = LEAF.set<"type">(WoodType::SPRUCE);

    WaterBlock& Blocks::WATER = s_blocks.get<WaterBlock>();

    FenceBlock& Blocks::FENCE  = s_blocks.get<FenceBlock>().set<"east">(0).set<"north">(0).set<"west">(0).set<"south">(0);
    FenceBlock& Blocks::ACACIA_FENCE   = FENCE.set<"type">(WoodType::ACACIA);
    FenceBlock& Blocks::BIRCH_FENCE    = FENCE.set<"type">(WoodType::BIRCH);
    FenceBlock& Blocks::DARK_OAK_FENCE = FENCE.set<"type">(WoodType::DARK_OAK);
    FenceBlock& Blocks::JUNGLE_FENCE   = FENCE.set<"type">(WoodType::JUNGLE);
    FenceBlock& Blocks::MANGROVE_FENCE = FENCE.set<"type">(WoodType::MANGROVE);
    FenceBlock& Blocks::OAK_FENCE      = FENCE.set<"type">(WoodType::OAK);
    FenceBlock& Blocks::SPRUCE_FENCE   = FENCE.set<"type">(WoodType::SPRUCE);

    FenceGateBlock& Blocks::FENCE_GATE = s_blocks.get<FenceGateBlock>().set<"open">(0);
    FenceGateBlock& Blocks::ACACIA_FENCE_GATE = FENCE_GATE.set<"type">(WoodType::ACACIA);
    FenceGateBlock& Blocks::BIRCH_FENCE_GATE = FENCE_GATE.set<"type">(WoodType::BIRCH);
    FenceGateBlock& Blocks::DARK_OAK_FENCE_GATE = FENCE_GATE.set<"type">(WoodType::DARK_OAK);
    FenceGateBlock& Blocks::JUNGLE_FENCE_GATE = FENCE_GATE.set<"type">(WoodType::JUNGLE);
    FenceGateBlock& Blocks::MANGROVE_FENCE_GATE = FENCE_GATE.set<"type">(WoodType::MANGROVE);
    FenceGateBlock& Blocks::OAK_FENCE_GATE = FENCE_GATE.set<"type">(WoodType::OAK);
    FenceGateBlock& Blocks::SPRUCE_FENCE_GATE = FENCE_GATE.set<"type">(WoodType::SPRUCE);

    PlantBlock& Blocks::PLANT              = s_blocks.get<PlantBlock>();
    PlantBlock& Blocks::GRASS              = PLANT.set<"type">(PlantType::GRASS             );
    PlantBlock& Blocks::FERN               = PLANT.set<"type">(PlantType::FERN              );
    PlantBlock& Blocks::DANDELION          = PLANT.set<"type">(PlantType::DANDELION         );
    PlantBlock& Blocks::POPPY              = PLANT.set<"type">(PlantType::POPPY             ); 
    PlantBlock& Blocks::BLUE_ORCHID        = PLANT.set<"type">(PlantType::BLUE_ORCHID       );
    PlantBlock& Blocks::ALLIUM             = PLANT.set<"type">(PlantType::ALLIUM            );
    PlantBlock& Blocks::AZURE              = PLANT.set<"type">(PlantType::AZURE             );
    PlantBlock& Blocks::RED_TULIP          = PLANT.set<"type">(PlantType::RED_TULIP         );
    PlantBlock& Blocks::WHITE_TULIP        = PLANT.set<"type">(PlantType::WHITE_TULIP       );
    PlantBlock& Blocks::PINK_TULIP         = PLANT.set<"type">(PlantType::PINK_TULIP        );
    PlantBlock& Blocks::OXEYE_DAISY        = PLANT.set<"type">(PlantType::OXEYE_DAISY       );
    PlantBlock& Blocks::CORNFLOWER         = PLANT.set<"type">(PlantType::CORNFLOWER        );
    PlantBlock& Blocks::LILY_OF_THE_VALLEY = PLANT.set<"type">(PlantType::LILY_OF_THE_VALLEY);

    TableclothBlock& Blocks::TABLECLOTH = s_blocks.get<TableclothBlock>();

    ChessBlock& Blocks::CHESS        = s_blocks.get<ChessBlock>();
    ChessBlock& Blocks::WHITE_KING   = CHESS.set<"color">(ChessColor::WHITE).set<"type">(ChessType::KING  );
    ChessBlock& Blocks::WHITE_QUEEN  = CHESS.set<"color">(ChessColor::WHITE).set<"type">(ChessType::QUEEN );
    ChessBlock& Blocks::WHITE_ROOK   = CHESS.set<"color">(ChessColor::WHITE).set<"type">(ChessType::ROOK  );
    ChessBlock& Blocks::WHITE_BISHOP = CHESS.set<"color">(ChessColor::WHITE).set<"type">(ChessType::BISHOP);
    ChessBlock& Blocks::WHITE_KNIGHT = CHESS.set<"color">(ChessColor::WHITE).set<"type">(ChessType::KNIGHT);
    ChessBlock& Blocks::WHITE_PAWN   = CHESS.set<"color">(ChessColor::WHITE).set<"type">(ChessType::PAWN  );
    ChessBlock& Blocks::BLACK_KING   = CHESS.set<"color">(ChessColor::BLACK).set<"type">(ChessType::KING  );
    ChessBlock& Blocks::BLACK_QUEEN  = CHESS.set<"color">(ChessColor::BLACK).set<"type">(ChessType::QUEEN );
    ChessBlock& Blocks::BLACK_ROOK   = CHESS.set<"color">(ChessColor::BLACK).set<"type">(ChessType::ROOK  );
    ChessBlock& Blocks::BLACK_BISHOP = CHESS.set<"color">(ChessColor::BLACK).set<"type">(ChessType::BISHOP);
    ChessBlock& Blocks::BLACK_KNIGHT = CHESS.set<"color">(ChessColor::BLACK).set<"type">(ChessType::KNIGHT);
    ChessBlock& Blocks::BLACK_PAWN   = CHESS.set<"color">(ChessColor::BLACK).set<"type">(ChessType::PAWN  );

    WoolBlock& Blocks::WOOL = s_blocks.get<WoolBlock>();
    WoolBlock& Blocks::YELLOW_WOOL     = WOOL.set<"color">(BlockColor::YELLOW     );
    WoolBlock& Blocks::WHITE_WOOL      = WOOL.set<"color">(BlockColor::WHITE      );
    WoolBlock& Blocks::RED_WOOL        = WOOL.set<"color">(BlockColor::RED        );
    WoolBlock& Blocks::PURPLE_WOOL     = WOOL.set<"color">(BlockColor::PURPLE     );
    WoolBlock& Blocks::PINK_WOOL       = WOOL.set<"color">(BlockColor::PINK       );
    WoolBlock& Blocks::ORANGE_WOOL     = WOOL.set<"color">(BlockColor::ORANGE     );
    WoolBlock& Blocks::MAGENTA_WOOL    = WOOL.set<"color">(BlockColor::MAGENTA    );
    WoolBlock& Blocks::LIME_WOOL       = WOOL.set<"color">(BlockColor::LIME       );
    WoolBlock& Blocks::LIGHT_GRAY_WOOL = WOOL.set<"color">(BlockColor::LIGHT_GRAY );
    WoolBlock& Blocks::LIGHT_BLUE_WOOL = WOOL.set<"color">(BlockColor::LIGHT_BLUE );
    WoolBlock& Blocks::GREEN_WOOL      = WOOL.set<"color">(BlockColor::GREEN      );
    WoolBlock& Blocks::GRAY_WOOL       = WOOL.set<"color">(BlockColor::GRAY       );
    WoolBlock& Blocks::CYAN_WOOL       = WOOL.set<"color">(BlockColor::CYAN       );
    WoolBlock& Blocks::BROWN_WOOL      = WOOL.set<"color">(BlockColor::BROWN      );
    WoolBlock& Blocks::BLUE_WOOL       = WOOL.set<"color">(BlockColor::BLUE       );
    WoolBlock& Blocks::BLACK_WOOL      = WOOL.set<"color">(BlockColor::BLACK      );

    ConcreteBlock& Blocks::CONCRETE = s_blocks.get<ConcreteBlock>();
    ConcreteBlock& Blocks::YELLOW_CONCRETE = CONCRETE.set<"color">(BlockColor::YELLOW);
    ConcreteBlock& Blocks::WHITE_CONCRETE = CONCRETE.set<"color">(BlockColor::WHITE);
    ConcreteBlock& Blocks::RED_CONCRETE = CONCRETE.set<"color">(BlockColor::RED);
    ConcreteBlock& Blocks::PURPLE_CONCRETE = CONCRETE.set<"color">(BlockColor::PURPLE);
    ConcreteBlock& Blocks::PINK_CONCRETE = CONCRETE.set<"color">(BlockColor::PINK);
    ConcreteBlock& Blocks::ORANGE_CONCRETE = CONCRETE.set<"color">(BlockColor::ORANGE);
    ConcreteBlock& Blocks::MAGENTA_CONCRETE = CONCRETE.set<"color">(BlockColor::MAGENTA);
    ConcreteBlock& Blocks::LIME_CONCRETE = CONCRETE.set<"color">(BlockColor::LIME);
    ConcreteBlock& Blocks::LIGHT_GRAY_CONCRETE = CONCRETE.set<"color">(BlockColor::LIGHT_GRAY);
    ConcreteBlock& Blocks::LIGHT_BLUE_CONCRETE = CONCRETE.set<"color">(BlockColor::LIGHT_BLUE);
    ConcreteBlock& Blocks::GREEN_CONCRETE = CONCRETE.set<"color">(BlockColor::GREEN);
    ConcreteBlock& Blocks::GRAY_CONCRETE = CONCRETE.set<"color">(BlockColor::GRAY);
    ConcreteBlock& Blocks::CYAN_CONCRETE = CONCRETE.set<"color">(BlockColor::CYAN);
    ConcreteBlock& Blocks::BROWN_CONCRETE = CONCRETE.set<"color">(BlockColor::BROWN);
    ConcreteBlock& Blocks::BLUE_CONCRETE = CONCRETE.set<"color">(BlockColor::BLUE);
    ConcreteBlock& Blocks::BLACK_CONCRETE = CONCRETE.set<"color">(BlockColor::BLACK);

    MineralBlock& Blocks::MINERAL = s_blocks.get<MineralBlock>();
    MineralBlock& Blocks::COPPER_BLOCK = MINERAL.set<"type">(MineralType::COPPER);
    MineralBlock& Blocks::IRON_BLOCK = MINERAL.set<"type">(MineralType::IRON);
    MineralBlock& Blocks::LAPIS_BLOCK = MINERAL.set<"type">(MineralType::LAPIS);
    MineralBlock& Blocks::EMERALD_BLOCK = MINERAL.set<"type">(MineralType::EMERALD);
    MineralBlock& Blocks::GOLD_BLOCK = MINERAL.set<"type">(MineralType::GOLD);
    MineralBlock& Blocks::DIAMOND_BLOCK = MINERAL.set<"type">(MineralType::DIAMOND);

    Blocks& Blocks::Get()
    {
        return s_blocks;
    }

}
