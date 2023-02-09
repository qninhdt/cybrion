#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    enum class SoilType
    {
        DIRT,
        SILT,
        BASALT,
        CLAY,
        GRASS
    };

    class SoilBlock : public TBlock<
        SoilBlock,
        block::state<SoilType, "type", 5>
    >
    {
    public:
        SoilBlock() : TBlock(BlockType::SOIL)
        {
            
        }
    };
}