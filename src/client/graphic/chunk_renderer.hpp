#pragma once

#include "world/chunk/chunk.hpp"
#include "client/GL/mesh.hpp"

namespace cybrion
{
    struct ChunkRenderer
    {
        ChunkRenderer(const ref<Chunk>& chunk);

        ref<Chunk> m_chunk;
        GL::Mesh opaqueMesh;
        umap<ivec3, Block*> visibleBlocks;

        bool inBuildQueue;
        bool inRebuildList;

        void buildChunkMesh();
        void rebuildChunkMesh();
    };
}