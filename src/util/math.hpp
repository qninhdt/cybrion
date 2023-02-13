#pragma once

namespace cybrion::util
{
    constexpr u32 CeilLog2(const u32 &n)
    {
        u32 result = 0, value = 1;
        while (value < n)
        {
            value <<= 1;
            ++result;
        }
        return result;
    }

    template <typename T>
    T Lerp(T a, T b, f32 t)
    {
        return a * (1 - t) + b * t;
    }

    f32 LerpAngle(f32 a, f32 b, f32 t);
    vec3 LerpRotaion(vec3 a, vec3 b, f32 t);

    int32_t SignNum(float num);

    float IntBound(float s, float ds);
}
