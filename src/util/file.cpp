#include "util/file.hpp"

namespace cybrion::util
{
    bool ReadFile(const string& path, string& content)
    {
        std::ifstream file(path);

        if (!file.is_open())
        {
            content = "";
            return false;
        }

        // avoid automatic reallocation
        file.seekg(0, std::ios::end);
        content.reserve(file.tellg());
        file.seekg(0, std::ios::beg);

        // read text
        content.assign(std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>());

        file.close();
        return true;
    }
}