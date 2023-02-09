#include "client/client.hpp"
#include "client/GL/mesh.hpp"
#include "client/camera.hpp"
#include "world/block/nature/soil_block.hpp"
#include "core/enum.hpp"

using namespace cybrion;

int main()
{
    // block type
    RegisterEnum(BlockType::AIR  , "air");
    RegisterEnum(BlockType::SOIL , "soil");

    // dirt type
    RegisterEnum(SoilType::DIRT   , "dirt");
    RegisterEnum(SoilType::BASALT , "basalt");
    RegisterEnum(SoilType::CLAY   , "clay");
    RegisterEnum(SoilType::GRASS  , "grass");

    // block shape
    RegisterEnum(BlockShape::CUBE   , "cube");
    RegisterEnum(BlockShape::CUSTOM , "custom");
    RegisterEnum(BlockShape::X      , "cube");

    // block display
    RegisterEnum(BlockDisplay::OPAQUE           , "opaque");
    RegisterEnum(BlockDisplay::TRANSPARENT      , "transparent");
    RegisterEnum(BlockDisplay::SEMI_OPAQUE      , "semi_opaque");
    RegisterEnum(BlockDisplay::SEMI_TRANSPARENT , "semi_transparent");

    // block rotation
    RegisterEnum(BlockRotation::R0   , "0");
    RegisterEnum(BlockRotation::R90  , "90");
    RegisterEnum(BlockRotation::R180 , "180");
    RegisterEnum(BlockRotation::R270 , "270");

    // block face
    RegisterEnum(BlockFace::EAST   , "east");
    RegisterEnum(BlockFace::TOP    , "top");
    RegisterEnum(BlockFace::SOUTH  , "south");
    RegisterEnum(BlockFace::WEST   , "west");
    RegisterEnum(BlockFace::BOTTOM , "bottom");
    RegisterEnum(BlockFace::NORTH  , "north");

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
