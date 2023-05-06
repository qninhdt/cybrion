#pragma once

#include "client/GL/mesh.hpp"
#include "client/GL/shader.hpp"

namespace cybrion
{
    class SkyRenderer
    {
    public:
        using SkyShader = GL::Shader<"projection_view">;

        SkyRenderer();
        void render();

    private:
        static float Vertices[];

        SkyShader m_shader;
        GL::Mesh m_mesh;
    };
}