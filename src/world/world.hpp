#pragma once

#include "core/entity.hpp"
#include "world/chunk/chunk.hpp"

namespace cybrion
{
    class World
    {
    public:
        World();

        Entity loadChunk(const ivec3& pos);

        Block& getBlock(const ivec3& pos);
        Block* tryGetBlock(const ivec3& pos);

    protected:

        static ivec3 GetChunkPos(const ivec3& pos);
        static uvec3 GetLocalPos(const ivec3& pos);

        umap<ivec3, Entity> m_chunkMap;
        
        queue<ivec3> m_loadQueue;

        entt::registry& m_registry;
    };
}