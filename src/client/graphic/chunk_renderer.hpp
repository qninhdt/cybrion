#pragma once

#include "world/chunk/chunk.hpp"
#include "client/GL/mesh.hpp"
#include "client/graphic/block_renderer.hpp"

namespace cybrion
{
    struct ChunkRenderer;

    struct ChunkMeshResult
    {
        CubeVertex* vertices;
        u32 size;
        u32 drawCount;
        ref<ChunkRenderer> renderer;
    };

    struct ChunkRenderer
    {
        ChunkRenderer(const ref<Chunk>& chunk);

        ref<Chunk> m_chunk;
        GL::Mesh opaqueMesh;
        umap<ivec3, Block*> visibleBlocks;

        bool inBuildQueue;
        bool inRebuildList;

        bool m_hasBuilt;
        std::atomic<bool> m_queueBuild;

        ChunkMeshResult buildChunkMesh();
        void rebuildChunkMesh();
    };
}