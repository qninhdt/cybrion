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

    void BlockRenderer::generateCubeMesh(bool culling[6], const vec3& position, Block* (&neighbors)[3][3][3], CubeVertex* result, u32& index)
    {
        for (u32 i = 0; i < 6; ++i)
        {
            if (culling[i]) continue;

            auto& adjs = faceAdjBlocks[i];

            CubeVertex* v[4];
            for (u32 j = 0; j < 4; ++j)
            {
                ivec3 p1 = adjs[(j * 2 + 0) % 8];
                ivec3 p2 = adjs[(j * 2 + 1) % 8];
                ivec3 p3 = adjs[(j * 2 + 2) % 8];

                Block* b1 = neighbors[p1.x + 1][p1.y + 1][p1.z + 1];
                Block* b2 = neighbors[p2.x + 1][p2.y + 1][p2.z + 1];
                Block* b3 = neighbors[p3.x + 1][p3.y + 1][p3.z + 1];

                bool o1 = b1 && b1->getDisplay() == BlockDisplay::OPAQUE;
                bool o2 = b2 && b2->getDisplay() == BlockDisplay::OPAQUE;
                bool o3 = b3 && b3->getDisplay() == BlockDisplay::OPAQUE;

                u32 ao;

                if (o1 && o3)
                    ao = 0;
                else
                    ao = 3 - (o1 + o2 + o3);

                result[index] = {
                    CubeVertices[i][j] + position,
                    m_cubeTexture[i][j].uv,
                    m_cubeTexture[i][j].textureId,
                    i,
                    ao
                };

                v[j] = &result[index];
                
                index += 1;
            }

            if (v[0]->ao + v[2]->ao > v[1]->ao + v[3]->ao)
            {
                cycle4(*v[0], *v[1], *v[2], *v[3], 2);
            }
            else
            {
                cycle4(*v[0], *v[1], *v[2], *v[3], 1);
            }
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
}