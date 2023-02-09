#pragma once

#include "core/registry.hpp"

namespace cybrion
{
    class entity
    {
    public:
        
        entity() : entity(entt::entity(0)) {}

        entity(entt::entity handle) : m_registry(GetRegistry()), m_handle(handle) {}

        entity(const entity& other) : entity(other.m_handle) {}

        template <typename Component, typename... Args>
        entity& assign(Args &&... args) {
            m_registry.emplace<Component>(m_handle, std::forward<Args>(args)...);
            return *this;
        }

        template <typename Component>
        entity& remove() {
            m_registry.remove<Component>(m_handle);
            return *this;
        }

        template <typename Component>
        Component& get() {
            return m_registry.get<Component>(m_handle);
        }

    private:
        entt::entity m_handle;
        entt::registry& m_registry;
    };
}