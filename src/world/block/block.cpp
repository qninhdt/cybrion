#include "world/block/block.hpp"
#include "game.hpp"

namespace cybrion
{
    BlockFace Block::GetFaceFromDirection(const ivec3& dir)
    {
        if (dir.z == 1)
            return BlockFace::NORTH;
        if (dir.z == -1)
            return BlockFace::SOUTH;
        if (dir.y == 1)
            return BlockFace::TOP;
        if (dir.y == -1)
            return BlockFace::BOTTOM;
        if (dir.x == 1)
            return BlockFace::EAST;
        return BlockFace::WEST;
    }

    BlockFace Block::GetOppositeFace(const BlockFace& face)
    {
        return (BlockFace)(((i32)face + 3) % 6);
    }
}