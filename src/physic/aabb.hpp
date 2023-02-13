#pragma once

#include "util/math.hpp"

namespace cybrion
{
    struct SweptAABBResult
    {
        f32 delta;
        vec3 normal;
    };

    class AABB
    {
    public:

        AABB(const vec3& position, const vec3& size);

        vec3 getMin() const;
        vec3 getMax() const;
        vec3 getPosition() const;
        vec3 getSize() const;

        static SweptAABBResult SweptAABB(const AABB& a, const AABB& b, const ivec3& v);

    private:
        vec3 m_position;
        vec3 m_size;
    };
}