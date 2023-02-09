#pragma once

#include "client/GL/texture_array.hpp"

namespace cybrion
{
    class BlockLoader
    {
    public:

        void load();

    private:

        void loadConfigFiles();
        void loadTextures();
        bool loadConfigFile(const string& path);

        GL::TextureArray m_textureArray;
        umap<string, u32> m_textureIdMap;

    };
}