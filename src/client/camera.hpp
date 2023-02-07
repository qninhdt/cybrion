#pragma once

#include "transform.hpp"

namespace cybrion
{
    class Camera : public Transform
    {
    public:
        static constexpr vec3 XAxis = { 1, 0, 0 };
        static constexpr vec3 YAxis = { 0, 1, 0 };
        static constexpr vec3 ZAxis = { 0, 0, 1 };

        Camera(f32 aspect, f32 fov, f32 near, f32 far);

        const mat4& getViewMatrix() const;
        const mat4& getProjectionViewMatrix() const;

        vec3 getUp() const;
        vec3 getRight() const;
        vec3 getForward() const;

        void updateViewMatrix();
        void updateProjectionMatrix();

    private:
        void updateProjectionViewMatrix();

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