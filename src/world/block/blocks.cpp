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
    RockBlock& Blocks::COBBLESTONE = ROCK.set<"type">(RockType::COBBLESTONE);

    Blocks& Blocks::Get()
    {
        return s_blocks;
    }

}
