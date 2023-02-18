#pragma once

#include "world/chunk/chunk.hpp"

namespace cybrion
{
    class WorldGenerator
    {
    public:
        WorldGenerator();

        void generateChunkAt(const ref<Chunk>& pos);

    private:
        FastNoiseLite m_noise;
    };
}