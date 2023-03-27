#pragma once

#include "world/block/block.hpp"
#include "world/entity/entity.hpp"
#include "core/stopwatch.hpp"

namespace cybrion
{
    static constexpr i32 PLAYER_BLOCK_INTERACT_DELAY = 200000;

    struct PlayerInput
    {
        bool isMoving = false;
        bool rightClick = false;
        bool leftClick = false;
        vec3 moveDir = { 0, 0, 0 };
        vec3 rot = { 0, 0, 0 };
        bool ctrl = false;
        bool shift = false;
        f32 scroll = 0;
    };

    class Player
    {
    public:
        static constexpr i32 INVENTORY_SIZE = 16;
        static constexpr i32 DISPLAYED_INVENTORY_SIZE = 9;

        Player();
        ref<Entity> getEntity() const;
        void setEntity(const ref<Entity>& entity);

        void tick();

        Block* getTargetBlock() const;
        vector<Block*>& getInventory();
        Block* getHeldBlock() const;
        i32 getHeldSlot() const;
        BlockFace getTargetFace() const;
        ivec3 getTargetPos() const;

        PlayerInput& getInput();

        BlockFace getBlockFace() const;
        BlockHorizontalFace getBlockHorizontalFace() const;

    private:
        ref<Entity> m_entity;

        ivec3 m_targetPos;
        Block* m_targetBlock;
        BlockFace m_targetFace;

        PlayerInput m_input;
        Stopwatch m_blockInteractStopwatch;
        vector<Block*> m_inventory;
        i32 m_heldSlot;
    };
}