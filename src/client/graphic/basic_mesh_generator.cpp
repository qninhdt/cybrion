#include "client/graphic/basic_mesh_generator.hpp"
#include "client/graphic/block_renderer.hpp"

namespace cybrion
{
    void BasicMeshGenerator::LineCubeMesh(GL::Mesh& mesh, f32 scale, vec3 color)
    {
        mesh.setAttributes({
            { GL::Type::VEC3 },
            { GL::Type::VEC3 }
        });

        vector<f32> vertices;
        vector<u32> indices;
        for (auto& face : BlockRenderer::CubeVertices)
        {
            u32 id = vertices.size() / 6;
            for (auto& vert : face)
            {
                vertices.insert(vertices.end(), {
                    vert.x * scale, vert.y * scale, vert.z * scale,
                    color.r, color.g, color.b
                });
            }

            indices.insert(indices.end(), {
                id + 0, id + 1,
                id + 1, id + 2,
                id + 2, id + 3,
                id + 3, id + 0
            });
        }

        mesh.setDrawCount(48);
        mesh.setIndices(indices.data(), indices.size());
        mesh.setVertices(vertices.data(), vertices.size());
    }
}