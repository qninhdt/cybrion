#pragma once

#include "world/entity/entity.hpp"
#include "world/world_generator.hpp"

namespace cybrion
{
    struct BlockModifyResult
    {
        ref<Chunk> chunk;
        ivec3 pos;
        Block& oldBlock;
        Block& block;
    };

    class World
    {
    public:

        World();

        ref<Entity> spawnEntity(const vec3& pos);
        ref<Chunk> loadChunk(const ivec3& pos);
        ref<Chunk> getChunk(const ivec3& pos);

        void tick();

        Block& getBlock(const ivec3& pos);
        Block* tryGetBlock(const ivec3& pos);
        BlockModifyResult setBlock(const ivec3& pos, Block& block);
        BlockModifyResult placeBlock(const ivec3& pos, BlockFace face, Block& block);
        BlockModifyResult breakBlock(const ivec3& pos);

        void updateEntityTransforms();

    private:
        u32 chunkId = 0;

        WorldGenerator m_generator;
        umap<ivec3, ref<Chunk>> m_chunkMap;
        vector<ref<Entity>> m_entities;
        queue<ref<Chunk>> m_loadChunkResults;
        std::mutex m_chunkLock;
    };
}