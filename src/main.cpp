#include "client/client.hpp"
#include "client/GL/mesh.hpp"
#include "client/camera.hpp"
#include "world/block/nature/dirt_block.hpp"
#include "core/enum.hpp"

using namespace cybrion;

int main()
{
    // block type
    RegisterEnum(BlockType::AIR  , "air");
    RegisterEnum(BlockType::DIRT , "dirt");

    // dirt type
    RegisterEnum(DirtType::SOIL   , "soil");
    RegisterEnum(DirtType::BASALT , "basalt");
    RegisterEnum(DirtType::CLAY   , "clay");

    // block shape
    RegisterEnum(BlockShape::CUBE   , "cube");
    RegisterEnum(BlockShape::CUSTOM , "custom");
    RegisterEnum(BlockShape::X      , "cube");

    Log::Init();

    Client client;
    Window& window = client.getWindow();

    if (window.open())
        CYBRION_CLIENT_TRACE("Main window is opened ({} x {})", window.getWidth(), window.getHeight());

    window.disableCursor();

    client.loadResources();

    client.init();
    client.start();
}
