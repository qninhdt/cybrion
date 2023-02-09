#pragma once

namespace cybrion
{
    class BlockLoader
    {
    public:

        void loadConfigFiles();

    private:

        bool loadConfigFile(const string& path);

    };
}