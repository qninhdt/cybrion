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

    void World::loadChunk(const ivec3& pos)
    {
        if (m_chunkMap.find(pos) != m_chunkMap.end())
            return;

        ref<Chunk> chunk = std::make_shared<Chunk>(pos);
        m_chunkMap[pos] = chunk;

        GetPool().submit([this, chunk] {
            if (chunk->isUnloaded())
                return;

            m_generator.generateChunkAt(chunk);

            if (chunk->isUnloaded())
                return;

            m_loadChunkResults.enqueue(chunk);
        });
    }

    void World::unloadChunk(const ivec3& pos)
    {
        auto it = m_chunkMap.find(pos);

        if (it == m_chunkMap.end())
            return;

        ref<Chunk> chunk = it->second;
        chunk->m_unloaded = true;

        Game::Get().onChunkUnloaded(chunk);

        // remove neighbors
        chunk->eachNeighbors([&](ref<Chunk>& neighbor, const ivec3& dir) {
            if (neighbor)
            {
                neighbor->setNeighbor(-dir, nullptr);
                chunk->setNeighbor(dir, nullptr);
            }
        });
        chunk->setNeighbor({ 0, 0, 0 }, nullptr);

        m_chunkMap.erase(it);
    }

    ref<Chunk> World::getChunk(const ivec3& pos)
    {
        auto it = m_chunkMap.find(pos);
        if (it == m_chunkMap.end()) return nullptr;
        return it->second;
    }

    void World::tick()
    {
        ref<Chunk> chunk;
        while (m_loadChunkResults.try_dequeue(chunk))
        {
            if (chunk->isUnloaded())
                continue;

            chunk->m_ready = true;

            chunk->m_id = ++chunkId;
            chunk->eachNeighbors([&](ref<Chunk>&, const ivec3& dir) {
                ref<Chunk> neighbor = getChunk(chunk->getChunkPos() + dir);

                if (neighbor && neighbor->isReady())
                {
                    chunk->setNeighbor(dir, neighbor);
                    neighbor->setNeighbor(-dir, chunk);
                }

                if (neighbor && neighbor->isReady() && neighbor->areAllNeighborsReady() && !neighbor->m_hasStructure)
                {
                    m_generator.generateStructure(neighbor);
                    Game::Get().onChunkLoaded(neighbor);
                }
            });

            chunk->setNeighbor({ 0, 0, 0 }, chunk);

            if (chunk->areAllNeighborsReady())
            {
                m_generator.generateStructure(chunk);
                Game::Get().onChunkLoaded(chunk);
            }
        }

        for (auto& entity : m_entities)
            entity->setOldPosAndRot();

        for (auto& entity : m_entities)
            entity->tick();
        
        updateEntityTransforms();

        i32 d = 4;
        ivec3 ppos = Game::Get().getPlayer().getEntity()->getChunkPos();

        for (i32 x = -d; x < d; ++x)
            for (i32 y = -2; y <= 6; ++y)
                for (i32 z = -d; z < d; ++z)
                    if (x*x + z*z <= d*d)
                        loadChunk({ x + ppos.x, y, z + ppos.z });

        vector<ivec3> unloadLists;
        for (auto& [pos, chunk] : m_chunkMap)
        {
            i32 dx = pos.x - ppos.x;
            i32 dz = pos.z - ppos.z;
            if (dx * dx + dz * dz > d * d)
                unloadLists.push_back(pos);
        }

        for (auto& pos : unloadLists)
            unloadChunk(pos);
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

        if (it == m_chunkMap.end()) return { nullptr, {0, 0, 0}, Blocks::AIR, Blocks::AIR };
        //CYBRION_ASSERT(it != m_chunkMap.end(), "Cannot change block because chunk is not loaded");

        auto chunk = it->second;

        Block& oldBlock = chunk->getBlock(localPos);

        if (oldBlock == block)
            return { chunk, pos, block, block };

        chunk->setBlock(localPos, block);

        oldBlock.onBroken(pos);
        block.onPlaced(pos);

        BlockModifyResult result{ chunk, pos, oldBlock, block };

        if (chunk->m_hasStructure)
        {
            Game::Get().onBlockChanged(result);
        }

        return result;
    }

    void World::setBlockOnly(const ivec3& pos, Block& block)
    {
        ivec3 chunkPos = Chunk::posToChunkPos(pos);
        uvec3 localPos = Chunk::posToLocalPos(pos);

        auto it = m_chunkMap.find(chunkPos);

        if (it == m_chunkMap.end()) return;

        it->second->setBlock(localPos, block);
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

    /// TODO: optimize AABB
    void World::updateEntityTransforms()
    {
        for (auto& entity : m_entities)
        {
            AABB bb = entity->getBB();
            vec3 size = bb.getSize();
            vec3 ppos = bb.getPos();
            vec3 pos = ppos + entity->getVelocity();
            AABB bound = AABB::mergeAABB({ ppos, size }, { pos, size });

            while (true)
            {
                SweptAABBResult result{ 1, { 0, 0, 0 } };

                ivec3 min = glm::floor(bound.getMin());
                ivec3 max = glm::ceil(bound.getMax());

                vec3 v = pos - ppos;

                for (i32 x = min.x; x <= max.x; ++x)
                    for (i32 y = min.y; y <= max.y; ++y)
                        for (i32 z = min.z; z <= max.z; ++z)
                        {
                            Block* block = tryGetBlock({ x, y, z });
                            if (!block || block->getType() == BlockType::AIR || block->getType() == BlockType::WATER) continue;

                            auto current = AABB::sweptAABB({ ppos, size },
                                { vec3(x + 0.5f,y + 0.5f,z + 0.5f), {0.8,0.8,0.8} }, v);

                            if (current.delta < result.delta)
                                result = current;
                        }

                pos = ppos + (v * result.delta) + 0.001f * result.normal;

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
