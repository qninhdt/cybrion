#pragma once

#include "core/transform.hpp"

namespace cybrion
{
    struct EntityData
    {
        Transform oldTransform;
        Transform transform;

        EntityData(const vec3& posistion);

        ivec3 getChunkPosition() const;
        vec3 getChunkWorldPosition() const;
    };
}