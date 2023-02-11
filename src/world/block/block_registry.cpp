#include "block_registry.hpp"

namespace cybrion
{
    BlockRegistry* BlockRegistry::s_blockRegistry = nullptr;

    BlockRegistry::BlockRegistry()
    {
        s_blockRegistry = this;
    }

    BlockRegistry& BlockRegistry::Get()
    {
        return *s_blockRegistry;
    }

}
