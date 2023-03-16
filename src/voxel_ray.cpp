#include "voxel_ray.hpp"
#include "util/math.hpp"

namespace cybrion
{
    void VoxelRay::Cast(const vec3& position, const vec3& direction, u32 maxDistance, CastCallback callback)
    {
        f32 x = floor(position.x);
        f32 y = floor(position.y);
        f32 z = floor(position.z);

        f32 dx = direction.x;
        f32 dy = direction.y;
        f32 dz = direction.z;

        i32 stepX = util::signNum(dx);
        i32 stepY = util::signNum(dy);
        i32 stepZ = util::signNum(dz);

        f32 tMaxX = util::intBound(position.x, dx);
        f32 tMaxY = util::intBound(position.y, dy);
        f32 tMaxZ = util::intBound(position.z, dz);

        f32 tDeltaX = stepX / dx;
        f32 tDeltaY = stepY / dy;
        f32 tDeltaZ = stepZ / dz;

        f32 radius = maxDistance ? maxDistance / sqrt(dx * dx + dy * dy + dz * dz) : INFINITY;

        ivec3 block{ x, y, z };
        ivec3 normal{ 0,0,0 };

        while (!callback(block, normal)) {
            if (tMaxX < tMaxY) {
                if (tMaxX < tMaxZ) {
                    if (tMaxX > radius) return;
                    // Update which cube we are now in.
                    block.x += stepX;
                    // Adjust tMaxX to the next X-oriented boundary crossing.
                    tMaxX += tDeltaX;
                    // Record the normal vector of the cube normal we entered.
                    normal[0] = -stepX;
                    normal[1] = 0;
                    normal[2] = 0;
                }
                else {
                    if (tMaxZ > radius) return;
                    block.z += stepZ;
                    tMaxZ += tDeltaZ;
                    normal[0] = 0;
                    normal[1] = 0;
                    normal[2] = -stepZ;
                }
            }
            else {
                if (tMaxY < tMaxZ) {
                    if (tMaxY > radius) return;
                    block.y += stepY;
                    tMaxY += tDeltaY;
                    normal[0] = 0;
                    normal[1] = -stepY;
                    normal[2] = 0;
                }
                else {
                    if (tMaxZ > radius) return;
                    block.z += stepZ;
                    tMaxZ += tDeltaZ;
                    normal[0] = 0;
                    normal[1] = 0;
                    normal[2] = -stepZ;
                }
            }
        }
    }
    bool VoxelRay::Intersection(const vec3& position, const vec3& direction, const AABB& aabb, ivec3& normal)
    {
        vec3 min = aabb.getMin();
        vec3 max = aabb.getMax();

        i32 stepX = util::signNum(direction.x);
        i32 stepY = util::signNum(direction.y);
        i32 stepZ = util::signNum(direction.z);

        f32 tx1 = (min.x - position.x) / direction.x;
        f32 tx2 = (max.x - position.x) / direction.x;

        f32 tmin = std::min(tx1, tx2);
        f32 tmax = std::max(tx1, tx2);

        f32 ty1 = (min.y - position.y) / direction.y;
        f32 ty2 = (max.y - position.y) / direction.y;

        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));

        f32 tz1 = (min.z - position.z) / direction.z;
        f32 tz2 = (max.z - position.z) / direction.z;

        tmin = std::max(tmin, std::min(tz1, tz2));
        tmax = std::min(tmax, std::max(tz1, tz2));

        if (tmin > tmax)
            return false;

        vec3 point = position + direction * tmin;
        vec3 localPoint = point - aabb.getPos();

        float minp = std::numeric_limits<float>::max();

        float distance = std::abs(aabb.getSize().x - std::abs(localPoint.x))/ aabb.getSize().x;
        if (distance < minp) {
            minp = distance;
            normal = { 1, 0, 0 };
            normal *= util::signNum(localPoint.x);
        }

        distance = std::abs(aabb.getSize().y - std::abs(localPoint.y))/ aabb.getSize().y;
        if (distance < minp) {
            minp = distance;
            normal = { 0, 1, 0 };
            normal *= util::signNum(localPoint.y);
        }

        distance = std::abs(aabb.getSize().z - std::abs(localPoint.z))/ aabb.getSize().z;
        if (distance < minp) {
            minp = distance;
            normal = { 0, 0, 1 };
            normal *= util::signNum(localPoint.z);
        }

        return true;
    }
}