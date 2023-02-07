#pragma once

namespace cybrion
{
    class Transform
    {
    public:

        Transform();
        Transform(const vec3& position, const vec3& rotation, const vec3& scale);

        vec3 getPosition() const;
        void setPosition(const vec3& position);
        vec3 move(const vec3& delta);

        vec3 getRotation() const;
        void setRotation(const vec3& rotation);
        vec3 rotate(const vec3& delta);

        vec3 getDirection() const;
        void setDirection(const vec3& direction);

        void updateModelMatrix();
        const mat4& getModelMatrix() const;

    private:
        vec3 m_position;
        vec3 m_rotation;
        vec3 m_scale;
        mat4 m_modelMatrix;
    };
}