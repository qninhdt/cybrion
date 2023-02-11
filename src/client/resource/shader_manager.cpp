#include "client/resource/shader_manager.hpp"
#include "client/application.hpp"
#include "util/file.hpp"

namespace cybrion
{
    ShaderManager* ShaderManager::s_shaderManager = nullptr;

    ShaderManager::ShaderManager()
    {
        s_shaderManager = this;
    }

    bool ShaderManager::loadShaders()
    {   

        u32 count = 0, total = 0;
        string configPath = Application::Get().getResourcePath("shader.yml");
     
        // check if config file exist
        if (!std::filesystem::exists(configPath))
        {
            CYBRION_CLIENT_ERROR("Missing file: shader.yml");
            CYBRION_CLIENT_ERROR("Cannot load shader programs");
            return false;
        }
        
        YAML::Node config = YAML::LoadFile(configPath);

        if (config["shader_list"])
        {
            YAML::Node shaderList = config["shader_list"];

            // check if shaderList is a list
            if (!shaderList.IsSequence())
            {
                CYBRION_CLIENT_ERROR("shader_list field is not a list");
                return false;
            }

            for (auto shader : shaderList)
            {
                string name = shader["name"].as<string>();
                bool reloadable = shader["reload"].as<bool>();
                if (!loadShader(name, reloadable))
                {
                    CYBRION_CLIENT_ERROR("An error occurred while loading shader {}", name);
                }
                else
                    count += 1;
                total += 1;
            }
        }
        else
        {
            CYBRION_CLIENT_ERROR("Shader list is missing in shader config file");
            return false;
        }

        if (total)
        {
            if (count == total)
                CYBRION_CLIENT_INFO("Loaded {} shaders successfully", total);
            else {
                CYBRION_CLIENT_WARN("Loaded {}/{} shaders", count, total);
                return false;
            }
        }
        else
        {
            CYBRION_CLIENT_ERROR("Failed to load shaders");
            return 0;
        }

        return true;
    }

    bool ShaderManager::reloadShaders()
    {
        u32 count = 0, total = 0;

        for (auto& [name, data] : m_shaders)
        {
            if (data->reloadable)
            {
                glDeleteProgram(data->programId);

                if (loadShader(name, data->reloadable))
                    count += 1;

                total += 1;
            }
        }

        if (total)
        {
            if (count == total)
                CYBRION_CLIENT_INFO("Reloaded {} shaders successfully", total);
            else {
                CYBRION_CLIENT_WARN("Reloaded {}/{} shaders", count, total);
                return false;
            }
        }

        return true;
    }

    ShaderManager::~ShaderManager()
    {
        for (auto& [name, data] : m_shaders)
            glDeleteProgram(data->programId);
    }

    ShaderManager& ShaderManager::Get()
    {
        return *s_shaderManager;
    }

    bool ShaderManager::loadShader(const string& name, bool reloadable)
    {
        GL::detail::ShaderData* data;
        
        if (m_shaders.find(name) == m_shaders.end())
        {
            data = new GL::detail::ShaderData;
            data->name = name;
            data->reloadable = reloadable;
            m_shaders[name] = data;
        }
        else
        {
            data = m_shaders[name];
        }

        string vertSource;
        string fragSource;

        // load shader sources

        string vertFile = name + ".vert";
        string fragFile = name + ".frag";

        string vertPath = Application::Get().getResourcePath("shaders/" + vertFile);
        string fragPath = Application::Get().getResourcePath("shaders/" + fragFile);

        if (!util::ReadFile(vertPath, vertSource))
        {
            CYBRION_CLIENT_ERROR("Missing file: {}", vertFile);
            return false;
        }

        if (!util::ReadFile(fragPath, fragSource))
        {
            CYBRION_CLIENT_ERROR("Missing file: {}", fragFile);
            return false;
        }
        
        // complie shader

        int vertSuccess, fragSuccess, programSuccess;
        char infoLog[512];

        // create vertex shader
        const GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);

        const GLchar* vertCSource = vertSource.c_str();
        glShaderSource(vertShader, 1, &vertCSource, NULL);

        // compile vertex shader
        glCompileShader(vertShader);

        glGetShaderiv(vertShader, GL_COMPILE_STATUS, &vertSuccess);
        if (!vertSuccess)
        {
            glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
            CYBRION_CLIENT_ERROR("Complie vertex shader [{0}]", data->name);
            CYBRION_CLIENT_ERROR("Vertex shader error {0}", infoLog);
            CYBRION_CLIENT_ERROR(infoLog);
            return false;
        }

        // create fragment shader
        const GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar* fragCSource = fragSource.c_str();
        glShaderSource(fragShader, 1, &fragCSource, NULL);

        // compile fragment shader
        glCompileShader(fragShader);

        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragSuccess);
        if (!fragSuccess)
        {
            glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
            CYBRION_CLIENT_ERROR("Complie fragment shader [{0}]", data->name);
            CYBRION_CLIENT_ERROR("Fragment shader error {0}", infoLog);
            CYBRION_CLIENT_ERROR(infoLog);
            return false;
        }

        // create shader program
        GLuint programId = glCreateProgram();

        glAttachShader(programId, vertShader);
        glAttachShader(programId, fragShader);

        glLinkProgram(programId);
        glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);

        if (!programSuccess || !vertSuccess || !fragSuccess)
        {
            glGetProgramInfoLog(programId, 512, NULL, infoLog);
            CYBRION_CLIENT_ERROR("Linking shader [{0}] failed", data->name);
            CYBRION_CLIENT_ERROR("Shader program error: {0}", infoLog);
            CYBRION_CLIENT_ERROR(infoLog);
            return false;
        }

        // delete shaders
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        data->programId = programId;

        return true;
    }

    u32 ShaderManager::getShaderCount() const
    {
        return (u32)m_shaders.size();
    }
}
