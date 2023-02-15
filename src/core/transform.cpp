#include "core/transform.hpp"

namespace cybrion
{   
    BasicTransform::BasicTransform() : BasicTransform({ 0, 0, 0 }, { 0, 0, 0 })
    {
    }

    BasicTransform::BasicTransform(const vec3& pos, const vec3& rot):
        m_pos(pos),
        m_rot(rot)
    {
    }

    const vec3& BasicTransform::getPos() const
    {
        return m_pos;
    }

    void BasicTransform::setPos(const vec3& pos)
    {
        m_pos = pos;
    }

    void BasicTransform::move(const vec3& delta)
    {
        m_pos += delta;
    }

    const vec3& BasicTransform::getRot() const
    {
        return m_rot;
    }

    void BasicTransform::setRot(const vec3& rot)
    {
        m_rot = rot;

        // m_rot in range [0, 2pi)

        if (m_rot.x < 0) m_rot.x += two_pi;
        if (m_rot.y < 0) m_rot.y += two_pi;
        if (m_rot.z < 0) m_rot.z += two_pi;

        if (m_rot.x >= two_pi) m_rot.x -= two_pi;
        if (m_rot.y >= two_pi) m_rot.y -= two_pi;
        if (m_rot.z >= two_pi) m_rot.z -= two_pi;
    }

    void BasicTransform::rotate(const vec3& delta)
    {
        setRot(m_rot + delta);
    }

    vec3 BasicTransform::getDir() const
    {
        return {
            sin(m_rot.y) * cos(m_rot.x),
            sin(m_rot.x),
            cos(m_rot.y) * cos(m_rot.x),
        };
    }

    void BasicTransform::setDir(const vec3& dir)
    {
        m_rot.x = asin(dir.y);
        m_rot.y = atan2(dir.x, dir.z);
    }

    Transform::Transform(): Transform({ 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 })
    {
    }

    Transform::Transform(const vec3& pos, const vec3& m_rot, const vec3& scale) :
        BasicTransform(pos, m_rot),
        m_scale(scale),
        m_modelMat(1.0f)
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

    void Transform::updateModelMat()
    {
        m_modelMat = mat4(1.0f);
        m_modelMat = glm::translate(m_modelMat, m_pos);
        m_modelMat = m_modelMat * glm::eulerAngleXYZ(m_rot.x, m_rot.y, m_rot.z);
        m_modelMat = glm::scale(m_modelMat, m_scale);
    }

    const mat4& Transform::getModelMat() const
    {
        return m_modelMat;
    }
}