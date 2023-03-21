#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    class TableclothBlock : public TBlock<
        TableclothBlock,
        BlockType::TABLECLOTH,
        block::state<u32, "face", 2>
    >
    {
    public:

        Block& getPlacedBlock(const ivec3& pos);
    };
}