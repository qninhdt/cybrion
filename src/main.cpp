#include "client/application.hpp"

using namespace cybrion;

i32 main(i32 argc, char *args[])
{
    // block type
    RegisterEnum(BlockType::AIR, "air");
    RegisterEnum(BlockType::SOIL, "soil");
    RegisterEnum(BlockType::LOG, "log");
    RegisterEnum(BlockType::SAND, "sand");
    RegisterEnum(BlockType::ROCK, "rock");
    RegisterEnum(BlockType::BRICK, "brick");
    RegisterEnum(BlockType::GLASS, "glass");
    RegisterEnum(BlockType::WATER, "water");
    RegisterEnum(BlockType::LEAF, "leaf");
    RegisterEnum(BlockType::FENCE, "fence");
    RegisterEnum(BlockType::FENCE_GATE, "fence_gate");
    RegisterEnum(BlockType::PLANT, "plant");
    RegisterEnum(BlockType::TABLECLOTH, "tablecloth");
    RegisterEnum(BlockType::CHESS, "chess");
    RegisterEnum(BlockType::WOOL, "wool");
    RegisterEnum(BlockType::CONCRETE, "concrete");
    RegisterEnum(BlockType::MINERAL, "mineral");

    // block color
    RegisterEnum(BlockColor::YELLOW, "yellow");
    RegisterEnum(BlockColor::WHITE, "white");
    RegisterEnum(BlockColor::RED, "red");
    RegisterEnum(BlockColor::PURPLE, "purple");
    RegisterEnum(BlockColor::PINK, "pink");
    RegisterEnum(BlockColor::ORANGE, "orange");
    RegisterEnum(BlockColor::MAGENTA, "magenta");
    RegisterEnum(BlockColor::LIME, "lime");
    RegisterEnum(BlockColor::LIGHT_GRAY, "light_gray");
    RegisterEnum(BlockColor::LIGHT_BLUE, "light_blue");
    RegisterEnum(BlockColor::GREEN, "green");
    RegisterEnum(BlockColor::GRAY, "gray");
    RegisterEnum(BlockColor::CYAN, "cyan");
    RegisterEnum(BlockColor::BROWN, "brown");
    RegisterEnum(BlockColor::BLUE, "blue");
    RegisterEnum(BlockColor::BLACK, "black");

    // dirt type
    RegisterEnum(SoilType::DIRT, "dirt");
    RegisterEnum(SoilType::BASALT, "basalt");
    RegisterEnum(SoilType::CLAY, "clay");
    RegisterEnum(SoilType::GRASS, "grass");

    // rock type
    RegisterEnum(RockType::STONE, "stone");
    RegisterEnum(RockType::GRAVEL, "gravel");
    RegisterEnum(RockType::COBBLESTONE, "cobblestone");

    // chess type
    RegisterEnum(ChessType::KING, "king");
    RegisterEnum(ChessType::QUEEN, "queen");
    RegisterEnum(ChessType::BISHOP, "bishop");
    RegisterEnum(ChessType::ROOK, "rook");
    RegisterEnum(ChessType::KNIGHT, "knight");
    RegisterEnum(ChessType::PAWN, "pawn");

    // chess color
    RegisterEnum(ChessColor::WHITE, "white");
    RegisterEnum(ChessColor::BLACK, "black");

    // brick type
    RegisterEnum(BrickType::STONE, "stone");
    RegisterEnum(BrickType::MOSSY_STONE, "mossy_stone");
    RegisterEnum(BrickType::CLAY, "clay");
    RegisterEnum(BrickType::QUARTZ, "quartz");

    // mineral type
    RegisterEnum(MineralType::COPPER, "copper");
    RegisterEnum(MineralType::IRON, "iron");
    RegisterEnum(MineralType::LAPIS, "lapis");
    RegisterEnum(MineralType::EMERALD, "emerald");
    RegisterEnum(MineralType::GOLD, "gold");
    RegisterEnum(MineralType::DIAMOND, "diamond");

    // block shape
    RegisterEnum(BlockShape::CUBE, "cube");
    RegisterEnum(BlockShape::CUSTOM, "custom");

    // block display
    RegisterEnum(BlockDisplay::OPAQUE, "opaque");
    RegisterEnum(BlockDisplay::LIQUID, "liquid");
    RegisterEnum(BlockDisplay::TRANSPARENT, "transparent");
    RegisterEnum(BlockDisplay::SEMI_OPAQUE, "semi_opaque");
    RegisterEnum(BlockDisplay::SEMI_TRANSPARENT, "semi_transparent");

    // block rotation
    RegisterEnum(BlockRotation::R0, "0");
    RegisterEnum(BlockRotation::R90, "90");
    RegisterEnum(BlockRotation::R180, "180");
    RegisterEnum(BlockRotation::R270, "270");

    // block face
    RegisterEnum(BlockFace::EAST, "east");
    RegisterEnum(BlockFace::TOP, "top");
    RegisterEnum(BlockFace::SOUTH, "south");
    RegisterEnum(BlockFace::WEST, "west");
    RegisterEnum(BlockFace::BOTTOM, "bottom");
    RegisterEnum(BlockFace::NORTH, "north");

    RegisterEnum(BlockHorizontalFace::EAST, "east");
    RegisterEnum(BlockHorizontalFace::SOUTH, "south");
    RegisterEnum(BlockHorizontalFace::WEST, "west");
    RegisterEnum(BlockHorizontalFace::NORTH, "north");

    // wood type
    RegisterEnum(WoodType::ACACIA, "acacia");
    RegisterEnum(WoodType::BIRCH, "birch");
    RegisterEnum(WoodType::DARK_OAK, "dark_oak");
    RegisterEnum(WoodType::JUNGLE, "jungle");
    RegisterEnum(WoodType::MANGROVE, "mangrove");
    RegisterEnum(WoodType::OAK, "oak");
    RegisterEnum(WoodType::SPRUCE, "spruce");

    // plant type
    RegisterEnum(PlantType::GRASS, "grass");
    RegisterEnum(PlantType::FERN, "fern");
    RegisterEnum(PlantType::DANDELION, "dandelion");
    RegisterEnum(PlantType::POPPY, "poppy");
    RegisterEnum(PlantType::BLUE_ORCHID, "blue_orchid");
    RegisterEnum(PlantType::ALLIUM, "allium");
    RegisterEnum(PlantType::AZURE, "azure");
    RegisterEnum(PlantType::RED_TULIP, "red_tulip");
    RegisterEnum(PlantType::WHITE_TULIP, "white_tulip");
    RegisterEnum(PlantType::PINK_TULIP, "pink_tulip");
    RegisterEnum(PlantType::OXEYE_DAISY, "oxeye_daisy");
    RegisterEnum(PlantType::CORNFLOWER, "cornflower");
    RegisterEnum(PlantType::LILY_OF_THE_VALLEY, "lily_of_the_valley");

    // log axis
    RegisterEnum(LogAxis::X, "x");
    RegisterEnum(LogAxis::Y, "y");
    RegisterEnum(LogAxis::Z, "z");

    Log::Init();
    jbt::init();

    string rootPath{args[0]};
    std::replace(rootPath.begin(), rootPath.end(), '\\', '/');
    const size_t last_slash_idx = rootPath.rfind('/');
    rootPath = rootPath.substr(0, last_slash_idx);
      
    Application app(rootPath);

    if (app.open())
        CYBRION_CLIENT_TRACE("Main window is opened ({} x {})", app.getWidth(), app.getHeight());
    else
        return EXIT_FAILURE;

    app.load();

    app.run();
    return EXIT_SUCCESS;
}
