#include "client/client.hpp"
#include "client/GL/mesh.hpp"
#include "client/camera.hpp"

using namespace cybrion;

int main()
{
    Log::Init();

    Client client;
    Window& window = client.getWindow();

    if (window.open())
        CYBRION_CLIENT_TRACE("Main window is opened ({} x {})", window.getWidth(), window.getHeight());

    GL::Mesh::GenerateGlobalIBO();
    client.loadResources();

    using BasicShader = GL::Shader<"MVP">;
    auto shader = client.getShaderManager().getShader<BasicShader>("basic");

    GL::Mesh mesh(true);

    mesh.setAttributes({
        { GL::Type::VEC3 }
    });
    
    mesh.setRotation({ 0, 0, 0.5f });
    mesh.updateModelMatrix();

    f32 vertices[] = {
        +0.5f, -0.5f, 0,
        -0.5f, -0.5f, 0,
        -0.5f, +0.5f, 0,
        +0.5f, +0.5f, 0,
    };

    mesh.setVertices(vertices, 12);
    mesh.setDrawCount(6);

    Camera camera(800.0f/600, glm::radians(50.0f), 0.2f, 1000.0f);
    camera.setPosition({ 0, 0, -1 });
    camera.updateViewMatrix();

    while (!window.isClosed())
    {
        window.beginLoop();

        shader.use();
        shader.setUniform<"MVP">(camera.getProjectionViewMatrix() * mesh.getModelMatrix());
        mesh.drawTriangles();

        window.endLoop();
    }
}