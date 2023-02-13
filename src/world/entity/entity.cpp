#include "world/entity/entity.hpp"
#include "world/chunk/chunk.hpp"

namespace cybrion
{
    EntityData::EntityData(const vec3& posistion)
    {
        transform.setPosition(posistion);
        oldTransform = transform;
    }

    ivec3 EntityData::getChunkPosition() const
    {
        vec3 pos = transform.getPosition();

        return {
            floor(pos.x / CHUNK_SIZE),
            floor(pos.y / CHUNK_SIZE),
            floor(pos.z / CHUNK_SIZE),
        };
    }

    vec3 EntityData::getChunkWorldPosition() const
    {
        ivec3 chunkPos = getChunkPosition();
        return {
            chunkPos.x* CHUNK_SIZE + CHUNK_SIZE / 2,
            chunkPos.y* CHUNK_SIZE + CHUNK_SIZE / 2,
            chunkPos.z* CHUNK_SIZE + CHUNK_SIZE / 2,
        };
    }
}