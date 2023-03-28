#pragma once

#include "world/block/nature/log_block.hpp"

namespace cybrion
{
    class FenceGateBlock : public TBlock<
        FenceGateBlock,
        BlockType::FENCE_GATE,
        block::bool_state<"open">,
        block::state<WoodType, "type", 7>,
        block::state<BlockHorizontalFace, "face", 4>
    >
    {
    public:
        Block& getPlacedBlock(const ivec3& pos);
        void onInteract(const ivec3& pos);
    };
}