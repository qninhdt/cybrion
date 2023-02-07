#pragma once

#include "core/const_string.hpp"

namespace cybrion
{
    class ShaderManager;
}

namespace cybrion::GL
{
    namespace detail
    {
        struct ShaderData
        {
            string name;
            bool reloadable;
            GLuint programId;
        };

        template <const_string uniform>
        struct UniformHolder
        {
            GLint location;

            void loadUniform(GLuint programId)
            {
                glGetUniformLocation(programId, uniform);
            }
        };
    }

    template <const_string... uniforms>
    class Shader : private detail::UniformHolder<uniforms>...
    {
    public:
        template <const_string name, typename T>
        void setUniform(const T& value)
        {
            const GLint location = ((detail::UniformHolder<name>&) * this).location;
            if constexpr (std::is_same_v<T, f32>)
                glUniform1f(location, value);
            else if constexpr (std::is_same_v<T, vec2>)
                glUniform2f(location, value.x, value.y);
            else if constexpr (std::is_same_v<T, vec3>)
                glUniform3f(location, value.x, value.y, value.z);
            else if constexpr (std::is_same_v<T, vec4>)
                glUniform4f(location, value.x, value.y, value.z, value.w);
            else if constexpr (std::is_same_v<T, mat4>)
                glUniformMatrix4fv(location, 1, false, &value[0][0]);
            else
                CYBRION_STATIC_ERROR("ngu, sai type r!");
        }

        void use() const
        {
            glUseProgram(m_data->programId);
        }

    private:
        friend class cybrion::ShaderManager;

        void loadUniforms()
        {
            (((detail::UniformHolder<uniforms>&) * this).loadUniform(m_data->programId), ...);
        }

        detail::ShaderData* m_data;
    };
}
