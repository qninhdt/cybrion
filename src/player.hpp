#pragma once

#include "core/object.hpp"
#include "world/block/block.hpp"
#include "world/entity/entity.hpp"

namespace cybrion
{
    struct PlayerInput
    {
        bool isMoving = false;
        bool rightClick = false;
        bool leftClick = false;
        vec3 moveDirection = { 0, 0, 0 };
        vec3 deltaRotation = { 0, 0, 0 };
    };

    class Player
    {
    public:

        Player();
        Object getEntity() const;
        void setEntity(Object entity);

        void tick();

        Block* getSelectingBlock() const;
        BlockFace getSelectingFace() const;
        ivec3 getSelectingPosition() const;
        bool isSelectingBlock() const;

        PlayerInput& getInput();

    private:
        Object m_entity;
        bool m_isSelecting;
        ivec3 m_selectingPos;
        BlockFace m_selectingFace;
        PlayerInput m_input;
    };
}