#include "world/entity/entity.hpp"
#include "world/chunk/chunk.hpp"
#include "util/math.hpp"

namespace cybrion
{
    std::atomic<u32> Entity::s_idN = 0;

    Entity::Entity(): Entity({ 0, 0, 0 })
    {
    }

    Entity::Entity(const vec3& pos): m_velocity(0), m_id(s_idN++)
    {
        m_pos = pos;
        m_posO = m_pos;
        m_rotO = m_rot;
    }

    u32 Entity::getId() const
    {
        return m_id;
    }

    ivec3 Entity::getChunkPos() const
    {
        return Chunk::posToChunkPos(getBlockPos());
    }

    ivec3 Entity::getBlockPos() const
    {
        return {
            std::floor(m_pos.x),
            std::floor(m_pos.y),
            std::floor(m_pos.z)
        };
    }

    vec3 Entity::lerpPos(f32 delta) const
    {
        return util::lerp(m_posO, m_pos, delta);
    }

    vec3 Entity::lerpRot(f32 delta) const
    {
        return util::lerpRot(m_rotO, m_rot, delta);
    }

    vec3 Entity::getVelocity() const
    {
        return m_velocity;
    }

    void Entity::setVelocity(const vec3& velocity)
    {
        m_velocity = velocity;
    }

    AABB Entity::getBB() const
    {
        return { m_localBB.getPos() + m_pos, m_localBB.getSize() };
    }

    AABB Entity::getOldBB() const
    {
        return { m_localBB.getPos() + m_posO, m_localBB.getSize() };
    }

    void Entity::setOldPosAndRot()
    {
        m_posO = m_pos;
        m_rotO = m_rot;
    }

    void Entity::tick()
    {
        
    }
}