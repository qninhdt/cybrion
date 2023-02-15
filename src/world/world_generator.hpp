#pragma once

#include "world/chunk/chunk.hpp"

namespace cybrion
{
    class WorldGenerator
    {
    public:

        ref<Chunk> generateChunkAt(const ivec3& pos);
    };
}