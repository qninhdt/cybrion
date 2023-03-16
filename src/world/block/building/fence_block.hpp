#pragma once

#include "world/block/nature/log_block.hpp"

namespace cybrion
{
    class World;

    class FenceBlock : public TBlock <
        FenceBlock,
        BlockType::FENCE,
        block::state<WoodType, "type", 1>,
        block::state<u32, "east"  , 2>,
        block::state<u32, "north" , 2>,
        block::state<u32, "west"  , 2>,
        block::state<u32, "south" , 2>
    >
    {
    public:
        Block& getPlacedBlock(const ivec3& pos, BlockFace face);
        void onNeighborChanged(const ivec3& pos, const ivec3& neighborPos, const Block& from, const Block& to);
        FenceBlock& getFenceAt(World& world, const ivec3& pos);
    };
}