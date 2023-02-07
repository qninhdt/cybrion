#include "client/camera.hpp"

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
        updateViewMatrix();
        updateProjectionMatrix();
    }

    const mat4& Camera::getViewMatrix() const
    {
        return m_viewMatrix;
    }

    const mat4& Camera::getProjectionViewMatrix() const
    {
        return m_projectionViewMatrix;
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

    void Camera::updateViewMatrix()
    {
        vec3 direction = getDirection();
        m_viewMatrix = glm::lookAt(getPosition(), getPosition() + direction, m_up);
        m_right = glm::normalize(glm::cross(direction, m_up));
        m_forward = glm::normalize(glm::cross(m_up, m_right));
        updateProjectionViewMatrix();
    }

    void Camera::updateProjectionMatrix()
    {
        m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_near, m_far);
        updateProjectionViewMatrix();
    }

    void Camera::updateProjectionViewMatrix()
    {
        m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
    }
}