#pragma once

#include "client/key_code.hpp"
#include "client/frame_profiler.hpp"
#include "client/resource/shader_manager.hpp"
#include "client/local_game.hpp"
#include "client/ui/page.hpp"

namespace cybrion
{
    class Application
    {
    public:
        Application();

        bool open();
        void load();
        void run();
        void close();
        void closeImmediately();

        void startGame();
        void exitGame();

        ref<ui::Page> getCurrentPage();
        void goToPage(const string &name);

        SDL_Window *getWindow() const;
        void *getContext() const;

        u32 getWidth() const;
        u32 getHeight() const;
        uvec2 getPos() const;
        f32 getAspect() const;
        bool isClosed() const;
        bool isPlayingGame() const;
        bool isKeyPressed(SDL_Scancode key) const;

        void toggleCursor();
        void enableCursor();
        void disableCursor();

        void playSound(const string &name);

        bool isCursorEnable() const;

        ShaderManager &getShaderManager();
        string getResourcePath(const string &path) const;
        string getSavePath(const string &path) const;

        f32 getFPS() const;
        f32 getDeltaTime() const;

        ~Application();

        static Application &Get();

        string currentGame = "";

    private:
        void resizeCallback(i32 width, i32 height);
        void closeCallback();
        void keyPressedCallback(SDL_Scancode key, SDL_EventType type);
        void scrollCallback(f64 xoffset, f64 yoffset);

        static Application *s_application;

        string m_rootPath;

        FrameProfiler m_frameProfiler;
        ShaderManager m_shaderManager;

        irrklang::ISoundEngine *m_soundEngine;

        ImFont *m_font;

        // game
        LocalGame *m_game;
        bool m_playingGame;

        // window
        SDL_Window *m_window;
        SDL_GLContext m_context;
        u32 m_width;
        u32 m_height;
        uvec2 m_pos;
        ivec2 m_mousePos;
        ivec2 m_lastMousePos;
        string m_title;
        bool m_isClosed;
        bool m_enableCursor;

        // ui
        umap<string, ref<ui::Page>> m_pages;
        string m_currentPage;
    };
}