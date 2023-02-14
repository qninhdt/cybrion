#pragma once

#include "core/transform.hpp"
#include "physic/aabb.hpp"

namespace cybrion
{
    struct EntityData
    {
        Transform oldTransform;
        Transform transform;
        vec3 velocity;
        AABB aabb;

        EntityData(const vec3& posistion);

        ivec3 getChunkPosition() const;
        vec3 getChunkWorldPosition() const;
        AABB getWorldAABB() const;
        AABB getOldWorldAABB() const;
    };
}