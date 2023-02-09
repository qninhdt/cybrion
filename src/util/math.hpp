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
}
