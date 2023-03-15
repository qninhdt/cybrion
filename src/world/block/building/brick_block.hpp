#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    enum class BrickType
    {
        STONE
    };

    class BrickBlock : public TBlock<
        BrickBlock,
        BlockType::BRICK,
        block::state<BrickType, "type", 1>
    >
    {

    };
}