#pragma once

#include "core/linear_palette.hpp"
#include "world/block/block_registry.hpp"

namespace cybrion
{
    static constexpr u32 LOG_2_CHUNK_SIZE = 5;
    static constexpr u32 CHUNK_SIZE = 1 << LOG_2_CHUNK_SIZE;
    static constexpr u32 CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

    struct ChunkData
    {
        LinearPalette<BlockRegistry::BlockStateCount(), CHUNK_VOLUME> blocks;
        
        Entity eastChunk;
        Entity topChunk;
        Entity southChunk;
        Entity westChunk;
        Entity bottomChunk;
        Entity northChunk;

        Block& getBlock(const uvec3& pos) const;
        Block* tryGetBlock(const ivec3& pos);
        void setBlock(const uvec3& pos, const Block& block);

        static u32 PosToIndex(const uvec3& pos);
    };
}