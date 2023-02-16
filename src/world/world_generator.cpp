#include "world/world_generator.hpp"
#include "world/block/blocks.hpp"

namespace cybrion
{
    ref<Chunk> WorldGenerator::generateChunkAt(const ivec3& pos)
    {
        auto chunk = std::make_shared<Chunk>(pos);

        LogBlock& log   = (LogBlock&) Blocks::Get().getBlock(BlockType::LOG);
        SoilBlock& dirt = (SoilBlock&)Blocks::Get().getBlock(BlockType::SOIL);
        SandBlock& sand = (SandBlock&)Blocks::Get().getBlock(BlockType::SAND);

        for (i32 x = 0; x < Chunk::CHUNK_SIZE; ++x)
        {
            for (i32 z = 0; z < Chunk::CHUNK_SIZE; ++z)
            {
                int height = sqrt(abs((Chunk::CHUNK_SIZE / 2 - x) * (Chunk::CHUNK_SIZE / 2 - z))) + 2;

                for (i32 y = 0; y < height; ++y)
                {
                    if (height > 10)
                        chunk->setBlock({ x, y, z }, dirt);
                    else
                        chunk->setBlock({ x, y, z }, sand);
                }
            }
        }

        return chunk;
    }
}