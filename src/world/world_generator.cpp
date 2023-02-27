#include "world/world_generator.hpp"
#include "world/block/blocks.hpp"

namespace cybrion
{
    WorldGenerator::WorldGenerator():
        m_biomeNoise0(24),
        m_biomeNoise1(48),
        m_plainNoise(24),
        m_desertNoise(24)
    {
        m_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_noise.SetFrequency(0.002f);

        m_plainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_plainNoise.SetFrequency(0.002f);

        m_desertNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_desertNoise.SetFrequency(0.003f);

        m_biomeNoise0.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_biomeNoise0.SetFrequency(.002f);

        m_riverNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        //m_riverNoise.SetFractalType(FastNoiseLite::FractalType_Ridged);
        m_riverNoise.SetFrequency(.0005f);
  /*      m_biomeNoise0.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Hybrid);
        m_biomeNoise0.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);
        m_biomeNoise0.SetCellularJitter(1.0f);*/

        m_biomeNoise1.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_biomeNoise1.SetFrequency(.005f);
    }

    void WorldGenerator::generateChunkAt(const ref<Chunk>& chunk)
    {
        ivec3 chunkPos = chunk->getChunkPos() * Chunk::CHUNK_SIZE;
        for (i32 x = 0; x < Chunk::CHUNK_SIZE; ++x)
        {
            for (i32 z = 0; z < Chunk::CHUNK_SIZE; ++z)
            {
                BiomeType biome = getBiome(chunkPos.x + x, chunkPos.z + z);
                f32 wposX = chunkPos.x + x;
                f32 wposZ = chunkPos.z + z;

                if (chunkPos.y >= 0)
                {
                    f32 river = getRiverValue(wposX, wposZ);

                    if (river > 0) continue;

                    if (biome == BiomeType::PLAIN)
                    {
                        // PLAIN
                        // |
                        // |- 75
                        // |               
                        // |---------------------------  -> grass  : 1
                        // |    .             .        | -> dirt   : 5-10
                        // |         .    .         .  | -> stone  : 20-30
                        // |---------------------------|
                        //
                        constexpr i32 MAX_PLAIN_HEIGHT = 50;

                        f32 noise = m_noise.GetNoise(wposX, wposZ) * 0.8
                            + m_plainNoise.GetNoise(wposX * 8, wposZ * 8) * 0.10f
                            + m_plainNoise.GetNoise(wposX * 16, wposZ * 16) * 0.10f;

                        i32 wh = (noise + 1) / 2 * MAX_PLAIN_HEIGHT + 15;
                        i32 lh = std::min(Chunk::CHUNK_SIZE, wh - chunkPos.y);

                        for (i32 y = 0; y < lh; ++y)
                        {
                            i32 wy = y + chunkPos.y;
                            if (wy == wh - 1)
                                chunk->setBlock({ x, y, z }, Blocks::GRASS);
                            else if (wy > wh * 3 / 4)
                                chunk->setBlock({ x, y, z }, Blocks::DIRT);
                            else
                                chunk->setBlock({ x, y, z }, Blocks::STONE);
                        }
                    }
                    else if (biome == BiomeType::DESERT)
                    {
                        constexpr i32 MAX_DESERT_HEIGHT = 50;

                        f32 noise = m_noise.GetNoise(wposX, wposZ) * 0.8
                            + m_desertNoise.GetNoise(wposX * 8, wposZ * 8) * 0.10f
                            + m_desertNoise.GetNoise(wposX * 16, wposZ * 16) * 0.10f;

                        i32 wh = (noise + 1) / 2 * MAX_DESERT_HEIGHT + 15;
                        i32 lh = std::min(Chunk::CHUNK_SIZE, wh - chunkPos.y);

                        for (i32 y = 0; y < lh; ++y)
                        {
                            i32 wy = y + chunkPos.y;
                            if (wy > wh * 3 / 4)
                                chunk->setBlock({ x, y, z }, Blocks::SAND);
                            else
                                chunk->setBlock({ x, y, z }, Blocks::STONE);
                        }
                    }
                }
                else
                {
                    for (i32 y = 0; y < 32; ++y)
                    {
                        chunk->setBlock({ x, y, z }, Blocks::STONE);
                    }
                }
            }
        }
    }

    BiomeType WorldGenerator::getBiome(i32 x, i32 z) const
    {
        f32 noise0 = m_biomeNoise0.GetNoise(f32(x), f32(z)) * 0.5;
        f32 noise1 = m_biomeNoise1.GetNoise(f32(x), f32(z)) * 0.6
            + m_biomeNoise1.GetNoise(f32(x * 4), f32(z * 4)) * 0.2;
            +m_biomeNoise1.GetNoise(f32(x * 8), f32(z * 8)) * 0.2;

        if (abs(noise0) < 2 * noise1)
            noise0 += noise1 * (abs(noise0)) * 50;

        //if (noise1 > -0.1)
        //    //return BiomeType::HILL;
        //    noise0 += 0.2 - noise1;

        //if (noise0 > 0)
        //    return BiomeType::PLAIN;
        //else if (noise0 > -0.5)
        //    return BiomeType::DESERT;
        //else
        //    return BiomeType::HILL;
        if (noise0 > 0)
            return BiomeType::PLAIN;
        else
            return BiomeType::DESERT;
    }

    f32 WorldGenerator::getRiverValue(f32 x, f32 z) const
    {
        f32 noise = m_riverNoise.GetNoise(x, z) * 0.5
            + m_riverNoise.GetNoise(x * 4, z * 4) * 0.25
            + m_riverNoise.GetNoise(x * 8, z * 8) * 0.25;

        f32 noise1 = m_riverNoise.GetNoise(x * 16 + 100, z * 16 + 100);

        return (0.2 < noise && noise < 0.3) ? 1 : -1;
    }
}