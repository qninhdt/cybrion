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

        i32 stepX = util::SignNum(dx);
        i32 stepY = util::SignNum(dy);
        i32 stepZ = util::SignNum(dz);

        f32 tMaxX = util::IntBound(position.x, dx);
        f32 tMaxY = util::IntBound(position.y, dy);
        f32 tMaxZ = util::IntBound(position.z, dz);

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
}