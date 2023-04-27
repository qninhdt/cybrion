#include "client/graphic/entity_renderer.hpp"
#include "client/graphic/basic_mesh_generator.hpp"

namespace cybrion
{
    EntityRenderer::EntityRenderer(const ref<Entity> &entity) : m_entity(entity)
    {
        BasicMeshGenerator::LineCubeMesh(aabbMesh, 1, {0, 0, 1});
        aabbMesh.setScale(entity->getBB().getSize());
    }

    void EntityRenderer::tick(f32 delta)
    {
        aabbMesh.setPos(util::lerp(
            m_entity->getOldBB().getPos(),
            m_entity->getBB().getPos(),
            delta));
        aabbMesh.updateModelMat();
    }
}