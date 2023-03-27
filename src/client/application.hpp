#pragma once

#include "client/key_code.hpp"
#include "client/frame_profiler.hpp"
#include "client/resource/shader_manager.hpp"
#include "client/hud.hpp"
#include "client/local_game.hpp"

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
        bool isRightMouseDown() const;
        bool isLeftMouseDown() const;

        void toggleCursor();
        void enableCursor();
        void disableCursor();

        void playSound(const string& name);

        bool isCursorEnable() const;

        vec2 getDeltaMousePos() const;
        
        ShaderManager& getShaderManager();
        string getResourcePath(const string& path) const;
        string getSavePath(const string& path) const;

        f32 getFPS() const;
        f32 getDeltaTime() const;

        ~Application();

        static Application& Get();

    private:
        static void GlfwResizeCallback(GLFWwindow* window, int width, int height);
        static void GlfwCloseCallback(GLFWwindow* window);
        static void GlfwMouseMovedCallback(GLFWwindow* window, double x, double y);
        static void GlfwKeyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void GlfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        static Application* s_application;

        string m_rootPath;

        FrameProfiler m_frameProfiler;
        HUD m_hud;
        ShaderManager m_shaderManager;
        
        irrklang::ISoundEngine* m_soundEngine;

        // game
        LocalGame* m_game;
        bool m_playingGame;

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