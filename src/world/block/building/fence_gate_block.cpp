#include "world/block/building/fence_gate_block.hpp"
#include "game.hpp"

namespace cybrion
{
    Block& cybrion::FenceGateBlock::getPlacedBlock(const ivec3& pos)
    {
        BlockHorizontalFace face = Block::GetOppositeHorizontalFace(
            Game::Get().getPlayer().getBlockHorizontalFace()
        );
        return set<"face">(face);
    }

    void FenceGateBlock::onInteract(const ivec3& pos)
    {
        Game::Get().getWorld().updateBlock(pos, set<"open">(!get<"open">()));
    }
}