#pragma once

namespace cybrion
{
    class BasicTransform
    {
    public:
        BasicTransform();
        BasicTransform(const vec3& pos, const vec3& rot);

        const vec3& getPos() const;
        void setPos(const vec3& pos);
        void move(const vec3& delta);

        const vec3& getRot() const;
        void setRot(const vec3& rot);
        void rotate(const vec3& delta);

        vec3 getDir() const;
        void setDir(const vec3& dir);

    protected:
        vec3 m_pos;
        vec3 m_rot;
    };

    class Transform : public BasicTransform
    {
    public:
        Transform();
        Transform(const vec3& position, const vec3& rotation, const vec3& scale);

        const vec3& getScale() const;
        void setScale(const vec3& scale);

        void updateModelMat();
        const mat4& getModelMat() const;

    private:
        vec3 m_scale;
        mat4 m_modelMat;
    };
}