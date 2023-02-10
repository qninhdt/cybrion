#include "game.hpp"
#include "world/block/nature/log_block.hpp"
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

        // precompute some block properties
        m_blockRegistry.precompute();
    }

    GameBlockRegistry& Game::getBlockRegistry()
    {
        return m_blockRegistry;
    }

    BlockLoader& Game::getBlockLoader()
    {
        return m_blockLoader;
    }

    Game& Game::Get()
    {
        return *s_game;
    }
}
