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
        // load block configs
        m_blockLoader.load();

        // precompute some block properties
        Blocks::Get().computeRotation();

        m_world.loadChunk({ 0, 0, 0 });

        m_player.setEntity(m_world.spawnEntity({ 0, 0, 0 }));
    }

    void Game::tick()
    {
        m_world.tick();
        m_player.tick();
    }

    World& Game::getWorld()
    {
        return m_world;
    }

    Player& Game::getPlayer()
    {
        return m_player;
    }

    Game& Game::Get()
    {
        return *s_game;
    }
}
