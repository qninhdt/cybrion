#pragma once

#include "client/graphic/entity_renderer.hpp"
#include "client/graphic/basic_mesh_generator.hpp"

namespace cybrion
{
    EntityRenderer::EntityRenderer(const ref<Entity>& entity)
    {
        aabbMesh.setScale(entity->getBB().getSize());
        BasicMeshGenerator::LineCubeMesh(aabbMesh, 1, { 0, 0, 1 });
    }

    void EntityRenderer::tick(f32 delta)
    {
        aabbMesh.setPos(m_entity->lerpPos(delta));
        aabbMesh.setRot(m_entity->lerpRot(delta));
        aabbMesh.updateModelMat();
    }
}