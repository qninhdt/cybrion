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
        BlockType::SOIL,
        block::state<SoilType, "type", 5>
    >
    {
    public:
    };
}