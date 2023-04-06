#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    enum class BrickType
    {
        CLAY,
        STONE,
        MOSSY_STONE,
        QUARTZ
    };

    class BrickBlock : public TBlock<
        BrickBlock,
        BlockType::BRICK,
        block::state<BrickType, "type", 4>
    >
    {

    };
}