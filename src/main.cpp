#include "client/client.hpp"
#include "client/GL/mesh.hpp"

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

    f32 vertices[] = {
        +0.5f, -0.5f, 0,
        -0.5f, -0.5f, 0,
        -0.5f, +0.5f, 0,
        +0.5f, +0.5f, 0,
    };

    mesh.setVertices(vertices, 12);
    mesh.setDrawCount(6);

    while (!window.isClosed())
    {
        window.beginLoop();

        shader.use();
        mesh.drawTriangles();

        window.endLoop();
    }
}