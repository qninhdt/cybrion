#include "client/application.hpp"
#include "physic/aabb.hpp"

using namespace cybrion;

int main()
{
    // block type
    RegisterEnum(BlockType::AIR  , "air");
    RegisterEnum(BlockType::SOIL , "soil");
    RegisterEnum(BlockType::LOG  , "log");

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

    // wood type
    RegisterEnum(WoodType::OAK, "oak");

    // log axis
    RegisterEnum(LogAxis::X, "x");
    RegisterEnum(LogAxis::Y, "y");
    RegisterEnum(LogAxis::Z, "z");

    /*Log::Init();

    Application app;

    if (app.open())
        CYBRION_CLIENT_TRACE("Main window is opened ({} x {})", app.getWidth(), app.getHeight());
    else    
        return EXIT_FAILURE;

    app.load();
    app.run();*/
    AABB a({ 0.5, 0.5, 0 }, { 2, 2, 2 });
    AABB b({ 1.0, 0.5, 5 }, { 1, 1, 1 });

    vec3 v = { 0, 1, 5 };
    //v *= 2;
    auto [delta, normal] = AABB::SweptAABB(a, b, v);

    std::cout << delta << '\n';
    std::cout << normal.x << ' ' << normal.y << ' ' << normal.z << '\n';

    return EXIT_SUCCESS;
}
