#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    enum class MineralType
    {
        COPPER,
        IRON,
        EMERALD,
        LAPIS,
        GOLD,
        DIAMOND
    };

    class MineralBlock : public TBlock<
        MineralBlock,
        BlockType::MINERAL,
        block::state<MineralType, "type", 6>
    >
    {

    };
}