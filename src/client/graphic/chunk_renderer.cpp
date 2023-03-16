#include "client/graphic/chunk_renderer.hpp"
#include "client/local_game.hpp"

namespace cybrion
{
    std::mutex glLock;

    ChunkRenderer::ChunkRenderer(const ref<Chunk>& chunk):
        m_chunk(chunk),
        opaqueMesh(true),
        transparentMesh(true),
        modelMesh(true),
        m_inBuildQueue(false),
        m_version(0),
        m_hasBuilt(false)
    {
        opaqueMesh.setAttributes({
            { GL::Type::UINT }, // packed_vertex
        });

        transparentMesh.setAttributes({
            { GL::Type::UINT }, // packed_vertex
        });

        modelMesh.setAttributes({
            { GL::Type::VEC3 }, // pos
            { GL::Type::VEC2 }, // tex
            { GL::Type::VEC3 }, // normal
            { GL::Type::UINT }, // tex_id
        });
    }
    
    ref<ChunkMeshResult> ChunkRenderer::buildChunkMesh()
    {
        auto result = std::make_shared<ChunkMeshResult>();
        
        Stopwatch stopwatch;
        stopwatch.reset();

        bool culling[6] = { 0 };
        Block::Block3x3x3 blocks;

        m_chunk->eachBlocks([&](Block& block, const ivec3& pos){
            // dont render transparent blocks like air
            if (block.getDisplay() == BlockDisplay::TRANSPARENT)
                return;

            auto& cubeRenderer = LocalGame::Get().getBlockRenderer(block.getId());

            bool visible = false;

            // block that is not visible now
            // but may be visible when its neighbors is loaded
            bool maybeVisible = false;

            // we can use simple block getter when block is not in chunk border
            bool inBorder = Chunk::isInBorder(pos);

            if (block.getShape() == BlockShape::CUBE)
            {
                for (auto& [dir, face] : BlockRenderer::CubeDirections)
                {
                    Block* neighbor = inBorder
                        ? std::get<0>(m_chunk->tryGetBlockMaybeOutside(pos + dir))
                        : m_chunk->tryGetBlock(pos + dir);

                    // cull this face when neighbor block is opaque
                    culling[u32(face)] = !neighbor
                        || (neighbor && neighbor->getDisplay() == BlockDisplay::OPAQUE)
                        || (neighbor && neighbor->getDisplay() == BlockDisplay::LIQUID && block.getDisplay() == BlockDisplay::LIQUID);
                    culling[u32(face)] &= !neighbor || neighbor->getShape() == BlockShape::CUBE;
                    visible |= !culling[u32(face)];
                    maybeVisible |= !neighbor;
                }

                if (visible)
                {
                    if (inBorder)
                        m_chunk->getBlockAndNeighborsMaybeOutside(pos, blocks);
                    else
                        m_chunk->getBlockAndNeighbors(pos, blocks);

                    if (block.getDisplay() == BlockDisplay::LIQUID)
                        cubeRenderer.generateCubeMesh(culling, pos, blocks, result->transparentVertices);
                    else
                        cubeRenderer.generateCubeMesh(culling, pos, blocks, result->opaqueVertices);
                }
            }
            else
            {
                auto meshes = block.getMeshes();

                for (auto mesh : meshes)
                {
                    for (auto vertex : mesh->vertices)
                    {
                        vertex.texId = block.getModelTexture(vertex.texId);
                        vertex.pos += vec3(pos) + vec3(
                            0.5f - Chunk::CHUNK_SIZE / 2,
                            0.5f - Chunk::CHUNK_SIZE / 2,
                            0.5f - Chunk::CHUNK_SIZE / 2
                        );
                        result->modelVertices.push_back(vertex);
                    }
                }
            }
        });

        opaqueMesh.setPos(m_chunk->getPos());
        opaqueMesh.updateModelMat();

        transparentMesh.setPos(m_chunk->getPos());
        transparentMesh.updateModelMat();

        modelMesh.setPos(m_chunk->getPos());
        modelMesh.updateModelMat();
        
        return result;
    }

    void ChunkRenderer::rebuildChunkMesh()
    {
      /*  CubeVertex* opaqueVertices = new CubeVertex[40000];
        Stopwatch stopwatch;
        stopwatch.reset();

        u32 opaqueSize = 0;
        bool culling[6] = { 0 };
        Block::Block3x3x3 blocks;

        for (auto& [pos, block] : visibleBlocks)
        {
            auto& cubeRenderer = LocalGame::Get().getBlockRenderer(block->getId());

            bool visible = false;
            bool inBorder = Chunk::isInBorder(pos);

            for (auto& [dir, face] : BlockRenderer::CubeDirections)
            {
                Block* neighbor = inBorder
                    ? std::get<0>(m_chunk->tryGetBlockMaybeOutside(pos + dir))
                    : m_chunk->tryGetBlock(pos + dir);

                culling[u32(face)] = !neighbor || (neighbor && neighbor->getDisplay() == BlockDisplay::OPAQUE);
                visible |= !culling[u32(face)];
            }

            if (visible)
            {
                if (inBorder)
                    m_chunk->getBlockAndNeighborsMaybeOutside(pos, blocks);
                else
                    m_chunk->getBlockAndNeighbors(pos, blocks);

                cubeRenderer.generateCubeMesh(culling, vec3(pos) + Chunk::CHUNK_ALIGN, blocks, opaqueVertices, opaqueSize);
            }
        }

        opaqueMesh.setVertices(opaqueVertices, opaqueSize);
        opaqueMesh.setDrawCount(opaqueSize / 4 * 6);

        CYBRION_CLIENT_TRACE("Rebuild chunk mesh in: {}", stopwatch.getDeltaTime()/1000.0f);
        delete[] opaqueVertices;*/
    }
}