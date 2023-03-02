#pragma once

#include "world/block/nature/log_block.hpp"

namespace cybrion
{
    class LeafBlock : public TBlock<
        LeafBlock,
        BlockType::LEAF,
        block::state<WoodType, "type", 1>
    >
    {

    };
}