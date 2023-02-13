#pragma once

#include "core/Object.hpp"
#include "world/chunk/chunk.hpp"

namespace cybrion
{
    class World
    {
    public:
        World();

        Object spawnEntity(const vec3& pos);
        Object loadChunk(const ivec3& pos);
        Object getChunk(const ivec3& pos);
        Object* tryGetChunk(const ivec3& pos);

        void tick();

        Block& getBlock(const ivec3& pos);
        Block* tryGetBlock(const ivec3& pos);
        tuple<Object&, Block&> setBlock(const ivec3& pos, Block& block);
        void placeBlock(const ivec3& pos, BlockFace face, Block& block);
        void breakBlock(const ivec3& pos);

        static ivec3 GetChunkPos(const ivec3& pos);
        static uvec3 GetLocalPos(const ivec3& pos);

    protected:

        umap<ivec3, Object> m_chunkMap;
        
        queue<ivec3> m_loadQueue;

        entt::registry& m_registry;
    };
}