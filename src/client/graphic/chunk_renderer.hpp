#pragma once

#include "world/chunk/chunk.hpp"
#include "client/GL/mesh.hpp"

namespace cybrion
{
    struct ChunkRenderer
    {
        GL::Mesh opaqueMesh;
        umap<uvec3, Block*> visibleBlocks;

        ChunkRenderer();

        void buildChunkMesh(ChunkData& data);
        void rebuildChunkMesh(ChunkData& data);

        void updateBlockVisiblity(ChunkData& data, const uvec3& pos, Block& block) const;
        void onBlockChanged(ChunkData& data, const uvec3& pos, Block& to, Block& from);
    };
}