#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    class WoolBlock : public TBlock <
        WoolBlock,
        BlockType::WOOL,
        block::state<BlockColor, "color", 16>
    >
    {
    };
}