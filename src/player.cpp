#include "player.hpp"
#include "world/entity/entity.hpp"
#include "voxel_ray.hpp"
#include "game.hpp"

namespace cybrion
{
    Player::Player():
        m_isSelecting(false)
    {
    }

    Object Player::getEntity() const
    {
        return m_entity;
    }

    void Player::setEntity(Object entity)
    {
        m_entity = entity;
    }

    void Player::tick()
    {
        auto& playerData = m_entity.get<EntityData>();

        // controller
        if (m_input.isMoving || glm::length(m_input.deltaRotation) > 0.001f)
        {
            auto& data = m_entity.get<EntityData>();

            if (m_input.isMoving)
            {
                data.velocity = m_input.moveDirection;
            }
            
            vec3 rotation = data.transform.getRotation() + m_input.deltaRotation * 0.003f;

            if (rotation.x > pi / 2 - 0.001f && rotation.x < pi * 3 / 2 + 0.001f)
            {
                if (rotation.x - pi / 2 - 0.001f < pi * 3 / 2 + 0.001f - rotation.x)
                    rotation.x = pi / 2 - 0.001f;
                else
                    rotation.x = pi * 3 / 2 + 0.001f;
            }

            data.transform.setRotation(rotation);

            m_input.deltaRotation = { 0, 0, 0 }; // reset
        }

        if (!m_input.isMoving)
            playerData.velocity = { 0, 0, 0 };

        if (m_input.rightClick)
        {
            if (m_isSelecting && m_blockInteractStopwatch.getDeltaTime() > PLAYER_BLOCK_INTERACT_DELAY)
            {
                Game::Get().getWorld().placeBlock(m_selectingPos, m_selectingFace, BlockRegistry::Get().getBlock(BlockType::LOG));
                m_blockInteractStopwatch.reset();
            }
            m_input.rightClick = false;
        }

        if (m_input.leftClick)
        {
            if (m_isSelecting && m_blockInteractStopwatch.getDeltaTime() > PLAYER_BLOCK_INTERACT_DELAY)
            {
                Game::Get().getWorld().breakBlock(m_selectingPos);
                m_blockInteractStopwatch.reset();
            }
            m_input.leftClick = false;
        }

        // selecting block
        VoxelRay::Cast(
            playerData.transform.getPosition(),
            playerData.transform.getDirection(),
            0,
            [&](const ivec3& blockPos, const ivec3& normal)
            {
                Block* block = Game::Get().getWorld().tryGetBlock(blockPos);

                // stop when block is not loaded
                if (!block) {
                    m_isSelecting = false;
                    return true;
                }
                
                if (block->getDisplay() == BlockDisplay::TRANSPARENT)
                    return false;

                m_selectingFace = Block::GetFaceFromDirection(normal);
                m_selectingPos = blockPos;
                m_isSelecting = true;

                return true;
            }
        );
    }

    Block* Player::getSelectingBlock() const
    {
        if (!m_isSelecting)
            return nullptr;

        return Game::Get().getWorld().tryGetBlock(m_selectingPos);
    }

    BlockFace Player::getSelectingFace() const
    {
        return m_selectingFace;
    }

    ivec3 Player::getSelectingPosition() const
    {
        return m_selectingPos;
    }

    bool Player::isSelectingBlock() const
    {
        return m_isSelecting;
    }

    PlayerInput& Player::getInput()
    {
        return m_input;
    }
}