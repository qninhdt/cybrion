#pragma once

#include "core/object.hpp"

namespace cybrion
{
    struct PlayerInput
    {
        bool isMoving = false;
        vec3 moveDirection = { 0, 0, 0 };
        vec3 deltaRotation = { 0, 0, 0 };
    };

    class Player
    {
    public:

        Object getEntity() const;
        void setEntity(Object entity);

        void tick();

        PlayerInput& getInput();

    private:
        Object m_entity;
        bool m_isMoving;
        PlayerInput m_input;
    };
}