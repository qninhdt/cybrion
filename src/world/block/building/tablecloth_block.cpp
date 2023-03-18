#include "world/block/building/tablecloth_block.hpp"
#include "game.hpp"

namespace cybrion
{
    void TableclothBlock::onPlaced(const ivec3& pos)
    {
        auto& player = Game::Get().getPlayer();
        BlockFace face = player.getBlockDirection(true);

        auto block = this;
        if (face == BlockFace::EAST || face == BlockFace::WEST)
            block = &block->set<"dir">(1);
        else
            block = &block->set<"dir">(0);

        Game::Get().getWorld().setBlock(pos, *block);
    }
}