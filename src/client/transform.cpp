#include "client/transform.hpp"

namespace cybrion
{   
    namespace detail
    {
        Transform0::Transform0(const vec3& position, const vec3& rotation):
            m_position(position),
            m_rotation(rotation)
        {
        }

        const vec3& Transform0::getPosition() const
        {
            return m_position;
        }

        void Transform0::setPosition(const vec3& position)
        {
            m_position = position;
        }

        void Transform0::move(const vec3& delta)
        {
            m_position += delta;
        }

        const vec3& Transform0::getRotation() const
        {
            return m_rotation;
        }

        void Transform0::setRotation(const vec3& rotation)
        {
            m_rotation = rotation;

            // rotation in range [0, 2pi)

            if (rotation.x < 0) m_rotation.x += two_pi;
            if (rotation.y < 0) m_rotation.y += two_pi;
            if (rotation.z < 0) m_rotation.z += two_pi;

            if (rotation.x >= two_pi) m_rotation.x -= two_pi;
            if (rotation.y >= two_pi) m_rotation.y -= two_pi;
            if (rotation.z >= two_pi) m_rotation.z -= two_pi;
        }

        void Transform0::rotate(const vec3& delta)
        {
            setRotation(m_rotation + delta);
        }

        vec3 Transform0::getDirection() const
        {
            return {
                sin(m_rotation.y) * cos(m_rotation.x),
                sin(m_rotation.x),
                cos(m_rotation.y) * cos(m_rotation.x),
            };
        }

        void Transform0::setDirection(const vec3& direction)
        {
            m_rotation.x = asin(direction.y);
            m_rotation.y = atan2(direction.x, direction.z);
        }

    }

    Transform::Transform(): Transform({ 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 })
    {
    }

    Transform::Transform(const vec3& position, const vec3& rotation, const vec3& scale) :
        Transform0(position, rotation),
        m_scale(scale),
        m_modelMatrix(1.0f)
    {
    }

    const vec3& Transform::getScale() const
    {
        return m_scale;
    }

    void Transform::setScale(const vec3& scale)
    {
        m_scale = scale;
    }

    void Transform::updateModelMatrix()
    {
        m_modelMatrix = mat4(1.0f);
        m_modelMatrix = glm::translate(m_modelMatrix, m_position);
        m_modelMatrix = m_modelMatrix * glm::eulerAngleXYZ(m_rotation.x, m_rotation.y, m_rotation.z);
        m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
    }

    const mat4& Transform::getModelMatrix() const
    {
        return m_modelMatrix;
    }
}