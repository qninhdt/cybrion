#pragma once

namespace cybrion
{
    namespace detail
    {
        class Transform0
        {
        public:

            Transform0(const vec3& position, const vec3& rotation);

            const vec3& getPosition() const;
            void setPosition(const vec3& position);
            void move(const vec3& delta);

            const vec3& getRotation() const;
            void setRotation(const vec3& rotation);
            void rotate(const vec3& delta);

            vec3 getDirection() const;
            void setDirection(const vec3& direction);

        protected:
            vec3 m_position;
            vec3 m_rotation;
        };
    }

    class Transform : public detail::Transform0
    {
    public:
        Transform();
        Transform(const vec3& position, const vec3& rotation, const vec3& scale);

        const vec3& getScale() const;
        void setScale(const vec3& scale);

        void updateModelMatrix();
        const mat4& getModelMatrix() const;

    private:
        vec3 m_scale;
        mat4 m_modelMatrix;
    };
}