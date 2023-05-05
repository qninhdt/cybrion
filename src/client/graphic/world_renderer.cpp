#include "world/world.hpp"
#include "client/resource/block_loader.hpp"
#include "client/resource/shader_manager.hpp"
#include "client/graphic/world_renderer.hpp"
#include "client/local_game.hpp"
#include "client/graphic/entity_renderer.hpp"
#include "world/entity/entity.hpp"
#include "client/graphic/basic_mesh_generator.hpp"
#include "core/pool.hpp"
#include "client/application.hpp"

namespace cybrion
{
    WorldRenderer::WorldRenderer(World &world) : m_world(world),
                                                 m_enableAO(true),
                                                 m_enableDiffuse(true),
                                                 m_chunkMeshResults()
    {
        m_basicShader = ShaderManager::Get().getShader<BasicShader>("basic");
        m_opaqueCubeShader = ShaderManager::Get().getShader<OpaqueCubeShader>("opaque_cube");
        m_blockModelShader = ShaderManager::Get().getShader<BlockModelShader>("block_model");
    }

    void WorldRenderer::render(f32 delta, bool showEntityBorder)
    {
        updateEntityRenderers(delta);
        i32 cnt = 0;
        ref<ChunkMeshResult> result;
        while (cnt < 32 && m_chunkMeshResults.try_dequeue(result))
        {
            auto renderer = result->renderer;

            if (renderer->m_chunk->isUnloaded())
                continue;

            // allow to install if version is still latest
            if (renderer->m_version != result->version)
                continue;

            renderer->opaqueMesh.setVertices(result->opaqueVertices.data(), result->opaqueVertices.size());
            renderer->opaqueMesh.setDrawCount(result->opaqueVertices.size() / 4 * 6);

            renderer->transparentMesh.setVertices(result->transparentVertices.data(), result->transparentVertices.size());
            renderer->transparentMesh.setDrawCount(result->transparentVertices.size() / 4 * 6);

            renderer->modelMesh.setVertices(result->modelVertices.data(), result->modelVertices.size());
            renderer->modelMesh.setDrawCount(result->modelVertices.size() / 4 * 6);

            renderer->m_hasBuilt = true;

            ++cnt;
        }

        {
            vector<u32> removeList;
            for (auto &[id, renderer] : m_chunkRenderers)
            {
                if (renderer->m_chunk->isUnloaded() && renderer.use_count() == 1)
                    removeList.push_back(id);
            }
            for (auto &id : removeList)
                m_chunkRenderers.erase(id);
        }

        BlockLoader::Get().bindTextureArray();

        m_opaqueCubeShader.use();
        m_opaqueCubeShader.setUniform<"enable_diffuse">((u32)m_enableDiffuse);
        m_opaqueCubeShader.setUniform<"enable_ao">((u32)m_enableAO);

        vector<ref<ChunkRenderer>> renderChunks;
        for (auto &[id, renderer] : m_chunkRenderers)
        {
            if (!renderer->m_hasBuilt)
            {
                continue;
            }
            renderChunks.push_back(renderer);
        }

        vec3 cameraPos = LocalGame::Get().getCamera().getPos();
        std::sort(renderChunks.begin(), renderChunks.end(), [&](ref<ChunkRenderer> &x, ref<ChunkRenderer> &y)
                  { return glm::distance(cameraPos, x->m_chunk->getPos()) > glm::distance(cameraPos, y->m_chunk->getPos()); });

        for (auto &renderer : renderChunks)
        {
            auto &opaqueMesh = renderer->opaqueMesh;
            auto &transparentMesh = renderer->transparentMesh;

            m_opaqueCubeShader.setUniform<"MVP">(
                LocalGame::Get().getCamera().getProjViewMat() * opaqueMesh.getModelMat());

            // OPAQUE

            if (opaqueMesh.getDrawCount() > 0)
            {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);

                opaqueMesh.drawTriangles();

                glDisable(GL_CULL_FACE);
            }

            // TRANSPARENT
            if (transparentMesh.getDrawCount() > 0)
            {
                glEnable(GL_BLEND);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                transparentMesh.drawTriangles();
                glDisable(GL_BLEND);
            }
        }

