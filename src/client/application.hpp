#pragma once

#include "client/key_code.hpp"
#include "client/frame_profiler.hpp"
#include "client/resource/shader_manager.hpp"
#include "client/debug_screen.hpp"
#include "client_game.hpp"

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

        GLFWwindow* getWindow() const;

        u32 getWidth() const;
        u32 getHeight() const;
        uvec2 getPos() const;
        f32 getAspect() const;
        bool isClosed() const;
        bool isPlayingGame() const;
        bool isKeyPressed(KeyCode key) const;

        void toggleCursor();
        void enableCursor();
        void disableCursor();

        bool isCursorEnable() const;

        vec2 getDeltaMousePos() const;

        

        ShaderManager& getShaderManager();
        string getResourcePath(const string& path) const;

        f32 getFPS() const;
        f32 getDeltaTime() const;

        ~Application();

        static Application& Get();

    private:
        static void GlfwResizeCallback(GLFWwindow* window, int width, int height);
        static void GlfwCloseCallback(GLFWwindow* window);
        static void GlfwMouseMovedCallback(GLFWwindow* window, double x, double y);
        static void GlfwKeyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        static Application* s_application;

        string m_rootPath;

        FrameProfiler m_frameProfiler;
        DebugScreen m_debugScreen;
        ShaderManager m_shaderManager;

        // game
        ClientGame* m_game;
        bool m_playingGame;
        std::thread m_gameThread;

        // window
        GLFWwindow* m_window;
        u32 m_width;
        u32 m_height;
        uvec2 m_pos;
        vec2 m_mousePos;
        vec2 m_lastMousePos;
        string m_title;
        bool m_isClosed;
        bool m_enableCursor;

    };
}