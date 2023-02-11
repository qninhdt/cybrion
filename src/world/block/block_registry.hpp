#pragma once

#include "world/block/block.hpp"

#include "world/block/nature/soil_block.hpp"
#include "world/block/nature/air_block.hpp"
#include "world/block/nature/log_block.hpp"

namespace cybrion
{
    // REGISTER NEW BLOCK HERE
    //           |
    //           |
    //           V
    using BlockRegistry = BaseBlockRegistry<
        AirBlock,
        SoilBlock,
        LogBlock
    >;
}