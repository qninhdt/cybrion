#pragma once

#include "client/GL/mesh.hpp"

namespace cybrion
{
    class BasicMeshGenerator
    {
    public:
        static void LineCubeMesh(GL::Mesh& mesh, f32 scale, vec3 color);

    private:
    };
}