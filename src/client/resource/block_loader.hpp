#pragma once

#include "client/GL/texture_array.hpp"

namespace cybrion
{
    class BlockLoader
    {
    public:

        BlockLoader();

        void load();

        u32 getTextureId(const string& name);

        void bindTextureArray();

        static BlockLoader& Get();

    private:
        static BlockLoader* s_blockLoader;

        void loadConfigFiles();
        void loadTextures();
        bool loadConfigFile(const string& path);

        GL::TextureArray m_textureArray;
        umap<string, u32> m_textureIdMap;

    };
}