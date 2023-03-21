#pragma once

namespace cybrion
{
    enum class BlockType
    {
        AIR,
        SOIL,
        LOG,
        SAND,
        ROCK,
        BRICK,
        LEAF,
        GLASS,
        WATER,
        FENCE,
        PLANT,
        TABLECLOTH,
        CHESS,
        ENUM_SIZE
    };

    enum class BlockShape
    {
        CUBE,
        CUSTOM
    };

    enum class BlockDisplay
    {
        OPAQUE,
        LIQUID,
        SEMI_OPAQUE,
        TRANSPARENT,
        SEMI_TRANSPARENT
    };

    enum class BlockFace
    {
        EAST   = 0,
        TOP    = 1,
        SOUTH  = 2,
        WEST   = 3,
        BOTTOM = 4,
        NORTH  = 5
    };

    enum class BlockRotation
    {
        R0,
        R90,
        R180,
        R270
    };

    enum class BlockHorizontalFace
    {
        EAST,
        SOUTH,
        WEST,
        NORTH
    };
}