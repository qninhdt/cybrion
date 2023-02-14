#include "world/world.hpp"
#include "world/entity/entity.hpp"
#include "game.hpp"

namespace cybrion
{
    World::World() :
        m_registry(GetRegistry())
    {}

    Object World::spawnEntity(const vec3& pos)
    {
        Object entity(m_registry.create());
        auto& data = entity.assign<EntityData>(pos);

        Game::Get().onEntitySpawned(entity);

        return entity;
    }

    Object World::loadChunk(const ivec3& pos)
    {
        Object chunk(m_registry.create());
        auto& data = chunk.assign<ChunkData>();

        data.position = pos;
        for (auto& [dir, face] : ChunkData::Directions)
        {
            ivec3 npos = dir + pos;
            auto it = m_chunkMap.find(npos);
            
            if (it != m_chunkMap.end())
            {
                Object& neighbor = it->second;
                auto& neighborData = neighbor.get<ChunkData>();

                data.neighbors[(u32)face] = neighbor;
                neighborData.neighbors[(u32)Block::GetOppositeFace(face)] = chunk;
            }
        }
 
        LogBlock& log = (LogBlock&)Game::Get().getBlockRegistry().getBlock(BlockType::LOG);
        SoilBlock& dirt = (SoilBlock&)Game::Get().getBlockRegistry().getBlock(BlockType::SOIL);

        for (u32 x = 0; x < CHUNK_SIZE; ++x)
        {
            for (u32 z = 0; z < CHUNK_SIZE; ++z)
            {
                int height = sqrt((x*x+z*z)/2);

                for (u32 y = 0; y < height; ++y)
                {
                    if (sin(x) > cos(y))
                        data.setBlock({ x, y, z }, dirt);
                    else
                        data.setBlock({ x, y, z }, log.set<"axis">((x + y + z) % 3));
                }
            }
        }

        m_chunkMap[pos] = chunk;

        Game::Get().onChunkLoaded(chunk);

        return chunk;
    }

    Object World::getChunk(const ivec3& pos)
    {
        return m_chunkMap[pos];
    }

    Object* World::tryGetChunk(const ivec3& pos)
    {
        auto it = m_chunkMap.find(pos);
        if (it == m_chunkMap.end())
            return nullptr;

        return &it->second;
    }

    void World::tick()
    {
        // store old entity transforms
        for (auto&& [_, data] : m_registry.view<EntityData>().each())
        {
            data.oldTransform = data.transform;
        }

        updateEntityTransforms();
    }

    Block& World::getBlock(const ivec3& pos)
    {
        Block* block = tryGetBlock(pos);
        assert(block != nullptr);
        return *block;
    }

    Block* World::tryGetBlock(const ivec3& pos)
    {
        ivec3 chunkPos = GetChunkPos(pos);
        uvec3 localPos = GetLocalPos(pos);

        auto it = m_chunkMap.find(chunkPos);

        if (it == m_chunkMap.end()) return nullptr;

        auto& chunkData = it->second.get<ChunkData>();
        return &chunkData.getBlock(localPos);
    }

    tuple<Object&, Block&> World::setBlock(const ivec3& pos, Block& block)
    {
        ivec3 chunkPos = GetChunkPos(pos);
        uvec3 localPos = GetLocalPos(pos);

        auto it = m_chunkMap.find(chunkPos);

        if (it == m_chunkMap.end())
        {
            CYBRION_GAME_ERROR("Cannot change block at ({}, {}, {})", pos.x, pos.y, pos.z);
        }

        Object chunk = it->second;
        auto& data = chunk.get<ChunkData>();
        Block& oldBlock = data.getBlock(localPos);
        data.setBlock(localPos, block);

        Game::Get().onBlockChanged(chunk, pos, block, oldBlock);

        return { chunk, oldBlock };
    }

    void World::placeBlock(const ivec3& pos, BlockFace face, Block& block)
    {
        ivec3 ppos = pos + Block::GetDirectionFromFace(face);
        Block& placedBlock = block.getPlacedBlock(pos, face);
        auto [chunk, _] = setBlock(ppos, placedBlock);
        Game::Get().onPlaceBlock(chunk, ppos, placedBlock, face);
    }

    void World::breakBlock(const ivec3& pos)
    {
        auto [chunk, block] = setBlock(pos, BlockRegistry::Get().getBlock(BlockType::AIR));
        Game::Get().onBreakBlock(chunk, pos, block);
    }

    void World::updateEntityTransforms()
    {
        auto view = m_registry.view<EntityData>();

        for (auto&& [entity, data] : view.each())
        {
            AABB box = { { 16, 16, 16 }, { 32, 32, 32 } };
            auto [delta, normal] = AABB::SweptAABB(data.getWorldAABB(), box, data.velocity);

            data.transform.move(data.velocity * delta);

            if (delta < 1.0f)
            {
                f32 angle = glm::dot(glm::normalize(data.velocity), normal);
                
                if (1.0f - abs(angle) < std::numeric_limits<f32>::epsilon()) continue;

                // slide
                vec3 slideDir = - glm::normalize(glm::cross(glm::cross(data.velocity, normal), normal));
                vec3 slideVec = slideDir * glm::dot(slideDir, data.velocity) * (1 - delta);
                data.transform.move(slideVec);
            }
        }
    }

    ivec3 World::GetChunkPos(const ivec3& pos)
    {
        return { pos.x >> LOG_2_CHUNK_SIZE, pos.y >> LOG_2_CHUNK_SIZE, pos.z >> LOG_2_CHUNK_SIZE };
    }

    uvec3 World::GetLocalPos(const ivec3& pos)
    {
        return { pos.x & (CHUNK_SIZE - 1), pos.y & (CHUNK_SIZE - 1), pos.z & (CHUNK_SIZE - 1) };
    }

}
