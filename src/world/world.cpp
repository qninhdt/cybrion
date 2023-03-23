#include "world/world.hpp"
#include "game.hpp"
#include "core/pool.hpp"
#include "client/application.hpp"

namespace cybrion
{

    World::World(const string& name):
        m_name(name)
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

        ivec3 regionPos = ToRegionPos(pos);
        ivec3 localPos = ToLocalRegionPos(pos);

        loadRegion(regionPos);

        auto region = m_regionMap[regionPos];
        u32 chunkId = localPos.x * 32 * 32 + localPos.y * 32 + localPos.z;

        if (region->has(chunkId))
        {
            jbt::tag tag;
            region->read(chunkId, tag);
            chunk->fromJBT(tag);
            chunk->m_isNewChunk = false;
            m_loadChunkResults.enqueue(chunk);
        }
        else
        {
            GetPool().submit([this, chunk] {
                if (chunk->isUnloaded())
                    return;

                m_generator.generateChunkAt(chunk);

                if (chunk->isUnloaded())
                    return;

                m_loadChunkResults.enqueue(chunk);
            });
        }
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

        if (chunk->hasStructure() && chunk->m_touched)
        {
            m_saveChunkQueue.push(chunk);
        }
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

                if (neighbor && neighbor->isReady() && neighbor->areAllNeighborsReady() && !neighbor->hasStructure())
                {
                    m_generator.generateStructure(neighbor);
                    Game::Get().onChunkLoaded(neighbor);
                }
            });

            chunk->setNeighbor({ 0, 0, 0 }, chunk);

            if (chunk->areAllNeighborsReady())
            {
                if (!chunk->m_hasStructure)
                    m_generator.generateStructure(chunk);
                Game::Get().onChunkLoaded(chunk);
            }

            //std::cout << chunk->toJBT() << '\n';
        }

        for (auto& entity : m_entities)
            entity->setOldPosAndRot();

        for (auto& entity : m_entities)
            entity->tick();
        
        updateEntityTransforms();

        #ifdef CYBRION_DEBUG
        const i32 d = 4;
        #else
        const i32 d = 8;
        #endif
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

        #ifdef CYBRION_DEBUG
        const i32 SAVED_CHUNK_PER_TICK = 2;
        #else
        const i32 SAVED_CHUNK_PER_TICK = 8;
        #endif

        i32 cnt = 0;
        std::cout << m_saveChunkQueue.size() << '\n';
        while (cnt < SAVED_CHUNK_PER_TICK && !m_saveChunkQueue.empty())
        {
            auto chunk = m_saveChunkQueue.front();
            m_saveChunkQueue.pop();

            saveChunk(chunk->getChunkPos(), chunk);
            cnt += 1;
        }

        syncRegionFiles();

        for (auto& [pos, chunk] : m_chunkMap)
        {
            if (chunk->m_dirty)
            {
                Game::Get().onChunkChanged(chunk);
                chunk->m_dirty = false;
            }
        }
    }

    void World::playSound(const string& name)
    {
        Game::Get().onPlaySound(name);
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

        auto chunk = it->second;

        Block& oldBlock = chunk->getBlock(localPos);

        if (oldBlock == block)
            return { chunk, pos, block, block };

        chunk->setBlock(localPos, block);

        BlockModifyResult result{ chunk, pos, oldBlock, block };
        return result;
    }

    BlockModifyResult World::updateBlock(const ivec3& pos, Block& block)
    {
        if (!block.beforePlace(pos))
            return { nullptr, {0, 0, 0}, Blocks::AIR, Blocks::AIR };

        auto result = setBlock(pos, block);

        result.chunk->eachBlockAndNeighbors(Chunk::posToLocalPos(result.pos), [&](Block*& neighbor, ref<Chunk>& chunk, const ivec3& dir) {
            if (neighbor)
            {
                neighbor->onTick(result.pos + dir);
                chunk->setDirty(true);
            }
        });

        return result;
    }

    //void World::setBlockOnly(const ivec3& pos, Block& block)
    //{
    //    ivec3 chunkPos = Chunk::posToChunkPos(pos);
    //    uvec3 localPos = Chunk::posToLocalPos(pos);

    //    auto it = m_chunkMap.find(chunkPos);

    //    if (it == m_chunkMap.end()) return;

    //    it->second->setBlock(localPos, block);
    //}

    BlockModifyResult World::placeBlock(const ivec3& pos, Block& block)
    {
        auto result = updateBlock(pos, block.getPlacedBlock(pos));

        if (result.chunk->m_hasStructure)
        {
           if (result.block == Blocks::AIR)
                Game::Get().onBreakBlock(result);
            else
                Game::Get().onPlaceBlock(result);
        }

        //result.block.onPlaced(result.pos);

        return result;
    }

    string World::getName() const
    {
        return m_name;
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

                            if (!block) continue;

                            vec3 offset = { x + 0.5f, y + 0.5f, z + 0.5f };

                            for (AABB& cbound : block->getCollisionBounds())
                            {
                                auto current = AABB::sweptAABB({ ppos, size },
                                    { cbound.getPos() + offset, cbound.getSize()}, v);

                                if (current.delta < result.delta)
                                    result = current;
                            }
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
            entity->setPos(pos - entity->getLocalBB().getPos());
        }
    }

    void World::loadRegion(const ivec3& pos)
    {
        if (m_regionMap.find(pos) == m_regionMap.end())
        {
            string regionPath = m_savePath + "/region/" + GetRegionFilename(pos);

            if (!std::filesystem::exists(regionPath))
                jbt::hjbt_util::create_empty_file(regionPath, 32 * 32 * 32, 1024);

            m_regionMap[pos] = std::make_shared<jbt::hjbt_file>(regionPath);
        }
    }

    void World::save(const string& path)
    {
        auto& player = Game::Get().getPlayer();
        auto playerEntity = player.getEntity();

        jbt::tag config;
        jbt::open_tag(config, path + "/world.jbt");

        config.set_float("player_x", playerEntity->getPos().x);
        config.set_float("player_y", playerEntity->getPos().y);
        config.set_float("player_z", playerEntity->getPos().z);

        jbt::save_tag(config, path + "/world.jbt");
        
        for (auto& [pos, chunk] : m_chunkMap)
            if (chunk->m_touched)
                saveChunk(pos, chunk);

        while (!m_saveChunkQueue.empty())
        {
            auto chunk = m_saveChunkQueue.front();
            m_saveChunkQueue.pop();

            saveChunk(chunk->getChunkPos(), chunk);
        }

        syncRegionFiles();
    }

    void World::saveChunk(const ivec3& pos, const ref<Chunk>& chunk)
    {
        ivec3 regionPos = ToRegionPos(pos);
        ivec3 localPos = ToLocalRegionPos(pos);

        loadRegion(regionPos);

        auto region = m_regionMap[regionPos];

        if (!region->is_writing)
            region->begin_write();

        auto tag = chunk->toJBT();
        region->write(localPos.x * 32 * 32 + localPos.y * 32 + localPos.z, tag);

        /// BUG:FFSA
        //delete[] tag.get_byte_array("blocks").data;
    }

    void World::syncRegionFiles()
    {
        for (auto& [pos, region] : m_regionMap)
            if (region->is_writing)
                region->end_write();
    }
    
    void World::createNewWorld(const string& name)
    {
        jbt::tag config(jbt::tag_type::OBJECT);

        config.set_string("name", name);

        config.set_float("player_x", 0);
        config.set_float("player_y", 80);
        config.set_float("player_z", 0);

        string worldPath = Application::Get().getSavePath(name);

        // create world folder
        std::filesystem::create_directory(worldPath);
        jbt::save_tag(config, worldPath + "/world.jbt");

        std::filesystem::create_directory(worldPath + "/region");
    }

    ref<World> World::loadWorld(const string& path)
    {
        jbt::tag config;
        jbt::open_tag(config, path + "/world.jbt");

        auto world = std::make_shared<World>(config.get_string("name"));
        world->m_savePath = path;

        // spawn player
        Game::Get().getPlayer().setEntity(world->spawnEntity({
            config.get_float("player_x"),
            config.get_float("player_y"),
            config.get_float("player_z")
        }));

        return world;
    }

    ivec3 World::ToRegionPos(const ivec3& pos)
    {
        return { pos.x >> 5, pos.y >> 5, pos.z >> 5 };
    }
    ivec3 World::ToLocalRegionPos(const ivec3& pos)
    {
        return { pos.x & 31, pos.y & 31, pos.z & 31 };
    }
    string World::GetRegionFilename(const ivec3& pos)
    {
        return "r." + std::to_string(pos.x) + "." + std::to_string(pos.y) + "." + std::to_string(pos.z) + ".hjbt";
    }
}
