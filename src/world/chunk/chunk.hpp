#pragma once

#include "core/palette.hpp"
#include "world/block/block.hpp"

namespace cybrion
{
    static constexpr u32 CHUNK_SIZE = 32;
    static constexpr u32 CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

    struct ChunkData
    {
        Palette<packed_block, CHUNK_SIZE> blocks;
    };
}