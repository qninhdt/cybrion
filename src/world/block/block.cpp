#include "world/block/block.hpp"
#include "game.hpp"

namespace cybrion
{
    BlockFace Block::GetFaceFromDirection(const ivec3& dir)
    {
        if (dir.z == 1)
            return BlockFace::SOUTH;
        if (dir.z == -1)
            return BlockFace::NORTH;
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

    ivec3 Block::GetDirectionFromFace(const BlockFace& face)
    {
        switch (face)
        {
        case BlockFace::EAST:
            return { 1, 0, 0 };
        case BlockFace::TOP:
            return { 0, 1, 0 };
        case BlockFace::SOUTH:
            return { 0, 0, 1 };
        case BlockFace::WEST:
            return { -1, 0, 0 };
        case BlockFace::BOTTOM:
            return { 0, -1, 0 };
        default:
            return { 0, 0, -1 };
        }
    }
}