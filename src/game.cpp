#include "game.hpp"
#include "world/block/nature/log_block.hpp"
#include "world/block/nature/air_block.hpp"

namespace cybrion
{
    Game *Game::s_game = nullptr;

    Game::Game(const string &worldPath) : m_worldPath(worldPath), m_isPaused(false)
    {
        s_game = this;
    }

    void Game::load()
    {
        if (!std::filesystem::exists(m_worldPath))
        {
            CYBRION_GAME_ERROR("World folder is not found");
            exit(0);
        }

        m_world = World::loadWorld(m_worldPath);

        // load block configs
        m_blockLoader.load();

        // precompute some block properties
        Blocks::Get().computeRotation();
    }

    void Game::tick()
    {
        m_world->tick();
        m_player.tick();
    }

    void Game::stop()
    {
        m_world->save(m_worldPath);
    }

    World &Game::getWorld()
    {
        return *m_world;
    }

    Player &Game::getPlayer()
    {
        return m_player;
    }

    Game &Game::Get()
    {
        return *s_game;
    }

    void Game::pause()
    {
        m_isPaused = true;
    }

    void Game::resume()
    {
        m_isPaused = false;
    }

    bool Game::isPaused()
    {
        return m_isPaused;
    }
}
