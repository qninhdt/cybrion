#include "world/block/nature/log_block.hpp"
#include "game.hpp"

namespace cybrion
{
    Block& LogBlock::getPlacedBlock(const ivec3& pos)
    {
        BlockFace face = Game::Get().getPlayer().getTargetFace();

        switch (face)
        {
        case cybrion::BlockFace::WEST:
        case cybrion::BlockFace::EAST:
            return set<"axis">(LogAxis::X);
        case cybrion::BlockFace::TOP:
        case cybrion::BlockFace::BOTTOM:
            return set<"axis">(LogAxis::Y);
        default:
            return set<"axis">(LogAxis::Z);
        }
    }
}