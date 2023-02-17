#include "world/world.hpp"
#include "client/resource/block_loader.hpp"
#include "client/resource/shader_manager.hpp"
#include "client/graphic/world_renderer.hpp"
#include "client/local_game.hpp"
#include "client/graphic/entity_renderer.hpp"
#include "world/entity/entity.hpp"
#include "client/graphic/basic_mesh_generator.hpp"
#include "core/pool.hpp"

namespace cybrion
{
    WorldRenderer::WorldRenderer(World& world):
        m_world(world),
        m_enableAO(true),
        m_enableDiffuse(true),
        m_chunkMeshResults()
    {
        m_basicShader = ShaderManager::Get().getShader<BasicShader>("basic");
        m_opaqueCubeShader = ShaderManager::Get().getShader<OpaqueCubeShader>("opaque_cube");
    }

    void WorldRenderer::render(f32 delta, bool showEntityBorder)
    {
        updateEntityRenderers(delta);
        i32 cnt = 0;
        ref<ChunkMeshResult> result;
        while ((cnt++) < 8 && m_chunkMeshResults.try_dequeue(result))
        {
            auto renderer = result->renderer;

            // allow to install if version is still latest
            if (renderer->m_version != result->version)
                continue;

            renderer->opaqueMesh.setVertices(result->vertices.data(), result->vertices.size());
            renderer->opaqueMesh.setDrawCount(result->vertices.size() / 4 * 6);
            renderer->m_hasBuilt = true;
        }

        BlockLoader::Get().bindTextureArray();
        m_opaqueCubeShader.use();

        m_opaqueCubeShader.setUniform<"enable_diffuse">((u32)m_enableDiffuse);
        m_opaqueCubeShader.setUniform<"enable_ao">((u32)m_enableAO);
        for (auto& [id, renderer]: m_chunkRenderers)
        {
            if (!renderer->m_hasBuilt || renderer->opaqueMesh.getDrawCount() == 0) {
                continue;
            }
            auto& opaqueMesh = renderer->opaqueMesh;
            m_opaqueCubeShader.setUniform<"MVP">(
                LocalGame::Get().getCamera().getProjViewMat()
                * opaqueMesh.getModelMat()
            );

            opaqueMesh.drawTriangles();
        }

        if (showEntityBorder)
        {
            m_basicShader.use();
            for (auto& [_, renderer]: m_entityRenderers)
            {
                auto& aabbMesh = renderer->aabbMesh;
                m_basicShader.setUniform<"MVP">(
                    LocalGame::Get().getCamera().getProjViewMat()
                    * aabbMesh.getModelMat()
                );
                aabbMesh.drawLines();
            }
        }
    }

    void WorldRenderer::addChunk(const ref<Chunk>& chunk)
    {
        auto renderer = std::make_shared<ChunkRenderer>(chunk);
        m_chunkRenderers[chunk->getId()] = renderer;
        prepareRebuild(renderer);

        chunk->eachNeighbors([this](ref<Chunk>& neighbor, const ivec3&) {
            if (neighbor)
                prepareRebuild(getChunkRenderer(neighbor));
        });
    }

    void WorldRenderer::addEntity(const ref<Entity>& entity)
    {
        auto renderer = std::make_shared<EntityRenderer>(entity);
        m_entityRenderers[entity->getId()] = renderer;
    }

    ref<ChunkRenderer> WorldRenderer::getChunkRenderer(const ref<Chunk>& chunk) const
    {
        auto it = m_chunkRenderers.find(chunk->getId());
        return it == m_chunkRenderers.end() ? nullptr : it->second;
    }

    ref<EntityRenderer> WorldRenderer::getEntityRenderer(const ref<Entity>& entity) const
    {
        auto it = m_entityRenderers.find(entity->getId());
        return it == m_entityRenderers.end() ? nullptr : it->second;
    }

    void WorldRenderer::prepareRebuild(const ref<ChunkRenderer>& renderer)
    {
        if (renderer->m_inBuildQueue) {
            return;
        }

        renderer->m_inBuildQueue = true;
        renderer->m_version += 1;

        u32 version = renderer->m_version;

        GetPool().submit([this, renderer, version] {
            renderer->m_inBuildQueue = false;

            auto result = renderer->buildChunkMesh();
            result->renderer = renderer;
            result->version = version;

            // allow to install if version is still latest after building
            if (version == renderer->m_version)
                m_chunkMeshResults.enqueue(result);
        });

    }

    void WorldRenderer::updateBlock(const BlockModifyResult& result)
    {
        result.chunk->eachBlockAndNeighbors(Chunk::posToLocalPos(result.pos), [&](Block* block, ref<Chunk>& chunk, const ivec3& dir) 
        {
            if (!block) return;

            auto renderer = getChunkRenderer(chunk);
            ivec3 pos = result.pos + dir;
            ivec3 localPos = Chunk::posToLocalPos(pos);

            prepareRebuild(renderer);

            if (block->getDisplay() == BlockDisplay::TRANSPARENT)
            {
                renderer->visibleBlocks.erase(localPos);
                return;
            }

            if (block->getDisplay() != BlockDisplay::OPAQUE)
            {
                renderer->visibleBlocks[localPos] = block;
                return;
            }

            for (auto [dir, face] : Block::Directions)
            {
                Block* nBlock = m_world.tryGetBlock(pos + dir);

                if (nBlock && nBlock->getDisplay() != BlockDisplay::OPAQUE)
                {
                    renderer->visibleBlocks[localPos] = block;
                    return;
                }
            }

            renderer->visibleBlocks.erase(localPos);
       });
    }

    void WorldRenderer::updateEntityRenderers(f32 delta)
    {
        for (auto& [_, renderer ]: m_entityRenderers)
            renderer->tick(delta);
    }
}