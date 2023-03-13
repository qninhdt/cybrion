#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    class WaterBlock : public TBlock<
        WaterBlock,
        BlockType::WATER
    >
    {
    };
}