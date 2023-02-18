#pragma once

#include "core/linear_palette.hpp"
#include "world/block/blocks.hpp"

namespace cybrion
{
    enum class ChunkStatus
    {
        NONE,
        IN_QUEUE,
        GENERATING,
        READY
    };

    class Chunk
    {
    public:
        static constexpr i32 LOG_2_CHUNK_SIZE = 5;
        static constexpr i32 CHUNK_SIZE = 1 << LOG_2_CHUNK_SIZE;
        static constexpr i32 CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
        static constexpr vec3 CHUNK_ALIGN = vec3(0.5f, 0.5f, 0.5f) - vec3(CHUNK_SIZE / 2, CHUNK_SIZE / 2, CHUNK_SIZE / 2);

        using BlockStorage = LinearPalette<Blocks::StateCount(), CHUNK_VOLUME>;
        using Chunk3x3x3 = array<array<array<ref<Chunk>, 3>, 3>, 3>;

        Chunk(const ivec3& chunkPos);

        Block& getBlock(const ivec3& pos) const;
        Block* tryGetBlock(const ivec3& pos) const;
        tuple<Block*, ref<Chunk>> tryGetBlockMaybeOutside(const ivec3& pos) const;
        ref<Chunk> getNeighbor(i32 dx, i32 dy, i32 dz) const;
        void setBlock(const ivec3& pos, Block& block);
        vec3 getPos() const;
        ivec3 getChunkPos() const;
        void getBlockAndNeighbors(const ivec3& pos, Block::Block3x3x3& blocks);
        void getBlockAndNeighborsMaybeOutside(const ivec3& pos, Block::Block3x3x3& blocks);
        void setNeighbor(const ivec3& dir, const ref<Chunk>& chunk);

        void eachNeighbors(const std::function<void(ref<Chunk>&, const ivec3&)>& callback);
        void eachBlocks(const std::function<void(Block&, const ivec3&)>& callback);
        void eachBlockAndNeighbors(const ivec3& pos, const std::function<void(Block*&, ref<Chunk>&, const ivec3& dir)>& callback);

        u32 getId() const;
        u32 getMemorySizeApproximately() const;

        ChunkStatus getStatus() const;
        bool isReady() const;
        bool isUnloaded() const;

        ~Chunk();

        static i32 posToIndex(const ivec3& pos);
        static ivec3 posToLocalPos(const ivec3& pos);
        static ivec3 posToChunkPos(const ivec3& pos);
        static bool isInBorder(const ivec3& pos);
        static bool isInside(const ivec3& pos);

    private:

        friend class World;
        friend class WorldGenerator;

        static std::atomic<u32> s_idN;

        std::atomic<ChunkStatus> m_status;
        std::atomic<bool> m_ready;
        std::atomic<bool> m_unloaded;

        BlockStorage m_blocks;
        Chunk3x3x3 m_neighbors;
        vec3 m_pos;
        ivec3 m_chunkPos;
        
        u32 m_id;
    };
}