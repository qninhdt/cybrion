#include "world/world_generator.hpp"
#include "world/block/blocks.hpp"

namespace cybrion
{
    WorldGenerator::WorldGenerator()
    {
        m_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    }

    void WorldGenerator::generateChunkAt(const ref<Chunk>& chunk)
    {
        static int cnt = 0;
        ++cnt; 

        Block* blocks[] = {
            &Blocks::DIRT,
            &Blocks::COBBLESTONE,
            &Blocks::OAK_LOG
        };

        ivec3 chunkPos = chunk->getChunkPos() * Chunk::CHUNK_SIZE;
        for (i32 x = 0; x < Chunk::CHUNK_SIZE; ++x)
        {
            for (i32 z = 0; z < Chunk::CHUNK_SIZE; ++z)
            {
                vec2 worldPos = { chunkPos.x + x, chunkPos.z + z };
                f32 noise = m_noise.GetNoise(worldPos.x, worldPos.y);
                i32 height = noise * 200 + 50;
                i32 localHeight = std::min(std::max(0, height - chunkPos.y), Chunk::CHUNK_SIZE);

                for (i32 y = 0; y < localHeight; ++y)
                {
                    chunk->setBlock({ x, y, z }, Blocks::COBBLESTONE);
                }
            }
        }
    }
}