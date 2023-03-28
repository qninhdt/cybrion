#pragma once

#include "world/block/nature/log_block.hpp"

namespace cybrion
{
    class World;

    class FenceBlock : public TBlock <
        FenceBlock,
        BlockType::FENCE,
        block::state<WoodType, "type", 7>,
        block::state<u32, "east"  , 2>,
        block::state<u32, "north" , 2>,
        block::state<u32, "west"  , 2>,
        block::state<u32, "south" , 2>
    >
    {
    public:
        void onTick(const ivec3& pos);
        bool isValidNeighbor(Block* block);
        FenceBlock& getFenceAt(World& world, const ivec3& pos);
    };
}