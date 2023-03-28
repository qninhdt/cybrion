#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    enum class WoodType
    {
        ACACIA   ,
        BIRCH    ,
        DARK_OAK ,
        JUNGLE   ,
        MANGROVE ,
        OAK      ,
        SPRUCE   ,
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
        block::state<WoodType, "type", 7>,
        block::state<LogAxis, "axis", 3>
    >
    {
    public:
        Block& getPlacedBlock(const ivec3& pos);
    };
         
}