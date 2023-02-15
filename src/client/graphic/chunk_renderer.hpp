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
        umap<uvec3, Block*> visibleBlocks;

        bool inBuildQueue;
        bool inRebuildList;

        ChunkRenderer();

        void buildChunkMesh();
        void rebuildChunkMesh();

        void updateBlockVisiblity(const uvec3& pos, Block& block) const;
        void updateBlock(const uvec3& pos, Block& to, Block& from);
    };
}