#include "world/block/decoration/tablecloth_block.hpp"
#include "game.hpp"

namespace cybrion
{
    Block& TableclothBlock::getPlacedBlock(const ivec3& pos)
    {
        BlockHorizontalFace face = Game::Get().getPlayer().getBlockHorizontalFace();

        if (face == BlockHorizontalFace::EAST || face == BlockHorizontalFace::WEST)
            return set<"face">(1);
        else
            return set<"face">(0);
    }
}