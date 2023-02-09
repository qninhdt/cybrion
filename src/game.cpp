#include "game.hpp"
#include "world/block/nature/dirt_block.hpp"
#include "world/block/nature/air_block.hpp"

namespace cybrion
{
    Game* Game::s_game = nullptr;

    Game::Game()
    {
        s_game = this;
    }

    void Game::load()
    {
        // register blocks
        m_blockRegistry.init();

        // load block configs
        m_blockLoader.load();

        DirtBlock& dirt = (DirtBlock&) m_blockRegistry.getBlock(BlockType::DIRT);
        std::cout << dirt.getDisplayName() << '\n';
        dirt = dirt.set<"worm">(true);
        std::cout << dirt.getDisplayName() << '\n';
        dirt = dirt.set<"type">(DirtType::CLAY);
        std::cout << dirt.getDisplayName() << '\n';
    }

    GameBlockRegistry& Game::getBlockRegistry()
    {
        return m_blockRegistry;
    }

    Game& Game::Get()
    {
        return *s_game;
    }
}
