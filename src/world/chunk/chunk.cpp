#include "world/chunk/chunk.hpp"
#include "game.hpp"

namespace cybrion
{
    Block& ChunkData::getBlock(const uvec3& pos) const
    {
        u32 blockId = blocks.get(PosToIndex(pos));
        return Game::Get().getBlockRegistry().getBlock(blockId);
    }

    void ChunkData::setBlock(const uvec3& pos, const Block& block)
    {
        u32 index = PosToIndex(pos);
        blocks.set(index, block.getId());
    }
    
    u32 ChunkData::PosToIndex(const uvec3& pos)
    {
        return pos.x * CHUNK_SIZE * CHUNK_SIZE + pos.y * CHUNK_SIZE + pos.z;
    }
}