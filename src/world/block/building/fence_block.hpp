#pragma once

#include "world/block/nature/log_block.hpp"

namespace cybrion
{
    class FenceBlock : public TBlock <
        FenceBlock,
        BlockType::FENCE,
        block::state<WoodType, "type", 1>
    >
    {

    };
}