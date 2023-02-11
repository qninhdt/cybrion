#include "world/chunk/chunk.hpp"
#include "game.hpp"

namespace cybrion
{
    array<tuple<ivec3, BlockFace>, 6> ChunkData::Directions = { {
        { { +1, 0, 0 }, BlockFace::EAST   },
        { { 0, +1, 0 }, BlockFace::TOP    },
        { { 0, 0, +1 }, BlockFace::SOUTH  },
        { { -1, 0, 0 }, BlockFace::WEST   },
        { { 0, -1, 0 }, BlockFace::BOTTOM },
        { { 0, 0, -1 }, BlockFace::NORTH  },
    } };

    Block& ChunkData::getBlock(const uvec3& pos) const
    {
        assert(pos.x < CHUNK_SIZE);
        assert(pos.y < CHUNK_SIZE);
        assert(pos.z < CHUNK_SIZE);
        u32 blockId = blocks.get(PosToIndex(pos));
        return Game::Get().getBlockRegistry().getBlock(blockId);
    }

    void ChunkData::setBlock(const uvec3& pos, const Block& block)
    {
        u32 index = PosToIndex(pos);
        blocks.set(index, block.getId());
    }

    vec3 ChunkData::getWorldPosition() const
    {
        return { position.x * CHUNK_SIZE, position.y * CHUNK_SIZE, position.z * CHUNK_SIZE };
    }
    
    u32 ChunkData::PosToIndex(const uvec3& pos)
    {
        return pos.x * CHUNK_SIZE * CHUNK_SIZE + pos.y * CHUNK_SIZE + pos.z;
    }
}