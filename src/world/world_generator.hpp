#pragma once

#include "world/chunk/chunk.hpp"

namespace cybrion
{
    enum class BiomeType
    {
        PLAIN,
        FOREST,
        DESERT,
        HILL
    };

    class WorldGenerator
    {
    public:
        WorldGenerator();

        void generateChunkAt(const ref<Chunk>& pos);

        BiomeType getBiome(i32 x, i32 z) const;
        f32 getRiverValue(f32 x, f32 z) const;
    private:
        FastNoiseLite m_noise;
        FastNoiseLite m_plainNoise;
        FastNoiseLite m_desertNoise;
        FastNoiseLite m_biomeNoise0;
        FastNoiseLite m_biomeNoise1;
        FastNoiseLite m_riverNoise;
    };
}