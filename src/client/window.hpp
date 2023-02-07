#pragma once

#include "client/key_code.hpp"

namespace cybrion
{
    class Window
    {
    public:
        Window(u32 width, u32 height, const string& title);

        void beginLoop();
        void endLoop();

        bool open();
        void close();
        void closeImmediately();

        GLFWwindow* getNativeWindow() const;

        u32 getWidth() const;
        u32 getHeight() const;
        uvec2 getPos() const;
        f32 getAspect() const;
        bool isClosed() const;

        bool isKeyPressed(KeyCode key) const;

        void toggleCursor();

        bool isEnableCursor() const;

        vec2 getDeltaMousePos() const;

        void onKeyPressed(KeyCode key, bool isRepeated);
        void onKeyReleased(KeyCode key);
        void onMouseMoved(const vec2& delta);

        ~Window();

    private:
        static void GlfwResizeCallback(GLFWwindow* nativeWindow, int width, int height);
        static void GlfwCloseCallback(GLFWwindow* nativeWindow);
        static void GlfwMouseMovedCallback(GLFWwindow* nativeWindow, double x, double y);
        static void GlfwKeyPressedCallback(GLFWwindow* nativeWindow, int key, int scancode, int action, int mods);

        GLFWwindow* m_nativeWindow;
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