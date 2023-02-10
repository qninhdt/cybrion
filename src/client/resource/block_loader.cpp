#include "block_loader.hpp"

#include "client/application.hpp"

#define OVERRIDE(field, value)\
    for (auto& block: blocks) block->##field = value

namespace cybrion
{
    void BlockLoader::load()
    {
        loadTextures();
        loadConfigFiles();
    }

    u32 BlockLoader::getTextureId(const string& name)
    {
        return m_textureIdMap[name];
    }

    void BlockLoader::bindTextureArray()
    {
        m_textureArray.bind(0);
    }

    void BlockLoader::loadConfigFiles()
    {
        string folderPath = Application::Get().getResourcePath("configs/blocks/");
    
        for (auto& entry : std::filesystem::directory_iterator(folderPath))
        {
            string path = entry.path().string();
            
            if (loadConfigFile(path))
                CYBRION_GAME_TRACE("Loaded file {}", path);
            else
                CYBRION_GAME_WARN("Cannot load file {}", path);
        }
    }

    void BlockLoader::loadTextures()
    {
        string folderPath = Application::Get().getResourcePath("textures/blocks/");

        // count number of block textures
        u32 layerCount = 0;
        for (auto& entry : std::filesystem::directory_iterator(folderPath)) {
            layerCount += 1;
        }

        // init texture array
        m_textureArray.init(16, 16, layerCount, 4, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);

        // loading textures
        stbi_set_flip_vertically_on_load(true);
        for (auto& entry : std::filesystem::directory_iterator(folderPath))
        {
            string path = entry.path().string();
            string name = entry.path().stem().string();
            CYBRION_CLIENT_TRACE("Loaded block texture: {}", name);

            i32 width, height, nchannels;
            u8* data = stbi_load(path.c_str(), &width, &height, &nchannels, STBI_rgb_alpha);

            if (width != 16 || height != 16)
            {
                CYBRION_CLIENT_ERROR("Incorrect texture size");
                continue;
            }

            u32 id = name == "no_texture" ? 0 : m_textureIdMap.size() + (m_textureIdMap.count("no_texture") == 0);
            m_textureIdMap[name] = id;

            m_textureArray.setSubImage(id, data);

            stbi_image_free(data);
        }

        for (auto& [name, id] : m_textureIdMap)
        {
            std::cout << name << " : " << id << '\n';
        }
    }

    bool BlockLoader::loadConfigFile(const string& path)
    {
        YAML::Node config = YAML::LoadFile(path);
        BlockType type = StringToEnum<BlockType>(std::filesystem::path(path).stem().string());

        for (auto it : config)
        {
            // handle key
            string key = it.first.as<string>();
            bool is_state = key.starts_with("[") && key.ends_with("]");

            if (is_state)
            {
                // erase "[" and "]"
                key.erase(key.begin());
                key.erase(key.end()-1);

                umap<string, string> stateMap;

                bool validKey = true;
                if (!key.empty())
                {
                    for (auto it0 : std::views::split(key, ','))
                    {
                        auto view = std::views::split(string(it0.begin(), it0.end()), '=');
                        auto it1 = view.begin();
                        string key { (*it1).begin(), (*it1).end() }; it1++;

                        if (it1 == view.end())
                        {
                            CYBRION_CLIENT_ERROR("An error occurred while loading {}", std::filesystem::path(path).filename().string());
                            validKey = false;
                            break;
                        }

                        string value { (*it1).begin(), (*it1).end() };
                        
                        stateMap[key] = value;
                    }
                }

                if (!validKey) continue;

                vector<Block*> blocks;

                Game::Get().getBlockRegistry().queryBlocks(type, stateMap, blocks);

                // handle value
                YAML::Node value = it.second;
                BlockLoader& loader = Game::Get().getBlockLoader();
                for (auto it0 : value)
                {
                    string key = it0.first.as<string>();
                    string value = it0.second.as<string>();

                    if (key == "display_name") OVERRIDE(m_displayName, value);
                    if (key == "shape") OVERRIDE(m_shape, StringToEnum<BlockShape>(value));

                    if (key == "rotate_x") OVERRIDE(m_rotationX, StringToEnum<BlockRotation>(value));
                    if (key == "rotate_y") OVERRIDE(m_rotationY, StringToEnum<BlockRotation>(value));
                    if (key == "rotate_z") OVERRIDE(m_rotationZ, StringToEnum<BlockRotation>(value));

                    if (key == "all")
                    {
                        u32 id = loader.getTextureId(value);
                        OVERRIDE(m_topTexture    , id);
                        OVERRIDE(m_bottomTexture , id);
                        OVERRIDE(m_northTexture  , id);
                        OVERRIDE(m_southTexture  , id);
                        OVERRIDE(m_eastTexture   , id);
                        OVERRIDE(m_westTexture   , id);
                    }

                    if (key == "side")
                    {
                        u32 id = loader.getTextureId(value);
                        OVERRIDE(m_northTexture , id);
                        OVERRIDE(m_southTexture , id);
                        OVERRIDE(m_eastTexture  , id);
                        OVERRIDE(m_westTexture  , id);
                    }

                    if (key == "top")
                    {
                        u32 id = loader.getTextureId(value);
                        OVERRIDE(m_topTexture  , id);
                    }

                    if (key == "bottom")
                    {
                        u32 id = loader.getTextureId(value);
                        OVERRIDE(m_bottomTexture, id);
                    }
                }
            }
        }

        return true;
    }
}
