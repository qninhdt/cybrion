#include "world/world.hpp"
#include "game.hpp"
#include "core/pool.hpp"

namespace cybrion
{

    World::World()
    {}

    ref<Entity> World::spawnEntity(const vec3& pos)
    {
        auto entity = std::make_shared<Entity>(pos);
        m_entities.push_back(entity);

        Game::Get().onEntitySpawned(entity);

        return entity;
    }

    ref<Chunk> World::loadChunk(const ivec3& pos)
    {
        GetPool().submit([this, pos] {
            auto chunk = m_generator.generateChunkAt(pos);
            m_chunkLock.lock();
            m_loadChunkResults.push(chunk);
            m_chunkLock.unlock();
        });

        return nullptr;
    }

    ref<Chunk> World::getChunk(const ivec3& pos)
    {
        auto it = m_chunkMap.find(pos);
        if (it == m_chunkMap.end()) return nullptr;
        return it->second;
    }

    void World::tick()
    {
        vector<ref<Chunk>> results;

        m_chunkLock.lock();
        while (!m_loadChunkResults.empty() && results.size() < 16)
        {
            results.push_back(m_loadChunkResults.front());
            m_loadChunkResults.pop();
        }
        m_chunkLock.unlock();

        for (auto& chunk : results)
        {
            chunk->m_id = ++chunkId;
            chunk->eachNeighbors([&](ref<Chunk>&, const ivec3& dir) {
                ref<Chunk> neighbor = getChunk(chunk->getChunkPos() + dir);
                chunk->setNeighbor(dir, neighbor);

                if (neighbor)
                    neighbor->setNeighbor(-dir, chunk);
            });

            chunk->setNeighbor({ 0, 0, 0 }, chunk);
            m_chunkMap[chunk->getChunkPos()] = chunk;
            Game::Get().onChunkLoaded(chunk);
        }

        for (auto& entity : m_entities)
            entity->setOldPosAndRot();

        for (auto& entity : m_entities)
            entity->tick();

        updateEntityTransforms();
    }

    Block& World::getBlock(const ivec3& pos)
    {
        Block* block = tryGetBlock(pos);
        CYBRION_ASSERT(block != nullptr, "Block is invalid or not loaded");
        return *block;
    }

    Block* World::tryGetBlock(const ivec3& pos)
    {
        ivec3 chunkPos = Chunk::posToChunkPos(pos);
        uvec3 localPos = Chunk::posToLocalPos(pos);

        auto it = m_chunkMap.find(chunkPos);
        if (it == m_chunkMap.end()) return nullptr;

        return it->second->tryGetBlock(localPos);
    }

    BlockModifyResult World::setBlock(const ivec3& pos, Block& block)
    {
        ivec3 chunkPos = Chunk::posToChunkPos(pos);
        uvec3 localPos = Chunk::posToLocalPos(pos);

        auto it = m_chunkMap.find(chunkPos);

        CYBRION_ASSERT(it != m_chunkMap.end(), "Cannot change block because chunk is not loaded");

        auto chunk = it->second;

        Block& oldBlock = chunk->getBlock(localPos);
        chunk->setBlock(localPos, block);

        BlockModifyResult result{ chunk, pos, oldBlock, block };

        Game::Get().onBlockChanged(result);

        return result;
    }

    BlockModifyResult World::placeBlock(const ivec3& pos, BlockFace face, Block& block)
    {
        ivec3 ppos = pos + Block::GetDirectionFromFace(face);
        Block& placedBlock = block.getPlacedBlock(pos, face);
        auto result = setBlock(ppos, placedBlock);
        Game::Get().onPlaceBlock(result);

        return result;
    }

    BlockModifyResult World::breakBlock(const ivec3& pos)
    {
        auto result = setBlock(pos, Blocks::AIR);
        Game::Get().onBreakBlock(result);
        
        return result;
    }

    void World::updateEntityTransforms()
    {
        for (auto& entity : m_entities)
        {
            AABB bb = entity->getBB();
            vec3 size = bb.getSize();
            vec3 ppos = bb.getPos();
            vec3 pos = ppos + entity->getVelocity();

            while (true)
            {
                SweptAABBResult result{ 1, { 0, 0, 0 } };

                ivec3 min = glm::floor(ppos) - 2.0f;
                ivec3 max = glm::floor(ppos) + 2.0f;

                vec3 v = pos - ppos;

                for (i32 x = min.x; x <= max.x; ++x)
                    for (i32 y = min.y; y <= max.y; ++y)
                        for (i32 z = min.z; z <= max.z; ++z)
                        {
                            Block* block = tryGetBlock({ x, y, z });
                            if (!block || block->getType() == BlockType::AIR) continue;

                            auto current = AABB::SweptAABB({ ppos, size },
                                { vec3(x + 0.5f,y + 0.5f,z + 0.5f), {0.5,0.5,0.5} }, v);

                            if (current.delta < result.delta)
                                result = current;
                        }

                pos = ppos + (v * result.delta) + 0.000001f * result.normal;

                if (result.delta == 1.0f) break;

                f32 NdotN = glm::dot(result.normal, result.normal);
                if (NdotN != 0)
                {
                    ppos = pos;

                    f32 VdotN = glm::dot(result.normal, v);
                    pos += (1 - result.delta) * v - (1 - result.delta) * (VdotN / NdotN) * result.normal;
                }
            }

            /// FIXME: should be setAABBposition()
            entity->setPos(pos);
        }
    }
}
