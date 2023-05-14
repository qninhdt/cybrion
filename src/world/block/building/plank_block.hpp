#pragma once

#include "world/block/nature/log_block.hpp"

namespace cybrion
{
    class PlankBlock : public TBlock<
                           PlankBlock,
                           BlockType::PLANK,
                           block::state<WoodType, "type", 7>>
    {
    };
}