#include "world/world.hpp"
#include "client/resource/block_loader.hpp"
#include "client/resource/shader_manager.hpp"
#include "client/graphic/world_renderer.hpp"
#include "client/local_game.hpp"
#include "client/graphic/entity_renderer.hpp"
#include "world/entity/entity.hpp"

namespace cybrion
{
    CubeVertex WorldRenderer::s_opaqueVertices[400000];

    WorldRenderer::WorldRenderer(World& world):
        m_registry(GetRegistry()),
        m_world(world)
    {
        m_opaqueCubeShader = ShaderManager::Get().getShader<OpaqueCubeShader>("opaque_cube");
    }

    void WorldRenderer::render(f32 deltaTime)
    {
        BlockLoader::Get().bindTextureArray();
        m_opaqueCubeShader.use();

        for (auto&& [Object, renderer] : m_registry.view<ChunkRenderer>().each())
        {
            auto& opaqueMesh = renderer.opaqueMesh;
            m_opaqueCubeShader.setUniform<"MVP">(
                LocalGame::Get().getCamera().getProjectionViewMatrix()
                * opaqueMesh.getModelMatrix()
            );
            opaqueMesh.drawTriangles();
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
                    m_rebuildMeshQueue.push(data.neighbors[i]);
                }
            }
        }
    }

    void WorldRenderer::setupEntity(Object entity)
    {
        auto& renderer = entity.assign<EntityRenderer>();
    }

    void WorldRenderer::buildChunkMeshes(f32 maxDuration)
    {
        m_stopwatch.reset();
        while (!m_buildMeshQueue.empty() && maxDuration > m_stopwatch.getDeltaTime())
        {
            Object chunk = m_buildMeshQueue.front();
            m_buildMeshQueue.pop();

            buildChunkMesh(chunk);
        }
    }

    void WorldRenderer::buildChunkMesh(Object& chunk)
    {
        Stopwatch stopwatch;
        stopwatch.reset();

        auto& [opaqueMesh, visibleBlocks, isDirty] = chunk.assign<ChunkRenderer>();
        auto& data = chunk.get<ChunkData>();

        auto neighbors = getNeighborChunkData(data);

        u32 opaqueSize = 0;
        bool culling[6] = { 0 };
        vec3 align = vec3(0.5f, 0.5f, 0.5f) - vec3(CHUNK_SIZE / 2, CHUNK_SIZE / 2, CHUNK_SIZE / 2);

        for (u32 x = 0; x < CHUNK_SIZE; ++x)
        {
            for (u32 y = 0; y < CHUNK_SIZE; ++y)
            {
                for (u32 z = 0; z < CHUNK_SIZE; ++z)
                {
                    uvec3 pos{ x, y, z };
                    Block& block = data.getBlock(pos);
                    BlockDisplay display = block.getDisplay();
                    auto& cubeRenderer = LocalGame::Get().getBlockRenderer(block.getId());

                    // dont render transparent blocks like air
                    if (display == BlockDisplay::TRANSPARENT)
                        continue;

                    bool visible = false;

                    // blocks that are not visible now because their neighbors are not loaded
                    // but may be visible when their neighbors are loaded
                    bool maybeVisible = false; 

                    for (auto& [dir, face] : BlockRenderer::CubeDirections)
                    {
                        Block* neighbor = nullptr;
                        ivec3 npos = ivec3(pos) + dir;

                        if (!getBlockIfIsOutside(npos, &neighbor, neighbors))
                            neighbor = &data.getBlock(uvec3(npos));

                        // cull this face when neighbor block is opaque
                        culling[u32(face)] = !neighbor || (neighbor && neighbor->getDisplay() == BlockDisplay::OPAQUE);
                        visible |= !culling[u32(face)];
                        maybeVisible |= !neighbor;
                    }

                    if (visible)
                        cubeRenderer.generateCubeMesh(culling, vec3(pos) + align, s_opaqueVertices, opaqueSize);

                    if (visible || maybeVisible)
                        visibleBlocks[pos] = &block;
                }
            }
        }

        opaqueMesh.setPosition(data.getWorldPosition());
        opaqueMesh.updateModelMatrix();

        opaqueMesh.setVertices(s_opaqueVertices, opaqueSize);
        opaqueMesh.setDrawCount(opaqueSize / 4 * 6);
        CYBRION_CLIENT_TRACE("Built chunk mesh   : {}", stopwatch.getDeltaTime());
        //m_rebuildMeshQueue.push(chunk);
    }

    void WorldRenderer::rebuildChunkMesh(Object& chunk)
    {
        Stopwatch stopwatch;
        stopwatch.reset();

        auto& [opaqueMesh, visibleBlocks, isDirty] = chunk.get<ChunkRenderer>();
        auto& data = chunk.get<ChunkData>();

        auto neighbors = getNeighborChunkData(data);

        u32 opaqueSize = 0;
        bool culling[6] = { 0 };
        vec3 align = vec3(0.5f, 0.5f, 0.5f) - vec3(CHUNK_SIZE/2, CHUNK_SIZE/2, CHUNK_SIZE/2);
        for (auto& [pos, block] : visibleBlocks)
        {
            auto& cubeRenderer = LocalGame::Get().getBlockRenderer(block->getId());

            for (auto& [dir, face] : BlockRenderer::CubeDirections)
            {
                Block* neighbor = nullptr;
                ivec3 npos = ivec3(pos) + dir;

                if (!getBlockIfIsOutside(npos, &neighbor, neighbors))
                    neighbor = &data.getBlock(uvec3(npos));

                culling[u32(face)] = !neighbor || (neighbor && neighbor->getDisplay() == BlockDisplay::OPAQUE);
            }

            cubeRenderer.generateCubeMesh(culling, vec3(pos) + align, s_opaqueVertices, opaqueSize);
        }

        opaqueMesh.setVertices(s_opaqueVertices, opaqueSize);
        opaqueMesh.setDrawCount(opaqueSize / 4 * 6);
        CYBRION_CLIENT_TRACE("Rebuilt chunk mesh : {}", stopwatch.getDeltaTime());
    }

    void WorldRenderer::rebuildChunkMeshes(f32 maxDuration)
    {
        m_stopwatch.reset();

        while (!m_rebuildMeshQueue.empty() && maxDuration > m_stopwatch.getDeltaTime())
        {
            Object chunk = m_rebuildMeshQueue.front();
            m_rebuildMeshQueue.pop();

            rebuildChunkMesh(chunk);
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

            renderer.mesh.setRotation(
                util::LerpRotaion(
                    data.oldTransform.getRotation(),
                    data.transform.getRotation(), lerpFactor
                 )
            );

            renderer.mesh.updateModelMatrix();
        }
    }

    std::array<ChunkData*, 6> WorldRenderer::getNeighborChunkData(ChunkData& data)
    {
        std::array<ChunkData*, 6> result;
        
        for (u32 i = 0; i < 6; ++i)
            result[i] = data.neighbors[i].valid() ? &data.neighbors[i].get<ChunkData>() : nullptr;
        
        return result;
    }

    bool WorldRenderer::getBlockIfIsOutside(const ivec3& pos, Block** block, std::array<ChunkData*, 6>& neighbors)
    {
        if (pos.x < 0) {
            if (neighbors[3]) *block = &neighbors[3]->getBlock({ 31, pos.y, pos.z });
            return true;
        } else if (pos.y < 0) { 
            if (neighbors[4]) *block = &neighbors[4]->getBlock({ pos.x, 31, pos.z });
            return true;
        } else if (pos.z < 0) {
            if (neighbors[5]) *block = &neighbors[5]->getBlock({ pos.x, pos.y, 31 });
            return true;
        } else if (pos.x >= CHUNK_SIZE) {
            if (neighbors[0]) *block = &neighbors[0]->getBlock({ 0, pos.y, pos.z });
            return true;
        } else if (pos.y >= CHUNK_SIZE) {
            if (neighbors[1]) *block = &neighbors[1]->getBlock({ pos.x, 0, pos.z });
            return true;
        } else if (pos.z >= CHUNK_SIZE) {
            if (neighbors[2]) *block = &neighbors[2]->getBlock({ pos.x, pos.y, 0 });
            return true;
        }
        return false;
    }

    ChunkRenderer::ChunkRenderer():
        opaqueMesh(true),
        isDirty(false)
    {
        opaqueMesh.setAttributes({
            { GL::Type::VEC3 },
            { GL::Type::VEC2 },
            { GL::Type::UINT }
        });
    }
}