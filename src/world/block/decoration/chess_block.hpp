#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    enum class ChessType
    {
        KING,
        QUEEN,
        ROOK,
        BISHOP,
        KNIGHT,
        PAWN
    };

    enum class ChessColor
    {
        WHITE,
        BLACK
    };

    class ChessBlock : public TBlock<
        ChessBlock,
        BlockType::CHESS,
        block::state<ChessType, "type", 6>,
        block::state<ChessColor, "color", 2>,
        block::state<BlockHorizontalFace, "face", 4>
    >
    {
    public:
        Block& getPlacedBlock(const ivec3& pos);
    };
}