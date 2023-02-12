#include "util/math.hpp"

namespace cybrion::util
{
    f32 LerpAngle(f32 a, f32 b, f32 t)
    {
        if (std::abs(a - b) < pi)
            return Lerp(a, b, t);

        if (b > a)
        {
            f32 r = Lerp(a, b - two_pi, t);
            if (r < 0) r += two_pi;
            return r;
        }
        else
        {
            f32 r = Lerp(a - two_pi, b, t);
            if (r < 0) r += two_pi;
            return r;
        }
    }

    vec3 LerpRotaion(vec3 a, vec3 b, f32 t)
    {
        return {
            LerpAngle(a.x, b.x, t),
            LerpAngle(a.y, b.y, t),
            LerpAngle(a.z, b.z, t)
        };
    }
}