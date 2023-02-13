#pragma once

namespace cybrion
{
    class VoxelRay
    {
    public:

        using CastCallback = std::function<bool(const ivec3&, const ivec3& normal)>;

        static void Cast(const vec3& position, const vec3& direction, u32 maxDistance, CastCallback callback);
    private:
    };
}