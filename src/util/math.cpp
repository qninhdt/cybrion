#include "util/math.hpp"

namespace cybrion::util
{
    f32 lerpAngle(f32 a, f32 b, f32 t)
    {
        if (std::abs(a - b) < pi)
            return lerp(a, b, t);

        if (b > a)
        {
            f32 r = lerp(a, b - two_pi, t);
            if (r < 0) r += two_pi;
            return r;
        }
        else
        {
            f32 r = lerp(a - two_pi, b, t);
            if (r < 0) r += two_pi;
            return r;
        }
    }

    vec3 lerpRot(vec3 a, vec3 b, f32 t)
    {
        return {
            lerpAngle(a.x, b.x, t),
            lerpAngle(a.y, b.y, t),
            lerpAngle(a.z, b.z, t)
        };
    }

    int32_t signNum(float num) {
        return num > 0 ? 1 : num < 0 ? -1 : 0;
    }

    float intBound(float s, float ds) {
        return (ds > 0 ? ceil(s) - s : s - std::floor(s)) / std::abs(ds);
    }
}