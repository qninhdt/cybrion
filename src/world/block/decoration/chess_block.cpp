#include "world/block/decoration/chess_block.hpp"
#include "game.hpp" 

namespace cybrion
{
    Block& ChessBlock::getPlacedBlock(const ivec3& pos)
    {
        BlockHorizontalFace face = Block::GetOppositeHorizontalFace(
            Game::Get().getPlayer().getBlockHorizontalFace()
        );

        return set<"face">(face);
    }
}