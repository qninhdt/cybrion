#include "client/graphic/chunk_renderer.hpp"
#include "client/graphic/block_renderer.hpp"
#include "client/local_game.hpp"

namespace cybrion
{
    CubeVertex opaqueVertices[400000];

    ChunkRenderer::ChunkRenderer(const ref<Chunk>& chunk):
        m_chunk(chunk),
        inBuildQueue(false),
        inRebuildList(false)
    {
    }

    ChunkRenderer::ChunkRenderer() :
        opaqueMesh(true)
    {
        opaqueMesh.setAttributes({
            { GL::Type::VEC3 }, // pos
            { GL::Type::VEC2 }, // uv
            { GL::Type::UINT }, // texId
            { GL::Type::UINT }, // face
            { GL::Type::UINT }, // ao
            });
    }
    
    void ChunkRenderer::buildChunkMesh()
    {
        u32 opaqueSize = 0;
        bool culling[6] = { 0 };

        m_chunk->eachBlocks([&](Block& block, const ivec3& pos){
            BlockDisplay display = block.getDisplay();
            auto& cubeRenderer = LocalGame::Get().getBlockRenderer(block.getId());

            // dont render transparent blocks like air
            if (display == BlockDisplay::TRANSPARENT)
                return;

            bool visible = false;

            // blocks that are not visible now because their neighbors are not loaded
            // but may be visible when their neighbors are loaded
            bool maybeVisible = false;

            auto blocks = m_chunk->getBlockAndNeighbors(pos);

            for (auto& [dir, face] : BlockRenderer::CubeDirections)
            {
                Block* neighbor = blocks[dir.x + 1][dir.y + 1][dir.z + 1];

                // cull this face when neighbor block is opaque
                culling[u32(face)] = !neighbor || (neighbor && neighbor->getDisplay() == BlockDisplay::OPAQUE);
                visible |= !culling[u32(face)];
                maybeVisible |= !neighbor;
            }

            if (visible)
                cubeRenderer.generateCubeMesh(culling, vec3(pos) + Chunk::CHUNK_ALIGN, blocks, opaqueVertices, opaqueSize);

            if (visible || maybeVisible)
                visibleBlocks[pos] = &block;
        });

        opaqueMesh.setPos(m_chunk->getPos());
        opaqueMesh.updateModelMat();

        opaqueMesh.setVertices(opaqueVertices, opaqueSize);
        opaqueMesh.setDrawCount(opaqueSize / 4 * 6);
    }

    void ChunkRenderer::rebuildChunkMesh()
    {
        u32 opaqueSize = 0;
        bool culling[6] = { 0 };

        for (auto& [pos, block] : visibleBlocks)
        {
            auto& cubeRenderer = LocalGame::Get().getBlockRenderer(block->getId());
            auto blocks = m_chunk->getBlockAndNeighbors(pos);

            for (auto& [dir, face] : BlockRenderer::CubeDirections)
            {
                Block* neighbor = blocks[dir.x + 1][dir.y + 1][dir.z + 1];
                culling[u32(face)] = !neighbor || (neighbor && neighbor->getDisplay() == BlockDisplay::OPAQUE);
            }

            cubeRenderer.generateCubeMesh(culling, vec3(pos) + Chunk::CHUNK_ALIGN, blocks, opaqueVertices, opaqueSize);
        }

        opaqueMesh.setVertices(opaqueVertices, opaqueSize);
        opaqueMesh.setDrawCount(opaqueSize / 4 * 6);
    }
}