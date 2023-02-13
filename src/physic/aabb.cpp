#include "physic/aabb.hpp"

namespace cybrion
{
    AABB::AABB(const vec3& position, const vec3& size):
        m_position(position),
        m_size(size)
    {
    }

    vec3 AABB::getMin() const
    {
        return m_position - (m_size / 2.0f);
    }

    vec3 AABB::getMax() const
    {
        return m_position + (m_size / 2.0f);
    }

    vec3 AABB::getPosition() const
    {
        return m_position;
    }

    vec3 AABB::getSize() const
    {
        return m_size;
    }

    // copy and modify from gamedev.net :3
    // https://www.gamedev.net/tutorials/programming/general-and-gameplay-programming/swept-aabb-collision-detection-and-response-r3084/
    SweptAABBResult AABB::SweptAABB(const AABB& a, const AABB& b, const ivec3& v)
    {
        f32 xInvEntry, yInvEntry, zInvEntry;
        f32 xInvExit, yInvExit, zInvExit;

        vec3 aMin = a.getMin();
        vec3 aMax = a.getMax();

        vec3 bMin = b.getMin();
        vec3 bMax = b.getMax();

        // find the distance between the objects on the near and far sides for both x and y 
        if (v.x > 0.0f)
        {
            xInvEntry = bMin.x - aMax.x;
            xInvExit  = bMax.x - aMin.x;
        }
        else
        {
            xInvEntry = bMin.x - aMin.x;
            xInvExit  = bMin.x - aMax.x;
        }

        if (v.y > 0.0f)
        {
            yInvEntry = bMin.y - aMax.y;
            yInvExit  = bMax.y - aMin.y;
        }
        else
        {
            yInvEntry = bMax.y - aMin.y;
            yInvExit  = bMin.y - aMax.y;
        }

        if (v.z > 0.0f)
        {
            zInvEntry = bMin.z - aMax.z;
            zInvExit  = bMax.z - aMin.z;
        }
        else
        {
            zInvEntry = bMax.z - aMin.z;
            zInvExit  = bMin.z - aMax.z;
        }

        // find time of collision and time of leaving for each axis (if statement is to prevent divide by zero) 
        f32 xEntry, yEntry, zEntry;
        f32 xExit, yExit, zExit;

        if (v.x == 0.0f)
        {
            xEntry = - INFINITY;
            xExit  = INFINITY;
        }
        else
        {
            xEntry = xInvEntry / v.x;
            xExit = xInvExit / v.x;
        }

        if (v.y == 0.0f)
        {
            yEntry = - INFINITY;
            yExit  = INFINITY;
        }
        else
        {
            yEntry = yInvEntry / v.y;
            yExit  = yInvExit / v.y;
        }

        if (v.z == 0.0f)
        {
            zEntry = -INFINITY;
            zExit  = INFINITY;
        }
        else
        {
            zEntry = zInvEntry / v.z;
            zExit  = zInvExit  / v.z;
        }

        // find the earliest/latest times of collision
        f32 entryTime = std::max({ xEntry, yEntry, zEntry });
        f32 exitTime = std::min({ xExit, yExit, zExit });

        // if there was no collision
        if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f && zEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f || zEntry > 1.0f)
            return { 1.0f, { 0, 0, 0 } };
        
        // if there was a collision 
        SweptAABBResult result{ entryTime, { 0, 0, 0 } };

        // calculate normal of collided surface
        if (xEntry > zEntry)
        {
            if (xEntry > yEntry)
            {
                if (xInvEntry < 0.0f)
                    result.normal.x = 1.0f;
                else
                    result.normal.x = -1.0f;
            }
            else
            {
                if (yInvEntry < 0.0f)
                    result.normal.y = 1.0f;
                else
                    result.normal.y = -1.0f;
            }
        }
        else
        {
            if (zEntry > yEntry)
            {
                if (zInvEntry < 0.0f)
                    result.normal.z = 1.0f;
                else
                    result.normal.z = -1.0f;
            }
            else
            {
                if (yInvEntry < 0.0f)
                    result.normal.y = 1.0f;
                else
                    result.normal.y = -1.0f;
            }
        }

        return result;
    }
}
