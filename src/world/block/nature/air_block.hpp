#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    class AirBlock : public TBlock<AirBlock>
    {
    public:
            AirBlock() : TBlock(BlockType::AIR) {}
    };
}