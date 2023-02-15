#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    enum class RockType
    {
        COBBLESTONE,
        GRAVEL,
    };

    class RockBlock : public TBlock<
        RockBlock, 
        BlockType::ROCK,
        block::state<RockType, "type", 2>
    >
    {

    };
}