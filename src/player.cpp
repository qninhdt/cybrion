#include "player.hpp"
#include "world/entity/entity.hpp"
#include "voxel_ray.hpp"
#include "game.hpp"

namespace cybrion
{
    Player::Player() :
        m_targetBlock(nullptr)
    {
    }

    ref<Entity> Player::getEntity() const
    {
        return m_entity;
    }

    void Player::setEntity(const ref<Entity>& entity)
    {
        m_entity = entity;
    }

    void Player::tick()
    {
        static int x = 0;
        // controller
        if (m_input.isMoving)
            m_entity->setVelocity(m_input.moveDir * (m_input.run ? 20.0f : 2.0f));
        else
            m_entity->setVelocity({ 0, 0, 0 });
        
        m_entity->setRot(m_input.rot);

        if (m_input.rightClick)
        {
            if (m_targetBlock && m_blockInteractStopwatch.getDeltaTime() > PLAYER_BLOCK_INTERACT_DELAY)
            {
                Game::Get().getWorld().placeBlock(m_targetPos, m_targetFace, Blocks::WATER);
                m_blockInteractStopwatch.reset();
            }
            m_input.rightClick = false;
        }

        if (m_input.leftClick)
        {
            if (m_targetBlock && m_blockInteractStopwatch.getDeltaTime() > PLAYER_BLOCK_INTERACT_DELAY)
            {
                Game::Get().getWorld().breakBlock(m_targetPos);
                m_blockInteractStopwatch.reset();
            }
            m_input.leftClick = false;
        }

        // selecting block
        VoxelRay::Cast(
            m_entity->getPos(),
            m_entity->getDir(),
            0,
            [&](const ivec3& blockPos, const ivec3& normal)
            {
                Block* block = Game::Get().getWorld().tryGetBlock(blockPos);

                // stop when block is not loaded
                if (!block) {
                    m_targetBlock = nullptr;
                    return true;
                }
                
                if (block->getDisplay() == BlockDisplay::TRANSPARENT || block->getDisplay() == BlockDisplay::LIQUID)
                    return false;

                m_targetFace = Block::GetFaceFromDirection(normal);
                m_targetPos = blockPos;
                m_targetBlock = block;

                return true;
            }
        );
    }

    Block* Player::getTargetBlock() const
    {
        return m_targetBlock;
    }

    BlockFace Player::getTargetFace() const
    {
        return m_targetFace;
    }

    ivec3 Player::getTargetPos() const
    {
        return m_targetPos;
    }

    PlayerInput& Player::getInput()
    {
        return m_input;
    }
}