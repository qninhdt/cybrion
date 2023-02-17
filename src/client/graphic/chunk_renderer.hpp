#pragma once

#include "world/chunk/chunk.hpp"
#include "client/GL/mesh.hpp"
#include "client/graphic/block_renderer.hpp"

namespace cybrion
{
    struct ChunkRenderer;

    enum class ChunkMeshStatus
    {
        NONE,
        QUEUE,
        RUNNING
    };

    struct ChunkMeshResult
    {
        vector<CubeVertex> vertices;
        ref<ChunkRenderer> renderer;
        u32 version;
    };

    struct ChunkRenderer
    {
        ChunkRenderer(const ref<Chunk>& chunk);

        ref<Chunk> m_chunk;
        GL::Mesh opaqueMesh;
        umap<ivec3, Block*> visibleBlocks;

        std::atomic<bool> m_inBuildQueue;
        std::atomic<u32> m_version;

        bool m_hasBuilt;

        ref<ChunkMeshResult> buildChunkMesh();
        void rebuildChunkMesh();
    };
}