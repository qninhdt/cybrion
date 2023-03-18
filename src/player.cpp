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
                ivec3 placedPos = m_targetPos + Block::GetDirectionFromFace(m_targetFace);
                auto& world = Game::Get().getWorld();

                static int i = 0;
                if (world.getBlock(placedPos) == Blocks::AIR)
                {
                    //world.placeBlock(placedPos, Blocks::DANDELION.set<"type">((PlantType)(i++ % 13)));
                    world.placeBlock(placedPos, Blocks::TABLECLOTH);
                }

                m_blockInteractStopwatch.reset();
            }
            m_input.rightClick = false;
        }

        if (m_input.leftClick)
        {
            if (m_targetBlock && m_blockInteractStopwatch.getDeltaTime() > PLAYER_BLOCK_INTERACT_DELAY)
            {
                Game::Get().getWorld().placeBlock(m_targetPos, Blocks::AIR);
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

                AABB bound = block->getBound();
                bound = { bound.getPos() + vec3(blockPos) + vec3(0.5f, 0.5f, 0.5f), bound.getSize()};
                
                ivec3 bnormal{ 0,0,0 };
                if (!VoxelRay::Intersection(m_entity->getPos(), m_entity->getDir(), bound, bnormal))
                    return false;

                //std::cout << bnormal.x << ' ' << bnormal.y << ' ' << bnormal.z << '\n';

                m_targetFace = Block::GetFaceFromDirection(bnormal);
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

    BlockFace Player::getBlockDirection(bool horizontal) const
    {
        array<tuple<vec3, BlockFace>, 6> dirs = { {
            { { +1, 0, 0 }, BlockFace::EAST   },
            { { 0, 0, +1 }, BlockFace::SOUTH  },
            { { -1, 0, 0 }, BlockFace::WEST   },
            { { 0, 0, -1 }, BlockFace::NORTH  },
            { { 0, +1, 0 }, BlockFace::TOP    },
            { { 0, -1, 0 }, BlockFace::BOTTOM },
        } };

        BlockFace result = BlockFace::EAST;
        f32 max = 0;
        vec3 dir = getEntity()->getDir();

        for (auto& [axis, face] : dirs)
        {
            if (horizontal && (face == BlockFace::TOP || face == BlockFace::BOTTOM))
                continue;

            f32 cos = glm::dot(axis, dir) / glm::length(dir);
            if (cos > max)
            {
                result = face;
                max = cos;
            }
        }

        return result;
    }
}