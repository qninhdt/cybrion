#include "client/graphic/block_renderer.hpp"

namespace cybrion
{
    array<tuple<ivec3, BlockFace>, 6> BlockRenderer::CubeDirections = { {
        { { +1, 0, 0 }, BlockFace::EAST   },
        { { 0, +1, 0 }, BlockFace::TOP    },
        { { 0, 0, +1 }, BlockFace::SOUTH  },
        { { -1, 0, 0 }, BlockFace::WEST   },
        { { 0, -1, 0 }, BlockFace::BOTTOM },
        { { 0, 0, -1 }, BlockFace::NORTH  },
    } };

    array<array<vec3, 4>, 6> BlockRenderer::CubeVertices = { {
        // Right face
        {{
            { +0.5f, -0.5f, +0.5f },  // top-left
            { +0.5f, +0.5f, +0.5f },  // top-right      
            { +0.5f, +0.5f, -0.5f },  // bottom-right          
            { +0.5f, -0.5f, -0.5f },  // bottom-left
        }},
        // Top face
        {{
            { +0.5f, +0.5f, +0.5f },  // top-right
            { -0.5f, +0.5f, +0.5f },  // bottom-right
            { -0.5f, +0.5f, -0.5f },  // bottom-left  
            { +0.5f, +0.5f, -0.5f },  // top-left    
        }},
        // Front face
        {{
            { -0.5f, -0.5f, +0.5f },  // top-right
            { -0.5f, +0.5f, +0.5f },  // bottom-right        
            { +0.5f, +0.5f, +0.5f },  // bottom-left
            { +0.5f, -0.5f, +0.5f },  // top-left       
        }},
        // Left face
        {{
            { -0.5f, -0.5f, -0.5f },  // bottom-left
            { -0.5f, +0.5f, -0.5f },  // top-left       
            { -0.5f, +0.5f, +0.5f },  // top-right
            { -0.5f, -0.5f, +0.5f },  // bottom-right

        }},
        // Bottom face
        {{
            { -0.5f, -0.5f, +0.5f },  // top-left        
            { +0.5f, -0.5f, +0.5f },  // top-right
            { +0.5f, -0.5f, -0.5f },  // bottom-right
            { -0.5f, -0.5f, -0.5f },  // bottom-left
        }},
        // Back face
        {{
            { +0.5f, -0.5f, -0.5f },  // bottom-left
            { +0.5f, +0.5f, -0.5f },  // bottom-right    
            { -0.5f, +0.5f, -0.5f },  // top-right              
            { -0.5f, -0.5f, -0.5f },  // top-left      
        }},
    } };

    BlockRenderer::CubeTexture BlockRenderer::DefaultCubeTexture = { {
        // Right face
        {{
            { { 0, 0 }, 0 },  // top-left
            { { 0, 1 }, 0 },  // top-right      
            { { 1, 1 }, 0 },  // bottom-right          
            { { 1, 0 }, 0 },  // bottom-left
        }},
        // Top face
        {{
            { { 0, 0 }, 0 },  // bottom-right
            { { 0, 1 }, 0 },  // bottom-left  
            { { 1, 1 }, 0 },  // top-left    
            { { 1, 0 }, 0 },  // top-right
        }},
        // Front face
        {{
            { { 0, 0 }, 0 },  // top-right
            { { 0, 1 }, 0 },  // bottom-right        
            { { 1, 1 }, 0 },  // bottom-left
            { { 1, 0 }, 0 },  // top-left       
        }},
        // Left face
        {{
            { { 0, 0 }, 0 }, // bottom-left
            { { 0, 1 }, 0 }, // top-left       
            { { 1, 1 }, 0 }, // top-right
            { { 1, 0 }, 0 }, // bottom-right
        }},
        // Bottom face
        {{
            { { 0, 0 }, 0 }, // bottom-left
            { { 0, 1 }, 0 }, // top-left        
            { { 1, 1 }, 0 }, // top-right
            { { 1, 0 }, 0 }, // bottom-right
        }},
        // Back face
        {{
            { { 0, 0 }, 0 }, // bottom-left
            { { 0, 1 }, 0 }, // bottom-right    
            { { 1, 1 }, 0 }, // top-right              
            { { 1, 0 }, 0 }, // top-left      
        }},
    } };


    array<array<ivec3, 8>, 6> faceAdjBlocks = { {
        {{ {1,-1,0}, {1,-1,1}, { 1,0,1}, {1,1,1}, {1,1,0},{1,1,-1},{1,0,-1},{1,-1,-1} }},
        {{ {1,1,0},{1,1,1},{0,1,1},{-1,1,1},{-1,1,0},{-1,1,-1},{0,1,-1},{1,1,-1} }},
        {{ {0,-1,1}, {-1,-1,1},{-1,0,1},{-1,1,1},{0,1,1},{1,1,1},{1,0,1},{1,-1,1}}},
        {{ {-1,-1,0}, {-1,-1,-1},{-1,0,-1},{-1,1,-1},{-1,1,0},{-1,1,1},{-1,0,1},{-1,-1,1} }},
        {{ {-1,-1,0},{-1,-1,1},{0,-1,1},{1,-1,1},{1,-1,0},{1,-1,-1},{0,-1,-1},{-1,-1,-1} }},
        {{ {0,-1,-1}, {1,-1,-1},{1,0,-1},{1,1,-1},{0,1,-1},{-1,1,-1},{-1,0,-1},{-1,-1,-1} }}
    } };

