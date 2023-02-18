#include "world/chunk/chunk.hpp"
#include "game.hpp"

namespace cybrion
{

    Chunk::Chunk(const ivec3& chunkPos): 
        m_neighbors{nullptr},
        m_chunkPos(chunkPos),
        m_id(0),
        m_status(ChunkStatus::NONE),
        m_ready(false),
        m_unloaded(false)
    {
        m_pos = m_chunkPos * Chunk::CHUNK_SIZE + ivec3(Chunk::CHUNK_SIZE / 2, Chunk::CHUNK_SIZE / 2, Chunk::CHUNK_SIZE / 2);
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
        auto neighbor = m_neighbors[chunkPos.x + 1][chunkPos.y + 1][chunkPos.z + 1];

        if (!neighbor)
            return { nullptr, nullptr };

        return { neighbor->tryGetBlock(posToLocalPos(pos)), neighbor };
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

    void Chunk::getBlockAndNeighbors(const ivec3& pos, Block::Block3x3x3& blocks)
    {
        ivec3 dir;
        for (dir.x = -1; dir.x <= 1; ++dir.x)
            for (dir.y = -1; dir.y <= 1; ++dir.y)
                for (dir.z = -1; dir.z <= 1; ++dir.z)
                    blocks[dir.x + 1][dir.y + 1][dir.z + 1] = &getBlock(pos + dir);
    }

    void Chunk::getBlockAndNeighborsMaybeOutside(const ivec3& pos, Block::Block3x3x3& blocks)
    {
        eachBlockAndNeighbors(pos, [&](Block*& block, ref<Chunk>&, const ivec3& dir) {
            blocks[dir.x + 1][dir.y + 1][dir.z + 1] = block;
        });
    }

    void Chunk::setNeighbor(const ivec3& dir, const ref<Chunk>& chunk)
    {
        m_neighbors[dir.x + 1][dir.y + 1][dir.z + 1] = chunk;
    }

    void Chunk::eachNeighbors(const std::function<void(ref<Chunk>&, const ivec3&)>& callback)
    {
        for (i32 x = -1; x <= 1; ++x)
            for (i32 y = -1; y <= 1; ++y)
                for (i32 z = -1; z <= 1; ++z)
                    if (!(x == 0 && y == 0 && z == 0))
                    {
                        callback(m_neighbors[x + 1][y + 1][z + 1], ivec3(x, y, z));
                    }
    }

    void Chunk::eachBlocks(const std::function<void(Block&, const ivec3&)>& callback)
    {
        ivec3 pos{ 0,0,0 };
        for (pos.x = 0; pos.x < Chunk::CHUNK_SIZE; ++pos.x)
            for (pos.y = 0; pos.y < Chunk::CHUNK_SIZE; ++pos.y)
                for (pos.z = 0; pos.z < Chunk::CHUNK_SIZE; ++pos.z)
                    callback(getBlock(pos), pos);
    }

    void Chunk::eachBlockAndNeighbors(const ivec3& pos, const std::function<void(Block*&, ref<Chunk>&, const ivec3&)>& callback)
    {
        ivec3 dir{ 0,0,0 };
        for (dir.x = -1; dir.x <= 1; ++dir.x)
            for (dir.y = -1; dir.y <= 1; ++dir.y)
                for (dir.z = -1; dir.z <= 1; ++dir.z)
                {
                    auto&& [block, chunk] = tryGetBlockMaybeOutside(pos + dir);
                    callback(block, chunk, dir);
                }
    }

    u32 Chunk::getId() const
    {
        return m_id;
    }

    u32 Chunk::getMemorySizeApproximately() const
    {
        return CHUNK_VOLUME * sizeof(u32) / (32 / (1 << m_blocks.getBitExp()));
    }

    ChunkStatus Chunk::getStatus() const
    {
        return m_status;
    }

    bool Chunk::isReady() const
    {
        return m_ready;
    }

    bool Chunk::isUnloaded() const
    {
        return m_unloaded;
    }

    Chunk::~Chunk()
    {

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
    bool Chunk::isInBorder(const ivec3& pos)
    {
        return pos.x == 0 || pos.y == 0 || pos.z == 0 || (pos.x == Chunk::CHUNK_SIZE - 1) || (pos.y == Chunk::CHUNK_SIZE - 1) || (pos.z == Chunk::CHUNK_SIZE - 1);
    }

    bool Chunk::isInside(const ivec3& pos)
    {
        return 0 <= pos.x && pos.x < CHUNK_SIZE && 0 <= pos.y && pos.y < CHUNK_SIZE && 0 <= pos.z && pos.z < CHUNK_SIZE;
    }
}