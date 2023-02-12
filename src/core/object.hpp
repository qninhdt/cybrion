#pragma once

#include "core/registry.hpp"

namespace cybrion
{
    class Object
    {
    public:
        
        static Object Null;
        
        Object() : m_handle(entt::null) {}
        Object(const entt::entity& handle): m_handle(handle) {}
        Object(const Object& other) : Object(other.m_handle) {}

        Object& operator = (const Object& other) = default;
        Object& operator = (Object&& other) = default;

        bool valid() const
        {
            return GetRegistry().valid(m_handle);
        }
            
        template <typename Component, typename... Args>
        Component& assign(Args &&... args) {
            return GetRegistry().emplace<Component>(m_handle, std::forward<Args>(args)...);
        }

        template <typename Component>
        Object& remove() {
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