#pragma once

namespace cybrion
{
    template <u32 N>
    struct const_string
    {
        char value[N + 1]{};
        constexpr const_string(char const* s)
        {
            for (unsigned i = 0; i != N; ++i)
                value[i] = s[i];
        }

        constexpr u32 length() const { return N; }

        constexpr operator char const* () const { return value; }

        constexpr bool operator==(const const_string&) const = default;
    };

    template <u32 N>
    const_string(char const (&)[N]) -> const_string<N - 1>;
}