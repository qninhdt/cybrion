#pragma once

#include "world/block/block.hpp"

#include "world/block/nature/soil_block.hpp"
#include "world/block/nature/air_block.hpp"
#include "world/block/nature/log_block.hpp"
#include "world/block/nature/sand_block.hpp"

namespace cybrion
{
    // REGISTER NEW BLOCK HERE
    //           |
    //           |
    //           V
    class BlockRegistry : public BaseBlockRegistry<
        AirBlock,
        SoilBlock,
        LogBlock,
        SandBlock
    >
    {
    public:
        BlockRegistry();
        static BlockRegistry& Get();
    private:
        static BlockRegistry* s_blockRegistry;
    };
}