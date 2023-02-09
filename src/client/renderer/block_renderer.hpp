#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    struct CubeVertice
    {
        vec3 position;
        vec2 uv;
        u32 textureId;
    };

    class BlockRenderer
    {
        using CubeMesh = array<array<CubeVertice, 6>, 6>;
    public:

        void generateCubeMesh();
    
    private:
        static array<tuple<ivec3, BlockFace>, 6> CubeBlockDirections;
        static array<array<tuple<vec3, vec2>, 6>, 6> DefaultCubeBlockVertices;

        Block* m_block;

        CubeMesh m_cubeMesh;
    };
}