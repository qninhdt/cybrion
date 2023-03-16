#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    enum class PlantType
    {
        GRASS,
        FERN,
        DANDELION,
        POPPY,
        BLUE_ORCHID,
        ALLIUM,
        AZURE,
        RED_TULIP,
        WHITE_TULIP,
        PINK_TULIP,
        OXEYE_DAISY,
        CORNFLOWER,
        LILY_OF_THE_VALLEY
    };

    class PlantBlock : public TBlock<
        PlantBlock,
        BlockType::PLANT,
        block::state<PlantType, "type", 13>
    >
    {
    public:

        bool beforePlace(const ivec3& pos);
    };
}