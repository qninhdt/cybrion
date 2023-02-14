#pragma once

#include "world/world.hpp"
#include "client/graphic/block_renderer.hpp"
#include "client/GL/mesh.hpp"
#include "client/GL/shader.hpp"
#include "core/stopwatch.hpp"
#include "client/graphic/chunk_renderer.hpp"

namespace cybrion
{
    using OpaqueCubeShader = GL::Shader<"MVP", "enable_diffuse", "enable_ao">;
    using BasicShader = GL::Shader<"MVP">;

    class WorldRenderer
    {
    public:

        WorldRenderer(World& world);

        void render(f32 deltaTime, bool showEntityBorder);

        void setupChunk(Object chunk);
        void setupEntity(Object entity);

        void buildChunkMeshes(f32 maxDuration);
        void rebuildChunkMeshes(f32 maxDuration);

        void queueRebuild(Object chunk);
        void onBlockChanged(const ivec3& pos);

        void updateEntityTransforms(f32 lerpFactor);
        void updateBlockVisiblity(const ivec3& pos);

    private:

        friend class HUD;

        World& m_world;
        queue<Object> m_buildMeshQueue;
        uset<Object> m_rebuildMeshSet;
        Stopwatch m_stopwatch;

        BasicShader m_basicShader;
        OpaqueCubeShader m_opaqueCubeShader;

        bool m_enableAO;
        bool m_enableDiffuse;

        entt::registry& m_registry;
    };
}