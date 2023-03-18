#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    class TableclothBlock : public TBlock<
        TableclothBlock,
        BlockType::TABLECLOTH,
        block::state<u32, "dir", 2>
    >
    {
    public:

        void onPlaced(const ivec3& pos);
    };
}