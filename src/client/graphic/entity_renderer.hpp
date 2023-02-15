#pragma once

#include "client/GL/mesh.hpp"
#include "world/entity/entity.hpp"

namespace cybrion
{
    class EntityRenderer
    {
    public:

        EntityRenderer(const ref<Entity>& entity);

        void tick(f32 delta);

        ref<Entity> m_entity;

        GL::Mesh mesh;
        GL::Mesh aabbMesh;
    };
}