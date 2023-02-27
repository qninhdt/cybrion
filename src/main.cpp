#include "client/application.hpp"
#include <jbt/hjbt.hpp>
#include <jbt/jbt.hpp>

using namespace cybrion;

int main()
{
    // block type
    RegisterEnum(BlockType::AIR  , "air");
    RegisterEnum(BlockType::SOIL , "soil");
    RegisterEnum(BlockType::LOG  , "log");
    RegisterEnum(BlockType::SAND , "sand");
    RegisterEnum(BlockType::ROCK , "rock");
    RegisterEnum(BlockType::BRICK, "brick");

    // dirt type
    RegisterEnum(SoilType::DIRT   , "dirt");
    RegisterEnum(SoilType::BASALT , "basalt");
    RegisterEnum(SoilType::CLAY   , "clay");
    RegisterEnum(SoilType::GRASS  , "grass");

    // rock type
    RegisterEnum(RockType::STONE, "stone");
    RegisterEnum(RockType::GRAVEL, "gravel");
    RegisterEnum(RockType::COBBLESTONE, "cobblestone");

    // brick type
    RegisterEnum(BrickType::STONE, "stone");

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

    // wood type
    RegisterEnum(WoodType::OAK, "oak");

    // log axis
    RegisterEnum(LogAxis::X, "x");
    RegisterEnum(LogAxis::Y, "y");
    RegisterEnum(LogAxis::Z, "z");

    Log::Init();

    Application app;

    if (app.open())
        CYBRION_CLIENT_TRACE("Main window is opened ({} x {})", app.getWidth(), app.getHeight());
    else    
        return EXIT_FAILURE;

    app.load();

    app.run();

    return EXIT_SUCCESS;
}
