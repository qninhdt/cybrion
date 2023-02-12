#include "player.hpp"
#include "world/entity/entity.hpp"

namespace cybrion
{
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
        if (m_input.isMoving || glm::length(m_input.deltaRotation) > 0.001f)
        {
            auto& data = m_entity.get<EntityData>();

            if (m_input.isMoving)
            {
                data.transform.move(m_input.moveDirection * 5.0f);
            }

            vec3 rotation = data.transform.getRotation() + m_input.deltaRotation * 1.0f;

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
    }

    PlayerInput& Player::getInput()
    {
        return m_input;
    }
}