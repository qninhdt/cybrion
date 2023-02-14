#include "world/world.hpp"
#include "client/resource/block_loader.hpp"
#include "client/resource/shader_manager.hpp"
#include "client/graphic/world_renderer.hpp"
#include "client/local_game.hpp"
#include "client/graphic/entity_renderer.hpp"
#include "world/entity/entity.hpp"
#include "client/graphic/basic_mesh_generator.hpp"

namespace cybrion
{
    WorldRenderer::WorldRenderer(World& world):
        m_registry(GetRegistry()),
        m_world(world)
    {
        m_basicShader = ShaderManager::Get().getShader<BasicShader>("basic");
        m_opaqueCubeShader = ShaderManager::Get().getShader<OpaqueCubeShader>("opaque_cube");
    }

    void WorldRenderer::render(f32 deltaTime)
    {
        BlockLoader::Get().bindTextureArray();
        m_opaqueCubeShader.use();

        for (auto&& [_, renderer] : m_registry.view<ChunkRenderer>().each())
        {
            auto& opaqueMesh = renderer.opaqueMesh;
            m_opaqueCubeShader.setUniform<"MVP">(
                LocalGame::Get().getCamera().getProjectionViewMatrix()
                * opaqueMesh.getModelMatrix()
            );
            opaqueMesh.drawTriangles();
        }

        m_basicShader.use();
        for (auto&& [_, renderer] : m_registry.view<EntityRenderer>().each())
        {
            auto& aabbMesh = renderer.aabbMesh;
            m_basicShader.setUniform<"MVP">(
                LocalGame::Get().getCamera().getProjectionViewMatrix()
                * aabbMesh.getModelMatrix()
            );
            aabbMesh.drawLines();
        }
    }

    void WorldRenderer::setupChunk(Object chunk)
    {
        m_buildMeshQueue.push(chunk);

        auto& data = chunk.get<ChunkData>();
        for (u32 i = 0; i < 6; ++i)
        {
            if (data.neighbors[i].valid())
            {
                /// NOTE: lock ?
                if (data.neighbors[i].tryGet<ChunkRenderer>() != nullptr)
                {
                    queueRebuild(data.neighbors[i]);
                }
            }
        }
    }

    void WorldRenderer::setupEntity(Object entity)
    {
        auto& renderer = entity.assign<EntityRenderer>();
        BasicMeshGenerator::LineCubeMesh(renderer.aabbMesh, 1, { 0, 0, 1 });
    }

    void WorldRenderer::buildChunkMeshes(f32 maxDuration)
    {
        m_stopwatch.reset();
        while (!m_buildMeshQueue.empty() && maxDuration > m_stopwatch.getDeltaTime())
        {
            Object chunk = m_buildMeshQueue.front();
            m_buildMeshQueue.pop();

            auto& data = chunk.get<ChunkData>();
            auto& renderer = chunk.tryGet<ChunkRenderer>()
                ? chunk.get<ChunkRenderer>()
                : chunk.assign<ChunkRenderer>();

            renderer.buildChunkMesh(data);
        }
    }

    void WorldRenderer::rebuildChunkMeshes(f32 maxDuration)
    {
        for (auto& chunk: m_rebuildMeshSet)
        {
            auto& data = chunk.get<ChunkData>();
            auto& renderer = chunk.get<ChunkRenderer>();

            renderer.rebuildChunkMesh(data);
        }

        m_rebuildMeshSet.clear();
    }

    void WorldRenderer::queueRebuild(Object chunk)
    {
        m_rebuildMeshSet.insert(chunk);
    }

    void WorldRenderer::onBlockChanged(const ivec3& pos)
    {
        updateBlockVisiblity(pos);

        for (auto& [dir, face] : ChunkData::Directions)
        {
            updateBlockVisiblity(pos + dir);
        }
    }

    void WorldRenderer::updateEntityTransforms(f32 lerpFactor)
    {
        auto view = m_registry.view<EntityData, EntityRenderer>();

        for (auto&& [_, data, renderer] : view.each())
        {
            renderer.mesh.setPosition(
                util::Lerp(
                    data.oldTransform.getPosition(),
                    data.transform.getPosition(), lerpFactor
                )
            );

            renderer.aabbMesh.setPosition(
                util::Lerp(
                    data.getOldWorldAABB().getPosition(),
                    data.getWorldAABB().getPosition(), lerpFactor
                )
            );
            renderer.aabbMesh.updateModelMatrix();

            renderer.mesh.setRotation(
                util::LerpRotaion(
                    data.oldTransform.getRotation(),
                    data.transform.getRotation(), lerpFactor
                 )
            );

            renderer.mesh.updateModelMatrix();
        }
    }

    void WorldRenderer::updateBlockVisiblity(const ivec3& pos)
    {
        ivec3 chunkPos = World::GetChunkPos(pos);
        uvec3 localPos = World::GetLocalPos(pos);

        Object* chunk = m_world.tryGetChunk(chunkPos);

        if (!chunk) return;

        queueRebuild(*chunk);

        auto& data = chunk->get<ChunkData>();
        auto& renderer = chunk->get<ChunkRenderer>();
        Block& block = data.getBlock(localPos);

        if (block.getDisplay() == BlockDisplay::TRANSPARENT)
        {
            renderer.visibleBlocks.erase(localPos);
            return;
        }

        if (block.getDisplay() != BlockDisplay::OPAQUE)
        {
            renderer.visibleBlocks[localPos] = &block;
            return;
        }

        for (auto [dir, face] : ChunkData::Directions)
        {
            Block* nBlock = m_world.tryGetBlock(ivec3(pos) + dir);

            if (nBlock && nBlock->getDisplay() != BlockDisplay::OPAQUE)
            {
                renderer.visibleBlocks[localPos] = &block;
                return;
            }
        }

        renderer.visibleBlocks.erase(localPos);
    }
}