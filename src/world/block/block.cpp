#include "world/block/block.hpp"
#include "game.hpp"

namespace cybrion
{
    //packed_block BlockUtil::Pack(IBlock block)
    //{
    //    return Pack(*block);
    //}

    //packed_block BlockUtil::Pack(const BlockBase& block)
    //{
    //    packed_block packed;
    //    packed.type = (u16)block.getType();
    //    packed.state = block.serialize();
    //    return packed;
    //}

    //IBlock BlockUtil::Unpack(const packed_block& data)
    //{
    //    return s_blocks[data]->copy();
    //}

   /* BlockFace Block::GetFaceFromDirection(const ivec3& dir)
    {
        if (dir.z == 1)
            return BlockFace::NORTH;
        if (dir.z == -1)
            return BlockFace::SOUTH;
        if (dir.y == 1)
            return BlockFace::UP;
        if (dir.y == -1)
            return BlockFace::DOWN;
        if (dir.x == 1)
            return BlockFace::EAST;
        return BlockFace::WEST;
    }

    BlockFace Block::GetOppositeFace(const BlockFace& face)
    {
        return (BlockFace)(((i32)face + 3) % 6);
    }*/

  /*  void BlockUtil::QueryBlockConfigs(BlockType type, umap<string, string>& query, vector<BlockConfig*>& result)
    {
        for (auto& [_, block] : s_blocks)
        {
            if (block->getType() == type && block->checkQuery(query))
            {
                result.push_back(block->m_config);
            }
        }
    }*/
}