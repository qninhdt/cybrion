#include "block_loader.hpp"
#include "client/client.hpp"

#define OVERRIDE(field, value)\
    for (auto& block: blocks) block->##field = value

namespace cybrion
{
    void BlockLoader::loadConfigFiles()
    {
        string folderPath = Client::Get().getResourcePath("configs/blocks/");
    
        for (auto& entry : std::filesystem::directory_iterator(folderPath))
        {
            string path = entry.path().generic_string();
            
            if (loadConfigFile(path))
            {
                CYBRION_GAME_TRACE("Loaded file {}", path);
            }
            else
            {
                CYBRION_GAME_WARN("Cannot load file {}", path);
            }
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
                        string key   { (*it1).begin(), (*it1).end() }; it1++;

                        if (it1 == view.end())
                        {
                            CYBRION_CLIENT_ERROR("An error occurred while loading {}", std::filesystem::path(path).filename().generic_string());
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
                for (auto it0 : value)
                {
                    string key = it0.first.as<string>();
                    string value = it0.second.as<string>();

                    if (key == "display_name") OVERRIDE(m_displayName, value);
                    if (key == "shape") OVERRIDE(m_shape, StringToEnum<BlockShape>(value));
                }
            }
        }

        return true;
    }
}
