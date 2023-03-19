#include "world/block/building/tablecloth_block.hpp"
#include "game.hpp"

namespace cybrion
{
    Block& TableclothBlock::getPlacedBlock(const ivec3& pos)
    {
        BlockFace face = Game::Get().getPlayer().getBlockDirection(true);

        if (face == BlockFace::EAST || face == BlockFace::WEST)
            return set<"dir">(1);
        else
            return set<"dir">(0);
    }
}