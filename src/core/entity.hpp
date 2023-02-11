#pragma once

#include "core/registry.hpp"

namespace cybrion
{
    class Entity
    {
    public:
        
        Entity() : m_handle(entt::null) {}
        Entity(const entt::entity& handle): m_handle(handle) {}
        Entity(const Entity& other) : Entity(other.m_handle) {}

        Entity& operator = (const Entity& other) = default;
        Entity& operator = (Entity&& other) = default;

        bool valid() const
        {
            return GetRegistry().valid(m_handle);
        }
            
        template <typename Component, typename... Args>
        Component& assign(Args &&... args) {
            return GetRegistry().emplace<Component>(m_handle, std::forward<Args>(args)...);
        }

        template <typename Component>
        Entity& remove() {
            GetRegistry().remove<Component>(m_handle);
            return *this;
        }

        template <typename Component>
        Component& get() {
            return GetRegistry().get<Component>(m_handle);
        }

        template <typename Component>
        Component* tryGet() {
            return GetRegistry().try_get<Component>(m_handle);
        }

    private:
        entt::entity m_handle;
    };
}