#include "client/graphic/chunk_renderer.hpp"
#include "client/graphic/block_renderer.hpp"
#include "client/local_game.hpp"

namespace cybrion
{
    CubeVertex opaqueVertices[400000];

    ChunkRenderer::ChunkRenderer() :
        opaqueMesh(true)
    {
        opaqueMesh.setAttributes({
            { GL::Type::VEC3 },
            { GL::Type::VEC2 },
            { GL::Type::UINT }
        });
    }

    std::array<ChunkData*, 6> getNeighborChunkData(ChunkData& data)
    {
        std::array<ChunkData*, 6> result;
        for (u32 i = 0; i < 6; ++i)
            result[i] = data.neighbors[i].valid() ? &data.neighbors[i].get<ChunkData>() : nullptr;
        return result;
    }

    bool getBlockIfIsOutside(const ivec3& pos, Block** block, std::array<ChunkData*, 6>& neighbors)
    {
        if (pos.x < 0) {
            if (neighbors[3]) *block = &neighbors[3]->getBlock({ 31, pos.y, pos.z });
            return true;
        }
        else if (pos.y < 0) {
            if (neighbors[4]) *block = &neighbors[4]->getBlock({ pos.x, 31, pos.z });
            return true;
        }
        else if (pos.z < 0) {
            if (neighbors[5]) *block = &neighbors[5]->getBlock({ pos.x, pos.y, 31 });
            return true;
        }
        else if (pos.x >= CHUNK_SIZE) {
            if (neighbors[0]) *block = &neighbors[0]->getBlock({ 0, pos.y, pos.z });
            return true;
        }
        else if (pos.y >= CHUNK_SIZE) {
            if (neighbors[1]) *block = &neighbors[1]->getBlock({ pos.x, 0, pos.z });
            return true;
        }
        else if (pos.z >= CHUNK_SIZE) {
            if (neighbors[2]) *block = &neighbors[2]->getBlock({ pos.x, pos.y, 0 });
            return true;
        }
        return false;
    }
    
    void ChunkRenderer::buildChunkMesh(ChunkData& data)
    {
        auto neighbors = getNeighborChunkData(data);

        u32 opaqueSize = 0;
        bool culling[6] = { 0 };
        vec3 align = vec3(0.5f, 0.5f, 0.5f) - vec3(CHUNK_SIZE / 2, CHUNK_SIZE / 2, CHUNK_SIZE / 2);

        for (u32 x = 0; x < CHUNK_SIZE; ++x)
        {
            for (u32 y = 0; y < CHUNK_SIZE; ++y)
            {
                for (u32 z = 0; z < CHUNK_SIZE; ++z)
                {
                    uvec3 pos{ x, y, z };
                    Block& block = data.getBlock(pos);
                    BlockDisplay display = block.getDisplay();
                    auto& cubeRenderer = LocalGame::Get().getBlockRenderer(block.getId());

                    // dont render transparent blocks like air
                    if (display == BlockDisplay::TRANSPARENT)
                        continue;

                    bool visible = false;

                    // blocks that are not visible now because their neighbors are not loaded
                    // but may be visible when their neighbors are loaded
                    bool maybeVisible = false;

                    for (auto& [dir, face] : BlockRenderer::CubeDirections)
                    {
                        Block* neighbor = nullptr;
                        ivec3 npos = ivec3(pos) + dir;

                        if (!getBlockIfIsOutside(npos, &neighbor, neighbors))
                            neighbor = &data.getBlock(uvec3(npos));

                        // cull this face when neighbor block is opaque
                        culling[u32(face)] = !neighbor || (neighbor && neighbor->getDisplay() == BlockDisplay::OPAQUE);
                        visible |= !culling[u32(face)];
                        maybeVisible |= !neighbor;
                    }

                    if (visible)
                        cubeRenderer.generateCubeMesh(culling, vec3(pos) + align, opaqueVertices, opaqueSize);

                    if (visible || maybeVisible)
                        visibleBlocks[pos] = &block;
                }
            }
        }

        //std::cout << data.getWorldPosition().x << ' ' << data.getWorldPosition().y << '\n';
        opaqueMesh.setPosition(data.getWorldPosition());
        opaqueMesh.updateModelMatrix();

        opaqueMesh.setVertices(opaqueVertices, opaqueSize);
        opaqueMesh.setDrawCount(opaqueSize / 4 * 6);
    }

    void ChunkRenderer::rebuildChunkMesh(ChunkData& data)
    {
        auto neighbors = getNeighborChunkData(data);

        u32 opaqueSize = 0;
        bool culling[6] = { 0 };
        vec3 align = vec3(0.5f, 0.5f, 0.5f) - vec3(CHUNK_SIZE / 2, CHUNK_SIZE / 2, CHUNK_SIZE / 2);
        for (auto& [pos, block] : visibleBlocks)
        {
            auto& cubeRenderer = LocalGame::Get().getBlockRenderer(block->getId());

            for (auto& [dir, face] : BlockRenderer::CubeDirections)
            {
                Block* neighbor = nullptr;
                ivec3 npos = ivec3(pos) + dir;

                if (!getBlockIfIsOutside(npos, &neighbor, neighbors))
                    neighbor = &data.getBlock(uvec3(npos));

                culling[u32(face)] = !neighbor || (neighbor && neighbor->getDisplay() == BlockDisplay::OPAQUE);
            }

            cubeRenderer.generateCubeMesh(culling, vec3(pos) + align, opaqueVertices, opaqueSize);
        }

        opaqueMesh.setVertices(opaqueVertices, opaqueSize);
        opaqueMesh.setDrawCount(opaqueSize / 4 * 6);
    }
}