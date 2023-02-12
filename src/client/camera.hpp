#pragma once

#include "core/transform.hpp"
#include "core/object.hpp"

namespace cybrion
{
    class Camera : public detail::Transform0
    {
    public:
        static constexpr vec3 XAxis = { 1, 0, 0 };
        static constexpr vec3 YAxis = { 0, 1, 0 };
        static constexpr vec3 ZAxis = { 0, 0, 1 };

        Camera(f32 aspect, f32 fov, f32 near, f32 far);

        void tick();

        const mat4& getViewMatrix() const;
        const mat4& getProjectionMatrix() const;
        const mat4& getProjectionViewMatrix() const;

        vec3 getUp() const;
        vec3 getRight() const;
        vec3 getForward() const;

        void setAspect(f32 aspect);

        void setTarget(Object target);

        void updateViewMatrix();
        void updateProjectionMatrix();

    private:
        void updateProjectionViewMatrix();

        Object m_target;

        f32 m_aspect;
        f32 m_fov;
        f32 m_near;
        f32 m_far;
        f32 m_zoom;
        vec3 m_direction;
        vec3 m_forward;
        vec3 m_right;
        vec3 m_up;
        mat4 m_viewMatrix;
        mat4 m_projectionMatrix;
        mat4 m_projectionViewMatrix;
    };
}