    void BlockRenderer::generateCubeMesh(bool culling[6], const ivec3& position, const Block::Block3x3x3& blocks, vector<u32>& result)
    {
        for (u32 i = 0; i < 6; ++i)
        {
            if (culling[i]) continue;

            auto& adjs = faceAdjBlocks[i];

            u32* v[4];
            result.reserve(result.size() + 4);
            for (u32 j = 0; j < 4; ++j)
            {
                ivec3 p1 = adjs[(j * 2 + 0) % 8];
                ivec3 p2 = adjs[(j * 2 + 1) % 8];
                ivec3 p3 = adjs[(j * 2 + 2) % 8];

                Block* b1 = blocks[p1.x + 1][p1.y + 1][p1.z + 1];
                Block* b2 = blocks[p2.x + 1][p2.y + 1][p2.z + 1];
                Block* b3 = blocks[p3.x + 1][p3.y + 1][p3.z + 1];

                bool o1 = b1 && b1->getDisplay() == BlockDisplay::OPAQUE && b1->getShape() == BlockShape::CUBE;
                bool o2 = b2 && b2->getDisplay() == BlockDisplay::OPAQUE && b2->getShape() == BlockShape::CUBE;
                bool o3 = b3 && b3->getDisplay() == BlockDisplay::OPAQUE && b3->getShape() == BlockShape::CUBE;

                u32 ao;

                if (o1 && o3)
                    ao = 0;
                else
                    ao = 3 - (o1 + o2 + o3);

                ivec3 pos = ivec3(CubeVertices[i][j] + vec3(0.5f, 0.5f, 0.5f)) + position;
                result.push_back(packCubeVertex(
                    pos.x, pos.y, pos.z,
                    i, ao,
                    m_cubeTexture[i][j].textureId
                ));

                v[j] = &result[result.size() - 1];
            }

       /*     if (v[0]->ao + v[2]->ao > v[1]->ao + v[3]->ao)
                cycle4(*v[0], *v[1], *v[2], *v[3], 2);
            else
                cycle4(*v[0], *v[1], *v[2], *v[3], 1);*/
        }
    }

    void BlockRenderer::generateCubeTexture()
    {
        m_cubeTexture = DefaultCubeTexture;

        for (u32 i = 0; i < 6; ++i)
            for (auto& [_, textId] : m_cubeTexture[i])
                textId = m_block->getTexture(i);

        auto& t = m_cubeTexture;

        // rotate x
        u32 nx = (u32) m_block->getRotationX();
        cycle4(t[1], t[2], t[4], t[5], nx);

        cycle4(t[0][3].uv, t[0][2].uv, t[0][1].uv, t[0][0].uv, nx);
        cycle4(t[3][0].uv, t[3][1].uv, t[3][2].uv, t[3][3].uv, nx);

        cycle4(t[1][3].uv, t[1][2].uv, t[1][1].uv, t[1][0].uv, nx);
        cycle4(t[2][3].uv, t[2][2].uv, t[2][1].uv, t[2][0].uv, nx);
        cycle4(t[4][3].uv, t[4][2].uv, t[4][1].uv, t[4][0].uv, nx);
        cycle4(t[5][3].uv, t[5][2].uv, t[5][1].uv, t[5][0].uv, nx);

        // rotate y
        u32 ny = (u32)m_block->getRotationY();
        cycle4(t[2], t[0], t[5], t[3], ny);
        cycle4(t[1][3].uv, t[1][2].uv, t[1][1].uv, t[1][0].uv, ny);
        cycle4(t[4][0].uv, t[4][1].uv, t[4][2].uv, t[4][3].uv, ny);

        // rotate z
        u32 nz = (u32)m_block->getRotationZ();
        cycle4(t[0], t[1], t[3], t[4], nz);
        cycle4(t[2][3].uv, t[2][2].uv, t[2][1].uv, t[2][0].uv, nz);
        cycle4(t[5][0].uv, t[5][1].uv, t[5][2].uv, t[5][3].uv, nz);

        if (nz != 0)
        {
            cycle4(t[3][3].uv, t[3][2].uv, t[3][1].uv, t[3][0].uv, 2);

            if (nz == 1) cycle4(t[4][3].uv, t[4][2].uv, t[4][1].uv, t[4][0].uv, 2);
            if (nz == 2) cycle4(t[0][3].uv, t[0][2].uv, t[0][1].uv, t[0][0].uv, 2);
            if (nz == 3) cycle4(t[1][3].uv, t[1][2].uv, t[1][1].uv, t[1][0].uv, 2);
        }
    }
    u32 BlockRenderer::packCubeVertex(u32 x, u32 y, u32 z, u32 normal, u32 ao, u32 texId)
    {
        u32 n = texId;
        n = n << 2 | ao;
        n = n << 3 | normal;
        n = n << 6 | z;
        n = n << 6 | y;
        n = n << 6 | x;
        return n;
    }
}