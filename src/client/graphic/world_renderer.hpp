#pragma once

#include "world/world.hpp"
#include "client/graphic/block_renderer.hpp"
#include "client/GL/mesh.hpp"
#include "client/GL/shader.hpp"
#include "core/stopwatch.hpp"
#include "client/graphic/chunk_renderer.hpp"
#include "client/graphic/entity_renderer.hpp"
#include <readerwriterqueue/readerwriterqueue.h>

namespace cybrion
{
    using OpaqueCubeShader = GL::Shader<"MVP", "enable_diffuse", "enable_ao">;
    using BlockModelShader = GL::Shader<"MVP">;
    using BasicShader = GL::Shader<"MVP">;
    using BasicBlockShader = GL::Shader<"MVP", "use_light">;

    class WorldRenderer
    {
    public:

        WorldRenderer(World& world);

        void render(f32 delta, bool showEntityBorder);

        void addChunk(const ref<Chunk>& chunk);
        void removeChunk(const ref<Chunk>& chunk);

        void addEntity(const ref<Entity>& entity);

        ref<EntityRenderer> getEntityRenderer(const ref<Entity>& entity) const;
        ref<ChunkRenderer> getChunkRenderer(const ref<Chunk>& chunk) const;

        void prepareRebuild(const ref<ChunkRenderer>& renderer);
        void updateBlock(const BlockModifyResult& result);
        void updateChunk(const ref<Chunk>& chunk);

        void updateEntityRenderers(f32 delta);

    private:

        friend class HUD;

        World& m_world;

        queue<ref<ChunkRenderer>> m_buildChunkQueue;
        vector<ref<ChunkRenderer>> m_rebuildChunkList;

        umap<u32, ref<ChunkRenderer>> m_chunkRenderers;
        umap<u32, ref<EntityRenderer>> m_entityRenderers;

        Stopwatch m_stopwatch;

        BasicShader m_basicShader;
        OpaqueCubeShader m_opaqueCubeShader;
        BlockModelShader m_blockModelShader;

        bool m_enableAO;
        bool m_enableDiffuse;

        moodycamel::ConcurrentQueue<ref<ChunkMeshResult>> m_chunkMeshResults;
    };
}