#pragma once

#include "world/block/block.hpp"

namespace cybrion
{
    struct CubeVertex
    {
        vec3 position;
        vec2 uv;
        u32 textureId;
        u32 face;
        u32 ao;
    };
    
    struct CubeVertexTexture
    {
        vec2 uv;
        u32 textureId;
    };

    class BlockRenderer
    {
        using CubeTexture = array<array<CubeVertexTexture, 4>, 6>;
    public:

        void generateCubeMesh(bool culling[6], const vec3& position, const Block::Block3x3x3& blocks, vector<CubeVertex>& result);
        void generateCubeTexture();
    
        static array<tuple<ivec3, BlockFace>, 6> CubeDirections;
        static array<array<vec3, 4>, 6> CubeVertices;

    private:

        friend class LocalGame;

        template <typename T>
        void cycle4(T& a, T& b, T& c, T& d, u32 n)
        {
            while (n--)
            {
                T e = d;
                d = c;
                c = b;
                b = a;
                a = e;
            }
        }

        static CubeTexture DefaultCubeTexture;

        Block* m_block = nullptr;

        CubeTexture m_cubeTexture;
    };
}