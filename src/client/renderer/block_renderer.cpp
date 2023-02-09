#include "client/renderer/block_renderer.hpp"

namespace cybrion
{
    array<tuple<ivec3, BlockFace>, 6> BlockRenderer::CubeBlockDirections = { {
        { { +1, 0, 0 }, BlockFace::EAST   },
        { { 0, +1, 0 }, BlockFace::TOP    },
        { { 0, 0, +1 }, BlockFace::SOUTH  },
        { { -1, 0, 0 }, BlockFace::WEST   },
        { { 0, -1, 0 }, BlockFace::BOTTOM },
        { { 0, 0, -1 }, BlockFace::NORTH  },
    } };

    array<array<tuple<vec3, vec2>, 2 * 3>, 6> BlockRenderer::DefaultCubeBlockVertices = { {
        // Right face
        {{
            { { +0.5f, -0.5f, +0.5f }, { 0, 0 } },  // top-left
            { { +0.5f, +0.5f, +0.5f }, { 0, 1 } },  // top-right      
            { { +0.5f, +0.5f, -0.5f }, { 1, 1 } },  // bottom-right          
            { { +0.5f, -0.5f, -0.5f }, { 1, 0 } },  // bottom-left
        }},
        // Top face
        {{
            { { -0.5f, +0.5f, +0.5f }, { 0, 0 } },  // bottom-right
            { { -0.5f, +0.5f, -0.5f }, { 0, 1 } },  // bottom-left  
            { { +0.5f, +0.5f, -0.5f }, { 1, 1 } },  // top-left    
            { { +0.5f, +0.5f, +0.5f }, { 1, 0 } },  // top-right

        }},
        // Front face
        {{
            { { -0.5f, -0.5f, +0.5f }, { 0, 0 } },  // top-right
            { { -0.5f, +0.5f, +0.5f }, { 0, 1 } },  // bottom-right        
            { { +0.5f, +0.5f, +0.5f }, { 1, 1 } },  // bottom-left
            { { +0.5f, -0.5f, +0.5f }, { 1, 0 } },  // top-left       
        }},
        // Left face
        {{
            { { -0.5f, -0.5f, -0.5f }, { 0, 0 } }, // bottom-left
            { { -0.5f, +0.5f, -0.5f }, { 0, 1 } }, // top-left       
            { { -0.5f, +0.5f, +0.5f }, { 1, 1 } }, // top-right
            { { -0.5f, -0.5f, +0.5f }, { 1, 0 } }, // bottom-right

        }},
        // Bottom face
        {{
            { { -0.5f, -0.5f, -0.5f }, { 0, 0 } }, // bottom-left
            { { -0.5f, -0.5f, +0.5f }, { 0, 1 } }, // top-left        
            { { +0.5f, -0.5f, +0.5f }, { 1, 1 } }, // top-right
            { { +0.5f, -0.5f, -0.5f }, { 1, 0 } }, // bottom-right
        }},
        // Back face
        {{
            { { +0.5f, -0.5f, -0.5f }, { 0, 0 } }, // bottom-left
            { { +0.5f, +0.5f, -0.5f }, { 0, 1 } }, // bottom-right    
            { { -0.5f, +0.5f, -0.5f }, { 1, 1 } }, // top-right              
            { { -0.5f, -0.5f, -0.5f }, { 1, 0 } }, // top-left      
        }},
    } };

    void BlockRenderer::generateCubeMesh()
    {

    }
}