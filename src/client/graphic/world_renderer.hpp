#pragma once

#include "world/world.hpp"
#include "client/graphic/block_renderer.hpp"
#include "client/GL/mesh.hpp"
#include "client/GL/shader.hpp"
#include "client/stopwatch.hpp"

namespace cybrion
{
    struct ChunkRenderer
    {
        GL::Mesh opaqueMesh;
        umap<uvec3, Block*> visibleBlocks;
        bool isDirty;

        ChunkRenderer();
    };

    using OpaqueCubeShader = GL::Shader<"MVP">;

    class WorldRenderer
    {
    public:

        WorldRenderer(World& world);

        void render(f32 deltaTime);

        void setupChunk(Object chunk);
        void setupEntity(Object entity);

        void buildChunkMeshes(f32 maxDuration);
        void buildChunkMesh(Object& chunk);
        void rebuildChunkMesh(Object& chunk);
        void rebuildChunkMeshes(f32 maxDuration);

        void updateEntityTransforms(f32 lerpFactor);

    private:
        static CubeVertex s_opaqueVertices[400000];

        std::array<ChunkData*, 6> getNeighborChunkData(ChunkData& data);

        bool getBlockIfIsOutside(const ivec3& pos, Block** block, std::array<ChunkData*, 6>& neighbors);

        World& m_world;
        queue<Object> m_buildMeshQueue;
        queue<Object> m_rebuildMeshQueue;
        Stopwatch m_stopwatch;

        OpaqueCubeShader m_opaqueCubeShader;

        entt::registry& m_registry;
    };
}