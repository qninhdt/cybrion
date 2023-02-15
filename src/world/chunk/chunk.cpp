#include "world/chunk/chunk.hpp"
#include "game.hpp"

namespace cybrion
{
    std::atomic<u32> Chunk::s_idN = 0;

    Chunk::Chunk(): m_id(s_idN++)
    {
    }

    Block& Chunk::getBlock(const ivec3& pos) const
    {
        Block* block = tryGetBlock(pos);
        CYBRION_ASSERT(block != nullptr, "This block is invalid or not loaded");
        return *block;
    }

    Block* Chunk::tryGetBlock(const ivec3& pos) const
    {
        CYBRION_ASSERT(0 <= pos.x && pos.x < CHUNK_SIZE && 0 <= pos.y && pos.y < CHUNK_SIZE && 0 <= pos.z && pos.z < CHUNK_SIZE, "Out of chunk size");
        u32 blockId = m_blocks.get(posToIndex(pos));
        return &Blocks::Get().getBlock(blockId);
    }

    tuple<Block*, ref<Chunk>> Chunk::tryGetBlockMaybeOutside(const ivec3& pos) const
    {
        ivec3 chunkPos = posToChunkPos(pos);
        ivec3 localPos = posToLocalPos(pos);

        auto chunk = getNeighbor(chunkPos.x, chunkPos.y, chunkPos.z);
        if (chunk == nullptr) return { nullptr, nullptr };

        return { chunk->tryGetBlock(localPos), chunk };
    }

    ref<Chunk> Chunk::getNeighbor(i32 dx, i32 dy, i32 dz) const
    {
        CYBRION_ASSERT(-1 <= dx && dx <= 1 && -1 <= dy && dy <= 1 && -1 <= dz && dz <= 1, "Too far");
        return m_neighbors[dx + 1][dy + 1][dz + 1];
    }

    void Chunk::setBlock(const ivec3& pos, Block& block)
    {
        CYBRION_ASSERT(0 <= pos.x && pos.x < CHUNK_SIZE && 0 <= pos.y && pos.y < CHUNK_SIZE && 0 <= pos.z && pos.z < CHUNK_SIZE, "Out of chunk size");
        m_blocks.set(posToIndex(pos), block.getId());
    }

    vec3 Chunk::getPos() const
    {
        return m_pos;
    }

    ivec3 Chunk::getChunkPos() const
    {
        return m_chunkPos;
    }

    Block::Block3x3x3 Chunk::getBlockAndNeighbors(const ivec3& pos)
    {
        Block::Block3x3x3 result;

        eachBlockAndNeighbors(pos, [&](Block* block, ref<Chunk>&, const ivec3& dir) {
            result[dir.x + 1][dir.y + 1][dir.z + 1] = block;
        });

        return result;
    }

    void Chunk::eachNeighbors(std::function<void(ref<Chunk>&)> callback)
    {
        for (i32 x = -1; x <= 1; ++x)
            for (i32 y = -1; y <= 1; ++y)
                for (i32 z = -1; z <= 1; ++z)
                    if (x && y && z)
                        callback(m_neighbors[x + 1][y + 1][z + 1]);
    }

    void Chunk::eachBlocks(std::function<void(Block&, const ivec3&)> callback)
    {
        ivec3 pos{ 0, 0, 0 };
        for (; pos.x < Chunk::CHUNK_SIZE; ++pos.x)
            for (; pos.y < Chunk::CHUNK_SIZE; ++pos.y)
                for (; pos.z < Chunk::CHUNK_SIZE; ++pos.z)
                    callback(getBlock(pos), pos);
    }

    void Chunk::eachBlockAndNeighbors(const ivec3& pos, std::function<void(Block*, ref<Chunk>&, const ivec3&)> callback)
    {
        ivec3 dir{ -1,-1,-1 };
        for (; dir.x <= 1; ++dir.x)
            for (; dir.y <= 1; ++dir.y)
                for (; dir.z <= 1; ++dir.z)
                {
                    auto [block, chunk] = tryGetBlockMaybeOutside(pos + dir);
                    callback(block, chunk, dir);
                }
    }

    u32 Chunk::getId() const
    {
        return m_id;
    }

    i32 Chunk::posToIndex(const ivec3& pos)
    {
        return pos.x * CHUNK_SIZE * CHUNK_SIZE + pos.y * CHUNK_SIZE + pos.z;
    }

    ivec3 Chunk::posToLocalPos(const ivec3& pos)
    {
        return { pos.x & (CHUNK_SIZE - 1), pos.y & (CHUNK_SIZE - 1), pos.z & (CHUNK_SIZE - 1) };
    }

    ivec3 Chunk::posToChunkPos(const ivec3& pos)
    {
        return { pos.x >> LOG_2_CHUNK_SIZE, pos.y >> LOG_2_CHUNK_SIZE, pos.z >> LOG_2_CHUNK_SIZE };
    }
}