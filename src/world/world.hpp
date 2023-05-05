#pragma once

#include "world/entity/entity.hpp"
#include "world/world_generator.hpp"

namespace cybrion
{
    struct BlockModifyResult
    {
        ref<Chunk> chunk;
        ivec3 pos;
        Block &oldBlock;
        Block &block;
    };

    class World
    {
    public:
        World(const string &name, i32 seed);

        ref<Entity> spawnEntity(const vec3 &pos, const vec3 &rot);
        void loadChunk(const ivec3 &pos);
        void unloadChunk(const ivec3 &pos);
        ref<Chunk> getChunk(const ivec3 &pos);

        void tick();

        void playSound(const string &name);

        Block &getBlock(const ivec3 &pos);
        Block *tryGetBlock(const ivec3 &pos);
        BlockModifyResult setBlock(const ivec3 &pos, Block &block);
        BlockModifyResult updateBlock(const ivec3 &pos, Block &block);
        BlockModifyResult placeBlock(const ivec3 &pos, Block &block);

        string getName() const;

        void updateEntityTransforms();

        void loadRegion(const ivec3 &pos);
        void save(const string &path);
        void saveChunk(const ivec3 &pos, const ref<Chunk> &chunk);
        void syncRegionFiles();

        static void createNewWorld(const string &name);
        static ref<World> loadWorld(const string &path);

        static ivec3 ToRegionPos(const ivec3 &pos);
        static ivec3 ToLocalRegionPos(const ivec3 &pos);
        static string GetRegionFilename(const ivec3 &pos);

    private:
        u32 chunkId = 0;

        WorldGenerator m_generator;
        umap<ivec3, ref<Chunk>> m_chunkMap;
        vector<ref<Entity>> m_entities;
        queue<ref<Chunk>> m_saveChunkQueue;

        moodycamel::ConcurrentQueue<ref<Chunk>> m_loadChunkResults;

        umap<ivec3, ref<jbt::hjbt_file>> m_regionMap;

        string m_name;
        string m_savePath;
    };
}