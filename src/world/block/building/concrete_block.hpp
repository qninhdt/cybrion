#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    class ConcreteBlock : public TBlock <
        ConcreteBlock,
        BlockType::CONCRETE,
        block::state<BlockColor, "color", 16>
    >
    {
    };
}