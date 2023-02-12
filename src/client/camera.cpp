#include "client/camera.hpp"
#include "world/entity/entity.hpp"
#include "client/graphic/entity_renderer.hpp"

namespace cybrion
{
    Camera::Camera(f32 aspect, f32 fov, f32 near, f32 far) :
        detail::Transform0({ 0, 0, 0 }, { 0, 0, 0 }),
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

    void Camera::tick()
    {
        if (m_target.valid())
        {
            auto& mesh = m_target.get<EntityRenderer>().mesh;

            setPosition(mesh.getPosition());
            setRotation(mesh.getRotation());

            updateViewMatrix();
        }
    }

    const mat4& Camera::getViewMatrix() const
    {
        return m_viewMatrix;
    }

    const mat4& Camera::getProjectionMatrix() const
    {
        return m_projectionMatrix;
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

    void Camera::setAspect(f32 aspect)
    {
        m_aspect = aspect;
    }

    void Camera::setTarget(Object target)
    {
        m_target = target;
    }

    void Camera::updateViewMatrix()
    {
        vec3 direction = getDirection();
        m_viewMatrix = glm::rotate(mat4(1.0f), m_rotation.z, direction)
            * glm::lookAt(m_position, m_position + direction, m_up);
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