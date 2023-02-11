#include "world/world.hpp"
#include "client/application.hpp"

namespace cybrion
{
    CubeVertex WorldRenderer::s_opaqueVertices[400000];

    WorldRenderer::WorldRenderer(World& world):
        m_registry(GetRegistry()),
        m_world(world)
    {
        m_registry
            .on_construct<ChunkData>()
            .connect<&WorldRenderer::onChunkCreated>(*this);

        m_opaqueCubeShader = Application::Get()
            .getShaderManager()
            .getShader<OpaqueCubeShader>("opaque_cube");
    }

    void WorldRenderer::render(f32 deltaTime)
    {
        ClientGame::Get().getBlockLoader().bindTextureArray();
        m_opaqueCubeShader.use();

        for (auto&& [entity, renderer] : m_registry.view<ChunkRenderer>().each())
        {
            auto& [opaqueMesh, isDirty] = renderer;
            m_opaqueCubeShader.setUniform<"MVP">(
                ClientGame::Get().getCamera().getProjectionViewMatrix()
            );
            opaqueMesh.drawTriangles();
        }
    }

    void WorldRenderer::onChunkCreated(entt::registry&, entt::entity entity)
    {
        Entity chunk(entity);
        m_buildMeshQueue.push(chunk);

        CYBRION_CLIENT_TRACE("Add chunk to mesh queue");
    }

    void WorldRenderer::buildChunkMeshes(f32 maxDuration)
    {
        m_stopwatch.reset();
        while (!m_buildMeshQueue.empty() && maxDuration > m_stopwatch.getDeltaTime())
        {
            Entity chunk = m_buildMeshQueue.front();
            m_buildMeshQueue.pop();

            buildChunkMesh(chunk);
        }
    }

    void WorldRenderer::buildChunkMesh(Entity& chunk)
    {
        m_stopwatch.reset();

        auto& [opaqueMesh, isDirty] = chunk.assign<ChunkRenderer>();
        auto& data = chunk.get<ChunkData>();

        auto eastData = data.eastChunk.valid() ? &data.eastChunk.get<ChunkData>() : nullptr;
        auto topData = data.topChunk.valid() ? &data.topChunk.get<ChunkData>() : nullptr;
        auto southData = data.southChunk.valid() ? &data.southChunk.get<ChunkData>() : nullptr;
        auto westData = data.westChunk.valid() ? &data.westChunk.get<ChunkData>() : nullptr;
        auto bottomData = data.bottomChunk.valid() ? &data.bottomChunk.get<ChunkData>() : nullptr;
        auto northData = data.northChunk.valid() ? &data.northChunk.get<ChunkData>() : nullptr;

        u32 opaqueSize = 0;

        bool culling[6] = { 0 };
        for (u32 x = 0; x < CHUNK_SIZE; ++x)
        {
            for (u32 y = 0; y < CHUNK_SIZE; ++y)
            {
                for (u32 z = 0; z < CHUNK_SIZE; ++z)
                {
                    uvec3 pos{ x, y, z };
                    Block& block = data.getBlock(pos);
                    BlockDisplay display = block.getDisplay();
                    auto& cubeRenderer = ClientGame::Get().getBlockRenderer(block.getId());

                    // dont render transparent block like air
                    if (display == BlockDisplay::TRANSPARENT)
                        continue;

                    for (auto& [dir, face] : BlockRenderer::CubeDirections)
                    {
                        Block* neighbor = nullptr;
                        ivec3 npos = ivec3(pos) + dir;

                        if (npos.x < 0) { if (eastData) neighbor = &eastData->getBlock({ 1, npos.y, npos.z }); }
                        else if (npos.y < 0) { if (bottomData) neighbor = &bottomData->getBlock({ npos.x, 1, npos.z }); }
                        else if (npos.z < 0) { if (northData) neighbor = &northData->getBlock({ npos.x, npos.y, 1 }); }
                        else if (npos.x >= CHUNK_SIZE) { if (westData) neighbor = &westData->getBlock({ 31, npos.y, npos.z }); }
                        else if (npos.y >= CHUNK_SIZE) { if (topData) neighbor = &topData->getBlock({ npos.x, 31, npos.z }); }
                        else if (npos.z >= CHUNK_SIZE) { if (southData) neighbor = &southData->getBlock({ npos.x, npos.y, 31 }); }

                        else
                            neighbor = &data.getBlock(npos);

                        // cull this face when neighbor block is opaque
                        culling[u32(face)] = neighbor && neighbor->getDisplay() == BlockDisplay::OPAQUE;
                    }

                    cubeRenderer.generateCubeMesh(culling, vec3(pos), s_opaqueVertices, opaqueSize);
                }
            }
        }

        opaqueMesh.setVertices(s_opaqueVertices, opaqueSize);
        opaqueMesh.setDrawCount(opaqueSize / 4 * 6);
        CYBRION_CLIENT_TRACE("Built chunk mesh");

        std::cout << m_stopwatch.getDeltaTime() << '\n';
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