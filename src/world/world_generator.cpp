#include "world/world_generator.hpp"
#include "game.hpp"
#include "world/block/blocks.hpp"
#include "world/block/nature/log_block.hpp"

namespace cybrion
{
    WorldGenerator::WorldGenerator(i32 seed) : m_biomeNoise0(seed << 0),
                                               m_biomeNoise1(seed << 1),
                                               m_plainNoise(seed << 2),
                                               m_desertNoise(seed << 3)
    {
        m_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_noise.SetFrequency(0.002f);

        m_plainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_plainNoise.SetFrequency(0.002f);

        m_desertNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_desertNoise.SetFrequency(0.002f);

        m_biomeNoise0.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_biomeNoise0.SetFrequency(.002f);

        m_riverNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        // m_riverNoise.SetFractalType(FastNoiseLite::FractalType_Ridged);
        m_riverNoise.SetFrequency(.0005f);
        /*      m_biomeNoise0.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Hybrid);
              m_biomeNoise0.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);
              m_biomeNoise0.SetCellularJitter(1.0f);*/

        m_biomeNoise1.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_biomeNoise1.SetFrequency(.005f);
    }

    void WorldGenerator::generateChunkAt(const ref<Chunk> &chunk)
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
                    bool hasMountain = false;
                    bool hasRiver = false;

                    f32 wNoise = m_noise.GetNoise(wposX, wposZ);
                    f32 mountainNoise = m_noise.GetNoise(wposX / 2, wposZ / 2) * 0.8 + m_noise.GetNoise(wposX * 4, wposZ * 4) * 0.1 + m_noise.GetNoise(wposX * 8, wposZ * 8) * 0.1;

                    mountainNoise = (mountainNoise + 1) / 2;

                    if (mountainNoise > 0.65)
                    {
                        hasMountain = true;
                        f32 wNoise1 = m_noise.GetNoise(wposX * 2 + 1024, wposZ * 2 + 1024) * 0.5 + m_noise.GetNoise(wposX * 4 + 1024, wposZ * 4 + 1024) * 0.25 + m_noise.GetNoise(wposX * 8 + 1024, wposZ * 8 + 1024) * 0.25;
                        wNoise1 = pow((wNoise1 + 1) / 2, 2);
                        wNoise += (mountainNoise - 0.65f) * 25 * wNoise1;
                    }

                    f32 river = getRiverValue(wposX, wposZ);

                    hasRiver = river > 0;

                    if (hasRiver && !hasMountain)
                    {
                        f32 noise = wNoise * 0.8 + m_plainNoise.GetNoise(wposX * 8, wposZ * 8) * 0.10f + m_plainNoise.GetNoise(wposX * 16, wposZ * 16) * 0.10f;

                        i32 wh = (noise + 1) / 2 * 40 + 15;
                        i32 lh = std::min(Chunk::CHUNK_SIZE, wh - chunkPos.y);

                        for (i32 y = 0; y < lh; ++y)
                        {
                            i32 wy = y + chunkPos.y;
                            chunk->setBlock({x, y, z}, Blocks::WATER);
                        }
                        continue;
                    }

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
                        constexpr i32 MAX_PLAIN_HEIGHT = 45;

                        f32 noise = wNoise * 0.8 + m_plainNoise.GetNoise(wposX * 8, wposZ * 8) * 0.10f + m_plainNoise.GetNoise(wposX * 16, wposZ * 16) * 0.10f;

                        i32 wh = (noise + 1) / 2 * MAX_PLAIN_HEIGHT + 15;
                        i32 lh = std::min(Chunk::CHUNK_SIZE, wh - chunkPos.y);

