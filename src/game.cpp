#include "game.hpp"
#include "world/block/nature/log_block.hpp"
#include "world/block/nature/air_block.hpp"

namespace cybrion
{
    Game* Game::s_game = nullptr;

    Game::Game():
        m_player(nullptr)
    {
        s_game = this;
    }

    void Game::load()
    {
        // register blocks
        m_blockRegistry.load();

        // load block configs
        m_blockLoader.load();

        // precompute some block properties
        m_blockRegistry.computeRotation();

        m_world.loadChunk({ 0, 0, 0 });
    }

    void Game::tick()
    {
        m_world.tick();
        //if (m_player)
            m_player->tick();
    }

    BlockRegistry& Game::getBlockRegistry()
    {
        return m_blockRegistry;
    }

    BlockLoader& Game::getBlockLoader()
    {
        return m_blockLoader;
    }

    World& Game::getWorld()
    {
        return m_world;
    }

    void Game::loadPlayer(Player& player)
    {
        Object playerEntity = m_world.spawnEntity({ 0, 0, 0 });
        player.setEntity(playerEntity);

        m_player = &player;
    }

    Game& Game::Get()
    {
        return *s_game;
    }
}
