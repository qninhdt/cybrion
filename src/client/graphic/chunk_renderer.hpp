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
        vector<u32> opaqueVertices;
        vector<u32> transparentVertices;
        vector<BlockVertex> modelVertices;
        ref<ChunkRenderer> renderer;
        u32 version;
    };

    struct ChunkRenderer
    {
        ChunkRenderer(const ref<Chunk>& chunk);

        ref<Chunk> m_chunk;
        GL::Mesh opaqueMesh;
        GL::Mesh transparentMesh;
        GL::Mesh modelMesh;
        umap<ivec3, Block*> visibleBlocks;

        std::atomic<bool> m_inBuildQueue;
        std::atomic<u32> m_version;

        bool m_hasBuilt;

        ref<ChunkMeshResult> buildChunkMesh();
        void rebuildChunkMesh();
    };
}