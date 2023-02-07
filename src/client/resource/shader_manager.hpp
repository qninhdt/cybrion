#pragma once

#include "client/GL/shader.hpp"

namespace cybrion
{
    class ShaderManager
    {
    public:
        bool loadShaders();
        bool reloadShaders();
        u32 getShaderCount() const;

        template <typename T>
        T getShader(const string& name)
        {
            T shader;
            if (m_shaders.find(name) == m_shaders.end())
                CYBRION_CLIENT_ERROR("Cannot find shader with name [{}]", name);

            shader.m_data = m_shaders[name];
            shader.loadUniforms();
            return shader;
        }

        ~ShaderManager();

    private:

        bool loadShader(const string& name, bool reloadable);

        umap<string, GL::detail::ShaderData*> m_shaders;
    };
}