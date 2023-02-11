#include "world/world.hpp"
#include "game.hpp"

namespace cybrion
{
    World::World() :
        m_registry(GetRegistry())
    {}

    Entity World::loadChunk(const ivec3& pos)
    {
        Entity chunk(m_registry.create());
        auto& data = chunk.assign<ChunkData>();
        data.position = pos;
 
        LogBlock& dirt = (LogBlock&) Game::Get().getBlockRegistry().getBlock(BlockType::LOG);

        for (u32 x = 0; x < CHUNK_SIZE; ++x)
        {
            for (u32 z = 0; z < CHUNK_SIZE; ++z)
            {
                int height = sqrt((x*x+z*z)/2);

                for (u32 y = 0; y < height; ++y)
                {
                    dirt = dirt.set<"axis">((x + y + z) % 3);
                    data.setBlock({ x, y, z }, dirt);
                }
            }
        }

        m_chunkMap[pos] = chunk;

        return chunk;
    }

    Block& World::getBlock(const ivec3& pos)
    {
        Block* block = tryGetBlock(pos);
        assert(block != nullptr);
        return *block;
    }

    Block* World::tryGetBlock(const ivec3& pos)
    {
        ivec3 chunkPos = GetChunkPos(pos);
        uvec3 localPos = GetLocalPos(pos);

        auto it = m_chunkMap.find(chunkPos);

        if (it == m_chunkMap.end()) return nullptr;

        auto& chunkData = it->second.get<ChunkData>();
        return &chunkData.getBlock(localPos);
    }

    ivec3 World::GetChunkPos(const ivec3& pos)
    {
        return { pos.x >> LOG_2_CHUNK_SIZE, pos.y >> LOG_2_CHUNK_SIZE, pos.z >> LOG_2_CHUNK_SIZE };
    }

    uvec3 World::GetLocalPos(const ivec3& pos)
    {
        return { pos.x & (CHUNK_SIZE - 1), pos.y & (CHUNK_SIZE - 1), pos.z & (CHUNK_SIZE - 1) };
    }

}
