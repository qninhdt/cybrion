#include "world/entity/entity.hpp"

namespace cybrion
{
    EntityData::EntityData(const vec3& posistion)
    {
        transform.setPosition(posistion);
        oldTransform = transform;
    }
}