#include "world/block/nature/plant_block.hpp"
#include "game.hpp"

namespace cybrion
{
    bool PlantBlock::beforePlace(const ivec3& pos)
    {
        auto& world = Game::Get().getWorld();
        Block& belowBlock = world.getBlock(pos + ivec3(0, -1, 0));

        if (belowBlock == Blocks::DIRT || belowBlock == Blocks::GRASS_BLOCK)
            return true;

        return false;
    }
}