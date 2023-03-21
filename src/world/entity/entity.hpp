#pragma once

#include "core/transform.hpp"
#include "physic/aabb.hpp"

namespace cybrion
{
    class Entity : public BasicTransform
    {
    public:

        Entity();
        Entity(const vec3& pos);

        u32 getId() const;

        ivec3 getChunkPos() const;
        ivec3 getBlockPos() const;

        vec3 lerpPos(f32 delta) const;
        vec3 lerpRot(f32 delta) const;

        vec3 getVelocity() const;
        void setVelocity(const vec3& velocity);

        AABB getLocalBB() const;
        AABB getBB() const;
        AABB getOldBB() const;

        void addForce() const;

        void setOldPosAndRot();

        virtual void tick();

    private:

        static std::atomic<u32> s_idN;

        vec3 m_rotO;
        vec3 m_posO;
        
        f32 m_mass;
        vec3 m_velocity;
        AABB m_localBB;

        u32 m_id;
    };
}