#pragma once

#include "client/resource/block_loader.hpp"
#include "world/world.hpp"
#include "player.hpp"

namespace cybrion
{
    class Game
    {
    public:
        Game(const string &worldPath);

        void load();
        void tick();
        void stop();

        void pause();
        void resume();
        bool isPaused();

        World &getWorld();
        Player &getPlayer();

        virtual void onChunkLoaded(const ref<Chunk> &chunk) = 0;
        virtual void onChunkUnloaded(const ref<Chunk> &chunk) = 0;
        virtual void onEntitySpawned(const ref<Entity> &entity) = 0;
        virtual void onChunkChanged(const ref<Chunk> &chunk) = 0;
        virtual void onBreakBlock(const BlockModifyResult &result) = 0;
        virtual void onPlaceBlock(const BlockModifyResult &result) = 0;
        virtual void onPlaySound(const string &name) = 0;

        static Game &Get();

    protected:
        static Game *s_game;
        static bool s_isFirstGame;

        BlockLoader m_blockLoader;
        ref<World> m_world;
        Player m_player;
        string m_worldPath;
        bool m_isPaused;
    };
}
