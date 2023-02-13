#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    enum class WoodType
    {
        OAK
    };

    enum class LogAxis
    {
        X,
        Y,
        Z
    };

    class LogBlock : public TBlock<
        LogBlock,
        BlockType::LOG,
        block::state<WoodType, "type", 1>,
        block::state<LogAxis, "axis", 3>
    >
    {
    public:
        Block& getPlacedBlock(const ivec3& pos, BlockFace face)
        {
            switch (face)
            {
            case cybrion::BlockFace::WEST:
            case cybrion::BlockFace::EAST:
                return set<"axis">(LogAxis::X);
            case cybrion::BlockFace::TOP:
            case cybrion::BlockFace::BOTTOM:
                return set<"axis">(LogAxis::Y);
            default:
                return set<"axis">(LogAxis::Z);
            }
        }
    };
         
}