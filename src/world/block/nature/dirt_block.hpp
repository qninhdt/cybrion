#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    enum class DirtType
    {
        SOIL,
        BASALT,
        CLAY
    };

    class DirtBlock : public TBlock<
        DirtBlock,
        block::bool_state<"worm">,
        block::state<DirtType, "type", 3>
    >
    {
    public:
        DirtBlock() : TBlock(BlockType::DIRT)
        {
            
        }
    };
}