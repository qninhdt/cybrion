#pragma once

#include "world/block/block.hpp"
#include "world/entity/entity.hpp"
#include "core/stopwatch.hpp"

namespace cybrion
{
    static constexpr i32 PLAYER_BLOCK_INTERACT_DELAY = 200;

    struct PlayerInput
    {
        bool isMoving = false;
        bool rightClick = false;
        bool leftClick = false;
        vec3 moveDir = { 0, 0, 0 };
        vec3 deltaRot = { 0, 0, 0 };
        bool run = false;
    };

    class Player
    {
    public:

        Player();
        ref<Entity> getEntity() const;
        void setEntity(const ref<Entity>& entity);

        void tick();

        Block* getTargetBlock() const;
        BlockFace getTargetFace() const;
        ivec3 getTargetPos() const;

        PlayerInput& getInput();

    private:
        ref<Entity> m_entity;

        ivec3 m_targetPos;
        Block* m_targetBlock;
        BlockFace m_targetFace;

        PlayerInput m_input;
        Stopwatch m_blockInteractStopwatch;
    };
}