        m_blockModelShader.use();
        for (auto &renderer : renderChunks)
        {
            auto &modelMesh = renderer->modelMesh;

            m_blockModelShader.setUniform<"MVP">(
                LocalGame::Get().getCamera().getProjViewMat() * modelMesh.getModelMat());

            if (modelMesh.getDrawCount() > 0)
            {
                modelMesh.drawTriangles();
            }
        }

        if (showEntityBorder)
        {
            m_basicShader.use();
            for (auto &[_, renderer] : m_entityRenderers)
            {
                auto &aabbMesh = renderer->aabbMesh;
                m_basicShader.setUniform<"MVP">(
                    LocalGame::Get().getCamera().getProjViewMat() * aabbMesh.getModelMat());
                aabbMesh.drawLines();
            }
        }
    }

    void WorldRenderer::addChunk(const ref<Chunk> &chunk)
    {
        if (chunk->isUnloaded())
            return;

        auto renderer = std::make_shared<ChunkRenderer>(chunk);
        m_chunkRenderers[chunk->getId()] = renderer;
        prepareRebuild(renderer);

        chunk->eachNeighbors([this](ref<Chunk> &neighbor, const ivec3 &)
                             {
            if (neighbor && neighbor->hasStructure())
            {
                auto renderer = getChunkRenderer(neighbor);

                if (renderer)
                    prepareRebuild(renderer);
            } });
    }

    void WorldRenderer::removeChunk(const ref<Chunk> &chunk)
    {
    }

    void WorldRenderer::addEntity(const ref<Entity> &entity)
    {
        auto renderer = std::make_shared<EntityRenderer>(entity);
        m_entityRenderers[entity->getId()] = renderer;
    }

    ref<ChunkRenderer> WorldRenderer::getChunkRenderer(const ref<Chunk> &chunk) const
    {
        auto it = m_chunkRenderers.find(chunk->getId());
        return it == m_chunkRenderers.end() ? nullptr : it->second;
    }

    ref<EntityRenderer> WorldRenderer::getEntityRenderer(const ref<Entity> &entity) const
    {
        auto it = m_entityRenderers.find(entity->getId());
        return it == m_entityRenderers.end() ? nullptr : it->second;
    }

    void WorldRenderer::prepareRebuild(const ref<ChunkRenderer> &renderer)
    {
        if (renderer->m_chunk->isUnloaded())
            return;

        if (renderer->m_inBuildQueue)
            return;

        renderer->m_inBuildQueue = true;
        renderer->m_version += 1;

        u32 version = renderer->m_version;

        std::ignore = GetPool().submit(
            [this, renderer, version]
            {
                if (!Application::Get().isPlayingGame())
                    return;

                renderer->m_inBuildQueue = false;

                if (renderer->m_chunk->isUnloaded())
                    return;

                auto result = renderer->buildChunkMesh();
                result->renderer = renderer;
                result->version = version;

                if (renderer->m_chunk->isUnloaded())
                    return;

                if (!Application::Get().isPlayingGame())
                    return;

                // allow to install if version is still latest after building
                if (version == renderer->m_version)
                    m_chunkMeshResults.enqueue(result);
            });
    }

    void WorldRenderer::updateBlock(const BlockModifyResult &result)
    {
        /*result.chunk->eachBlockAndNeighbors(Chunk::posToLocalPos(result.pos), [&](Block* block, ref<Chunk>& chunk, const ivec3& dir)
        {
            if (!block) return;

            auto renderer = getChunkRenderer(chunk);

            if (renderer == nullptr)
                return;

            ivec3 pos = result.pos + dir;
            ivec3 localPos = Chunk::posToLocalPos(pos);

            if (renderer->m_chunk->hasStructure())
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
       });*/
    }

    void WorldRenderer::updateChunk(const ref<Chunk> &chunk)
    {
        auto renderer = getChunkRenderer(chunk);

        if (renderer == nullptr)
            return;

        if (renderer->m_chunk->hasStructure())
            prepareRebuild(renderer);
    }

    void WorldRenderer::updateEntityRenderers(f32 delta)
    {
        for (auto &[_, renderer] : m_entityRenderers)
            renderer->tick(delta);
    }
}