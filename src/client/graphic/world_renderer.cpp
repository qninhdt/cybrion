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
        m_enableDiffuse(true)
    {
        m_basicShader = ShaderManager::Get().getShader<BasicShader>("basic");
        m_opaqueCubeShader = ShaderManager::Get().getShader<OpaqueCubeShader>("opaque_cube");
    }

    void WorldRenderer::render(f32 delta, bool showEntityBorder)
    {
        updateEntityRenderers(delta);

        //buildChunkMeshes(0.05f); // allow build meshes in 1 second
        //rebuildChunkMeshes(1); // allow rebuild meshes in 1 second

        vector<ChunkMeshResult> results;

        m_chunkLock.lock();
        if (!m_chunkMeshResults.empty())
            std::cout << m_chunkMeshResults.size() << '\n';
        while (!m_chunkMeshResults.empty() && results.size() < 4)
        {
            results.push_back(m_chunkMeshResults.front());
            m_chunkMeshResults.pop();
        }
        m_chunkLock.unlock();

        for (auto& result : results)
        {  
            auto renderer = result.renderer;
            renderer->opaqueMesh.setDrawCount(result.drawCount);
            renderer->opaqueMesh.setVertices(result.vertices, result.size);
            
            renderer->m_hasBuilt = true;
            renderer->m_queueBuild = false;

            delete[] result.vertices;
        }

        BlockLoader::Get().bindTextureArray();
        m_opaqueCubeShader.use();

        m_opaqueCubeShader.setUniform<"enable_diffuse">((u32)m_enableDiffuse);
        m_opaqueCubeShader.setUniform<"enable_ao">((u32)m_enableAO);
        for (auto& [id, renderer]: m_chunkRenderers)
        {
            if (renderer->m_queueBuild || id % 31 != 0) continue;
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

        /*renderer->inBuildQueue = true;
        m_buildChunkQueue.push(renderer);*/

        prepareRebuild(renderer);

 /*       chunk->eachNeighbors([&](ref<Chunk>& neighbor, const ivec3&) {
            if (neighbor)
            {
                auto neighborRenderer = getChunkRenderer(neighbor);
                prepareRebuild(neighborRenderer);
            }
        });*/
    }

    void WorldRenderer::addEntity(const ref<Entity>& entity)
    {
        auto renderer = std::make_shared<EntityRenderer>(entity);
        m_entityRenderers[entity->getId()] = renderer;
    }

    void WorldRenderer::buildChunkMeshes(f32 maxDuration)
    {
        m_stopwatch.reset();
        while (!m_buildChunkQueue.empty() && maxDuration > m_stopwatch.getDeltaTime())
        {
            auto renderer = m_buildChunkQueue.front();
            m_buildChunkQueue.pop();

            renderer->buildChunkMesh();
            renderer->inBuildQueue = false;
        }
    }

    void WorldRenderer::rebuildChunkMeshes(f32 maxDuration)
    {
        for (auto& renderer: m_rebuildChunkList)
        {
            renderer->rebuildChunkMesh();
            renderer->inRebuildList = false;
        }

        m_rebuildChunkList.clear();
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
        /*if (!renderer->inBuildQueue && !renderer->inRebuildList)
        {
            m_rebuildChunkList.push_back(renderer);
            renderer->inRebuildList = true;
        }*/
        /// FIXME: multiple rebuild mesh ?
        GetPool().submit([this, renderer = renderer] {
            auto result = renderer->buildChunkMesh();
            result.renderer = renderer;
            renderer->m_queueBuild = true;
            m_chunkLock.lock();
            m_chunkMeshResults.push(result);
            m_chunkLock.unlock();
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