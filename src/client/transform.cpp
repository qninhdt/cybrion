#include "client/transform.hpp"

namespace cybrion
{   
    Transform::Transform(): Transform({ 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 })
    {
    }

    Transform::Transform(const vec3& position, const vec3& rotation, const vec3& scale) :
        m_position(position),
        m_rotation(rotation),
        m_scale(scale),
        m_modelMatrix(1.0f)
    {
        //updateModelMatrix();
    }

    vec3 Transform::getPosition() const
    {
        return m_position;
    }

    void Transform::setPosition(const vec3& position)
    {
        m_position = position;
    }

    vec3 Transform::move(const vec3& delta)
    {
        return m_position += delta;
    }

    vec3 Transform::getRotation() const
    {
        return m_rotation;
    }

    void Transform::setRotation(const vec3& rotation)
    {
        m_rotation = rotation;
    }

    vec3 Transform::rotate(const vec3& delta)
    {
        setRotation(m_rotation + delta);
        return m_rotation;
    }

    vec3 Transform::getDirection() const
    {
        return {
            sin(m_rotation.y) * cos(m_rotation.x),
            sin(m_rotation.x),
            cos(m_rotation.y) * cos(m_rotation.x),
        };
    }

    void Transform::setDirection(const vec3& direction)
    {
        m_rotation.x = asin(direction.y);
        m_rotation.y = atan2(direction.x, direction.z);
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