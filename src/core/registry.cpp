#include "core/registry.hpp"

entt::registry registry;

namespace cybrion
{
    entt::registry& GetRegistry()
    {
        return registry;
    }
}