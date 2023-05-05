#pragma once

#include "client/ui/page.hpp"
#include "client/GL/texture.hpp"

namespace cybrion::ui
{
    struct WorldInfo
    {
        string name;
        float size;
    };

    class WorldListPage : public Page
    {
    public:
        WorldListPage();
        void onOpen() override;
        void onRender() override;
        void onClose() override;

    private:
        void loadWorldList();
        bool isValidName(const string &name) const;

        char m_worldInput[64];
        GL::Texture m_backgroundTexture;
        vector<WorldInfo> m_worldList;
    };
}