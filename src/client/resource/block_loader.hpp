#pragma once

#include "client/GL/texture_array.hpp"
#include "world/block/block.hpp"

namespace cybrion
{
    class BlockLoader
    {
    public:

        BlockLoader();

        void load();

        u32 getTextureId(const string& name);

        ref<BlockModel> getModel(const string& name) const;

        void bindTextureArray();

        static BlockLoader& Get();

    private:
        static BlockLoader* s_blockLoader;

        void loadConfigFiles();
        void loadTextures();
        void loadModels();

        ref<BlockModel> loadObjFile(const string& path);

        bool loadConfigFile(const string& path);

        GL::TextureArray m_textureArray;
        umap<string, u32> m_textureIdMap;
        umap<string, ref<BlockModel>> m_modelMap;
    };
}