                        for (i32 y = 0; y < lh; ++y)
                        {
                            i32 wy = y + chunkPos.y;
                            if (hasMountain)
                            {
                                f32 shitNoise = m_noise.GetNoise(wposX * 8, wposZ * 8) * 0.40 + m_noise.GetNoise(wposX * 16, wposZ * 16) * 0.20 + m_noise.GetNoise(wposX * 32, wposZ * 32) * 0.40;

                                if (wy < 75 - shitNoise * 10)
                                {
                                    if (wy == wh - 1)
                                        chunk->setBlock({x, y, z}, Blocks::GRASS_BLOCK);
                                    else if (wy > wh * 3 / 4)
                                        chunk->setBlock({x, y, z}, Blocks::DIRT);
                                    else
                                        chunk->setBlock({x, y, z}, Blocks::STONE);
                                }
                                else
                                {
                                    chunk->setBlock({x, y, z}, (i32(shitNoise * 20) % 5 != 0) ? Blocks::STONE : Blocks::COBBLESTONE);
                                }
                            }
                            else
                            {
                                if (wy == wh - 1)
                                    chunk->setBlock({x, y, z}, Blocks::GRASS_BLOCK);
                                else if (wy > wh * 3 / 4)
                                    chunk->setBlock({x, y, z}, Blocks::DIRT);
                                else
                                    chunk->setBlock({x, y, z}, Blocks::STONE);
                            }
                        }
                    }
                    else if (biome == BiomeType::DESERT)
                    {
                        constexpr i32 MAX_DESERT_HEIGHT = 45;

                        f32 noise = wNoise * 0.8 + m_desertNoise.GetNoise(wposX * 8, wposZ * 8) * 0.10f + m_desertNoise.GetNoise(wposX * 16, wposZ * 16) * 0.10f;

                        f32 bigRockNoise = m_noise.GetNoise(wposX * 2 + 1024, wposZ * 2 + 1024);
                        bigRockNoise = (bigRockNoise + 1) / 2;

                        bool isBigRock = false;

                        if (bigRockNoise > 0.92)
                        {
                            isBigRock = true;
                            noise += std::min(0.93f - 0.92f, bigRockNoise - 0.92f) * 35 + std::max(0.0f, bigRockNoise - 0.93f) * 5 * (m_noise.GetNoise(wposX * 32 + 512, wposZ * 32 + 512) + 1);
                        }

                        i32 wh = (noise + 1) / 2 * MAX_DESERT_HEIGHT + 15;
                        i32 lh = std::min(Chunk::CHUNK_SIZE, wh - chunkPos.y);

                        for (i32 y = 0; y < lh; ++y)
                        {
                            i32 wy = y + chunkPos.y;

                            if (hasMountain)
                            {
                                f32 shitNoise = m_noise.GetNoise(wposX * 8, wposZ * 8) * 0.20 + m_noise.GetNoise(wposX * 16, wposZ * 16) * 0.40 + m_noise.GetNoise(wposX * 32, wposZ * 32) * 0.40;

                                if (wy < 75 - shitNoise * 10)
                                    chunk->setBlock({x, y, z}, Blocks::SAND);
                                else
                                    chunk->setBlock({x, y, z}, (i32(shitNoise * 20) % 5 != 0) ? Blocks::STONE : Blocks::COBBLESTONE);
                            }
                            else if (isBigRock)
                            {
                                if (wy > wh - 3)
                                    chunk->setBlock({x, y, z}, Blocks::SAND);
                                else
                                    chunk->setBlock({x, y, z}, Blocks::STONE);
                            }
                            else
                            {
                                if (wy > wh * 3 / 4)
                                    chunk->setBlock({x, y, z}, Blocks::SAND);
                                else
                                    chunk->setBlock({x, y, z}, Blocks::STONE);
                            }
                        }
                    }
                }
                else
                {
                    for (i32 y = 0; y < 32; ++y)
                    {
                        chunk->setBlock({x, y, z}, Blocks::STONE);
                    }
                }
            }
        }
    }

    void WorldGenerator::generateStructure(const ref<Chunk> &chunk)
    {
        if (!chunk->isNewChunk())
        {
            chunk->m_hasStructure = true;
            return;
        }

        auto &world = Game::Get().getWorld();

        ivec3 chunkPos = chunk->getChunkPos() * Chunk::CHUNK_SIZE;

        if (chunkPos.y > 20 && chunkPos.y < 50)
            for (i32 x = 0; x < Chunk::CHUNK_SIZE; ++x)
                for (i32 z = 0; z < Chunk::CHUNK_SIZE; ++z)
                    for (i32 y = 0; y < Chunk::CHUNK_SIZE; ++y)
                    {
                        ivec3 wpos = chunkPos + ivec3(x, y, z);
                        if (chunk->getBlock({x, y, z}) == Blocks::GRASS_BLOCK && wpos.x % 30 == 0 && wpos.z % 30 == 0)
                        {
                            growTreeAt(chunkPos + ivec3(x, y + 1, z));
                            break;
                        }
                    }

        chunk->m_hasStructure = true;
    }

    void WorldGenerator::growTreeAt(const ivec3 &pos)
    {
        auto &world = Game::Get().getWorld();
        auto &wood = Blocks::OAK_LOG.set<"axis">(LogAxis::Y);
        ivec3 p = pos;

        i32 h = (rand() % 3) + 2;

        for (i32 i = 0; i < h; ++i)
        {
            world.setBlock(p, wood);
            p.y += 1;
        }

        for (i32 i = -1; i <= 1; ++i)
            for (i32 j = -1; j <= 1; ++j)
                if (!(i == 0 && j == 0) && (rand() % 3) == 0)
                    world.setBlock(p + ivec3(i, -1, j), Blocks::OAK_LEAF);

        world.setBlock(p, Blocks::OAK_LEAF);

        i32 dx = (rand() % 3) - 1;
        i32 dz = (rand() % 3) - 1;

        if (dx == 0 && dz == 0)
            dx = 1;

        p.x += dx;
        p.z += dz;

        for (i32 i = 0; i < h; ++i)
        {
            world.setBlock(p, wood);
            p.y += 1;
        }

        array<ivec3, 4> dirs = {{{-1, 0, 0},
                                 {+1, 0, 0},
                                 {0, 0, -1},
                                 {0, 0, +1}}};

        i32 lh = std::max(h - 1, 2);
        for (i32 i = -lh; i <= lh; ++i)
        {
            for (i32 j = -lh; j <= lh; ++j)
            {
                if ((abs(i) - lh) + (abs(j) - lh) != 0)
                    world.setBlock(p + ivec3(i, 0, j), Blocks::OAK_LEAF);
            }
        }

        lh -= 1;
        p.y += 1;
        for (i32 i = -lh; i <= lh; ++i)
        {
            for (i32 j = -lh; j <= lh; ++j)
            {
                if ((abs(i) - lh) + (abs(j) - lh) != 0)
                    world.setBlock(p + ivec3(i, 0, j), Blocks::OAK_LEAF);
            }
        }
    }

    BiomeType WorldGenerator::getBiome(i32 x, i32 z) const
    {
        f32 noise0 = m_biomeNoise0.GetNoise(f32(x), f32(z)) * 0.5;
        f32 noise1 = m_biomeNoise1.GetNoise(f32(x), f32(z)) * 0.6 + m_biomeNoise1.GetNoise(f32(x * 4), f32(z * 4)) * 0.2;
        +m_biomeNoise1.GetNoise(f32(x * 8), f32(z * 8)) * 0.2;

        if (abs(noise0) < 2 * noise1)
            noise0 += noise1 * (abs(noise0)) * 50;

        // if (noise1 > -0.1)
        //     //return BiomeType::HILL;
        //     noise0 += 0.2 - noise1;

        // if (noise0 > 0)
        //     return BiomeType::PLAIN;
        // else if (noise0 > -0.5)
        //     return BiomeType::DESERT;
        // else
        //     return BiomeType::HILL;
        if (noise0 > -0.1)
            return BiomeType::PLAIN;
        else
            return BiomeType::DESERT;
    }

    f32 WorldGenerator::getRiverValue(f32 x, f32 z) const
    {
        f32 noise = m_riverNoise.GetNoise(x, z) * 0.5 + m_riverNoise.GetNoise(x * 4, z * 4) * 0.25 + m_riverNoise.GetNoise(x * 8, z * 8) * 0.25;

        f32 noise1 = m_riverNoise.GetNoise(x * 16 + 100, z * 16 + 100);

        return (0.2 < noise && noise < 0.3) ? 1 : -1;
    }
}