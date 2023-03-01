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
        void loadChunk(const ivec3& pos);
        void unloadChunk(const ivec3& pos);
        ref<Chunk> getChunk(const ivec3& pos);

        void tick();

        Block& getBlock(const ivec3& pos);
        Block* tryGetBlock(const ivec3& pos);
        BlockModifyResult setBlock(const ivec3& pos, Block& block);
        void setBlockOnly(const ivec3& pos, Block& block);
        BlockModifyResult placeBlock(const ivec3& pos, BlockFace face, Block& block);
        BlockModifyResult breakBlock(const ivec3& pos);

        void updateEntityTransforms();

    private:
        u32 chunkId = 0;

        WorldGenerator m_generator;
        umap<ivec3, ref<Chunk>> m_chunkMap;
        vector<ref<Entity>> m_entities;
        moodycamel::ConcurrentQueue<ref<Chunk>> m_loadChunkResults;
    };
}