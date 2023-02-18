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

        AABB();
        AABB(const vec3& pos, const vec3& size);

        vec3 getMin() const;
        vec3 getMax() const;
        vec3 getPos() const;
        vec3 getSize() const;

        static SweptAABBResult sweptAABB(const AABB& a, const AABB& b, const vec3& v);
        static AABB mergeAABB(const AABB& a, const AABB& b);

    private:
        vec3 m_pos;
        vec3 m_size;
    };
}