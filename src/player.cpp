#include "player.hpp"
#include "world/entity/entity.hpp"
#include "voxel_ray.hpp"
#include "game.hpp"

namespace cybrion
{
    Player::Player() :
        m_targetBlock(nullptr),
        m_heldSlot(0),
        m_inventory(INVENTORY_SIZE, nullptr)
    {
        m_inventory[0] = &Blocks::GRASS_BLOCK;
        m_inventory[1] = &Blocks::DIRT;
        m_inventory[2] = &Blocks::STONE;
        m_inventory[3] = &Blocks::COBBLESTONE;
        m_inventory[4] = &Blocks::OAK_FENCE.set<"east">(1).set<"west">(1);
        m_inventory[5] = &Blocks::OAK_FENCE_GATE.set<"face">(BlockHorizontalFace::NORTH);
        m_inventory[6] = &Blocks::CORNFLOWER;
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
            m_entity->setVelocity(m_input.moveDir * (m_input.ctrl ? 20.0f : (m_input.shift ? 0.5f : 1)));
        else
            m_entity->setVelocity({ 0, 0, 0 });
        
        m_entity->setRot(m_input.rot);

        if (m_input.scroll)
        {
            i32 delta = m_input.scroll < 0 ? 1 : -1;
            m_heldSlot = (((m_heldSlot + delta) % INVENTORY_SIZE) + INVENTORY_SIZE) % INVENTORY_SIZE;

            m_input.scroll = 0;
        }

        if (m_input.rightClick)
        {
            if (m_targetBlock && m_blockInteractStopwatch.getDeltaTime() > PLAYER_BLOCK_INTERACT_DELAY)
            {
                if (!m_targetBlock->isInteractive() || m_input.shift)
                {
                    ivec3 placedPos = m_targetPos + Block::GetDirectionFromFace(m_targetFace);
                    auto& world = Game::Get().getWorld();

                    static int i = 0;
                    if (world.getBlock(placedPos) == Blocks::AIR)
                    {
                        Block* block = m_inventory[m_heldSlot];

                        if (block)
                            world.placeBlock(placedPos, *block);
                    }
                }
                else
                {
                    m_targetBlock->onInteract(m_targetPos);
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

    vector<Block*>& Player::getInventory()
    {
        return m_inventory;
    }

    Block* Player::getHeldBlock() const
    {
        return m_inventory[m_heldSlot];
    }

    i32 Player::getHeldSlot() const
    {
        return m_heldSlot;
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

    BlockFace Player::getBlockFace() const
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
            f32 cos = glm::dot(axis, dir) / glm::length(dir);
            if (cos > max)
            {
                result = face;
                max = cos;
            }
        }

        return result;
    }

    BlockHorizontalFace Player::getBlockHorizontalFace() const
    {
        array<tuple<vec3, BlockHorizontalFace>, 6> dirs = { {
            { { +1, 0, 0 }, BlockHorizontalFace::EAST   },
            { { 0, 0, +1 }, BlockHorizontalFace::SOUTH  },
            { { -1, 0, 0 }, BlockHorizontalFace::WEST   },
            { { 0, 0, -1 }, BlockHorizontalFace::NORTH  }
        } };

        BlockHorizontalFace result = BlockHorizontalFace::EAST;
        f32 max = 0;
        vec3 dir = getEntity()->getDir();

        for (auto& [axis, face] : dirs)
        {
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