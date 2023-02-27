#include "world/block/blocks.hpp"

namespace cybrion
{
    Blocks Blocks::s_blocks;

    AirBlock& Blocks::AIR = s_blocks.get<AirBlock>();

    SoilBlock& Blocks::SOIL  = s_blocks.get<SoilBlock>();
    SoilBlock& Blocks::GRASS = SOIL.set<"type">(SoilType::GRASS);
    SoilBlock& Blocks::DIRT  = SOIL.set<"type">(SoilType::DIRT);

    LogBlock& Blocks::LOG    = s_blocks.get<LogBlock>();
    LogBlock& Blocks::OAK_LOG = LOG.set<"type">(WoodType::OAK);

    RockBlock& Blocks::ROCK = s_blocks.get<RockBlock>();
    RockBlock& Blocks::STONE = ROCK.set<"type">(RockType::STONE);
    RockBlock& Blocks::COBBLESTONE = ROCK.set<"type">(RockType::COBBLESTONE);

    BrickBlock& Blocks::BRICK = s_blocks.get<BrickBlock>();
    BrickBlock& Blocks::STONE_BRICK = BRICK.set<"type">(BrickType::STONE);

    SandBlock& Blocks::SAND = s_blocks.get<SandBlock>();

    Blocks& Blocks::Get()
    {
        return s_blocks;
    }

}
