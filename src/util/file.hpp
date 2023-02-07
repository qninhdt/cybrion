#pragma once

namespace cybrion::util
{
    bool ReadFile(const string& path, string& content);
    string GetFileNameFromPath(const string& path);
}