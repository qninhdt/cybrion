#pragma once

#include "core/entity.hpp"

namespace cybrion
{
    class ChunkSystem
    {
    public:
        ChunkSystem();
    private:
        entt::registry& m_registry;
    };
}