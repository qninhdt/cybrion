#include "world/block/block.hpp"

namespace cybrion
{
    array<tuple<ivec3, BlockFace>, 6> Block::Directions = { {
        { { +1, 0, 0 }, BlockFace::EAST   },
        { { 0, +1, 0 }, BlockFace::TOP    },
        { { 0, 0, +1 }, BlockFace::SOUTH  },
        { { -1, 0, 0 }, BlockFace::WEST   },
        { { 0, -1, 0 }, BlockFace::BOTTOM },
        { { 0, 0, -1 }, BlockFace::NORTH  },
    } };


    Block::Block(const BlockType& type) :
        m_shape(BlockShape::CUBE),
        m_type(type),
        m_displayName(""),
        m_display(BlockDisplay::OPAQUE),
        m_rotationX(BlockRotation::R0),
        m_rotationY(BlockRotation::R0),
        m_rotationZ(BlockRotation::R0),
        m_northTexture(0),
        m_southTexture(0),
        m_westTexture(0),
        m_eastTexture(0),
        m_topTexture(0),
        m_bottomTexture(0),
        m_toLocalFace{ 0, 1, 2, 3, 4, 5 },
        m_toWorldFace{ 0 },
        m_id(0)
    {
    }

    u32 Block::getId() const
    {
        return m_id;
    }

    BlockType Block::getType() const
    {
        return m_type;
    }

    string Block::getDisplayName() const
    {
        return m_displayName;
    }

    BlockDisplay Block::getDisplay() const
    {
        return m_display;
    }

    BlockShape Block::getShape() const
    {
        return m_shape;
    }

    u32 Block::getTexture(u32 index)
    {
        return m_textures[index];
    }

    BlockFace Block::getWorldFace(BlockFace face) const
    {
        return (BlockFace)m_toWorldFace[(u32)face];
    }

    BlockRotation Block::getRotationX() const
    {
        return m_rotationX;
    }

    BlockRotation Block::getRotationY() const
    {
        return m_rotationY;
    }

    BlockRotation Block::getRotationZ() const
    {
        return m_rotationZ;
    }

    bool Block::operator == (const Block& other) const
    {
        return m_id == other.m_id;
    }

    Block& Block::getPlacedBlock(const ivec3& pos, BlockFace face)
    {
        return *this;
    }

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