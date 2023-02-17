#pragma once

#include "world/chunk/chunk.hpp"

namespace cybrion
{
    class WorldGenerator
    {
    public:
        WorldGenerator();

        ref<Chunk> generateChunkAt(const ivec3& pos);

    private:
        FastNoiseLite m_noise;
    };
}