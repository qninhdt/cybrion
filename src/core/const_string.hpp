#pragma once

namespace cybrion
{
    template <u32 N>
    struct const_string
    {
        char value[N + 1]{};
        constexpr const_string(char const *str)
        {
            for (unsigned i = 0; i != N; ++i)
                value[i] = str[i];
        }

        constexpr u32 length() const { return N; }

        constexpr operator char const *() const { return value; }

        constexpr string to_string() const { return string((const char *)value); }
    };

    template <u32 N>
    const_string(char const (&)[N]) -> const_string<N - 1>;
}