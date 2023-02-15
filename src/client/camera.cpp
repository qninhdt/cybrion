#include "client/camera.hpp"
#include "world/entity/entity.hpp"
#include "client/graphic/entity_renderer.hpp"

namespace cybrion
{
    Camera::Camera(f32 aspect, f32 fov, f32 near, f32 far) :
        m_aspect(aspect),
        m_fov(fov),
        m_near(near),
        m_far(far),
        m_zoom(1),
        m_up(YAxis)
    {
        updateViewMat();
        updateProjMat();
    }

    void Camera::tick(f32 delta)
    {
        if (m_target)
        {
            setPos(m_target->lerpPos(delta));
            setRot(m_target->lerpRot(delta));

            updateViewMat();
        }
    }

    const mat4& Camera::getViewMat() const
    {
        return m_viewMat;
    }

    const mat4& Camera::getProjMat() const
    {
        return m_projMat;
    }

    const mat4& Camera::getProjViewMat() const
    {
        return m_projViewMat;
    }

    vec3 Camera::getUp() const
    {
        return m_up;
    }

    vec3 Camera::getRight() const
    {
        return m_right;
    }

    vec3 Camera::getForward() const
    {
        return m_forward;
    }

    void Camera::setAspect(f32 aspect)
    {
        m_aspect = aspect;
    }

    void Camera::setTarget(const ref<Entity>& target)
    {
        m_target = target;
    }

    void Camera::updateViewMat()
    {
        vec3 dir = getDir();
        m_viewMat = glm::rotate(mat4(1.0f), m_rot.z, dir)
            * glm::lookAt(m_pos, m_pos + dir, m_up);
        m_right = glm::normalize(glm::cross(dir, m_up));
        m_forward = glm::normalize(glm::cross(m_up, m_right));
        updateProjViewMat();
    }

    void Camera::updateProjMat()
    {
        m_projMat = glm::perspective(m_fov, m_aspect, m_near, m_far);
        updateProjViewMat();
    }

    void Camera::updateProjViewMat()
    {
        m_projViewMat = m_projMat * m_viewMat;
    }
}