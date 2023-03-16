#include "world/block/building/fence_block.hpp"
#include "game.hpp"

namespace cybrion
{
    /*Block& FenceBlock::getPlacedBlock(const ivec3& pos, BlockFace face)
    {
        return *this;
    }

    void FenceBlock::onPlaced(const ivec3& pos)
    {
        updateNeighborFences(pos);
    }

    void FenceBlock::onBroken(const ivec3& pos)
    {
        updateNeighborFences(pos);
    }

    void FenceBlock::updateNeighborFences(const ivec3& pos)
    {
        array<vec2, 5> dirs = { {
            { 0, +1 },
            { 0, -1 },
            { +1, 0 },
            { -1, 0 },
            { 0, 0 }
        } };

        auto& world = Game::Get().getWorld();

        for (auto& dir : dirs)
        {
            ivec3 npos = { pos.x + dir.x, pos.y, pos.z + dir.y };
            Block& neighbor = world.getBlock(npos);

            if (neighbor.getType() == BlockType::FENCE)
                world.setBlockOnly(npos, getFenceAt(world, npos));
        }
    }*/

    void FenceBlock::onTick(const ivec3& pos)
    {
        auto& world = Game::Get().getWorld();
        world.setBlock(pos, getFenceAt(world, pos));
    }

    FenceBlock& FenceBlock::getFenceAt(World& world, const ivec3& pos)
    {
        FenceBlock* block = this;

        Block* eastBlock = world.tryGetBlock({ pos.x + 1, pos.y, pos.z });
        if (eastBlock && eastBlock->getType() == BlockType::FENCE)
            block = &block->set<"east">(1);
        else
            block = &block->set<"east">(0);

        Block* westBlock = world.tryGetBlock({ pos.x - 1, pos.y, pos.z });
        if (westBlock && westBlock->getType() == BlockType::FENCE)
            block = &block->set<"west">(1);
        else
            block = &block->set<"west">(0);

        Block* northBlock = world.tryGetBlock({ pos.x, pos.y, pos.z + 1 });
        if (northBlock && northBlock->getType() == BlockType::FENCE)
            block = &block->set<"north">(1);
        else
            block = &block->set<"north">(0);

        Block* southBlock = world.tryGetBlock({ pos.x, pos.y, pos.z - 1 });
        if (southBlock && southBlock->getType() == BlockType::FENCE)
            block = &block->set<"south">(1);
        else
            block = &block->set<"south">(0);
        
        return *block;
    }

}