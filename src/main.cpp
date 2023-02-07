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

    window.disableCursor();

    GL::Mesh::GenerateGlobalIBO();
    client.loadResources();

    client.start();
}