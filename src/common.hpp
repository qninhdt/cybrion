#pragma once

#include "core/log.hpp"

#ifdef _MSC_VER
#ifdef _DEBUG
#define CYBRION_DEBUG
#endif
#endif

#ifdef _WIN32
#ifdef _WIN64
#define CYBRION_PLATFORM_WINDOWS
#else
#error "x86 Builds are not supported!"
#endif
#endif

#ifdef CYBRION_DEBUG
#ifdef CYBRION_PLATFORM_WINDOWS
#define CYBRION_DEBUGBREAK() __debugbreak()
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#endif

#define CYBRION_STATIC_ERROR(message) static_assert(cybrion::detail::always_false<T> && message);

#ifdef CYBRION_DEBUG
#define CYBRION_ROOT_PATH "D:/github/cybrion/"
#else
#define CYBRION_ROOT_PATH "D:/github/cybrion/"
#endif

namespace cybrion
{
    namespace detail
    {
        template <class...>
        constexpr std::false_type always_false{};
    };

    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;

    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;

    using f32 = float;
    using f64 = double;

    using vec2 = glm::vec2;
    using vec3 = glm::vec3;
    using vec4 = glm::vec4;

    using ivec2 = glm::ivec2;
    using ivec3 = glm::ivec3;
    using ivec4 = glm::ivec4;

    using uvec2 = glm::uvec2;
    using uvec3 = glm::uvec3;
    using uvec4 = glm::uvec4;

    using mat2 = glm::mat2;
    using mat3 = glm::mat3;
    using mat4 = glm::mat4;

    using string = std::string;

    template <typename K, typename V>
    using umap = robin_hood::unordered_map<K, V>;

    template <typename V>
    using uset = robin_hood::unordered_set<V>;

    template <typename V, u32 size>
    using array = std::array<V, size>;

    template <typename... V>
    using tuple = std::tuple<V...>;

    template <typename V>
    using vector = std::vector<V>;

    constexpr f32 pi = 3.14159265358979323846;
    constexpr f32 two_pi = pi * 2;
} // namespace nith