#include "client/window.hpp"
#include "client/client.hpp"

namespace cybrion
{
    Window::Window(u32 width, u32 height, const string& title) :
        m_width(width),
        m_height(height),
        m_mousePos(0, 0),
        m_title(title),
        m_isClosed(false),
        m_enableCursor(true),
        m_nativeWindow(nullptr),
        m_pos(0, 0)
    {
        m_lastMousePos = m_mousePos;
    }

    bool Window::open()
    {
        // init GLFW
        if (!glfwInit())
        {
            CYBRION_CLIENT_ERROR("Cannot initialize GLFW");
            return false;
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_nativeWindow = glfwCreateWindow((i32)m_width, (i32)m_height, m_title.c_str(), nullptr, nullptr);

        if (!m_nativeWindow)
        {
            CYBRION_CLIENT_ERROR("Cannot create window");
            return false;
        }

        glfwMakeContextCurrent(m_nativeWindow);

        // load OpenGL
        if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            char* name = (char*) glGetString(GL_RENDERER);
            char* version = (char*) glGetString(GL_VERSION);
            CYBRION_CLIENT_INFO("Loaded OpenGL {}", version);
            CYBRION_CLIENT_TRACE("Graphic card: [{}]", name);
        }
        else
        {
            CYBRION_CLIENT_ERROR("Cannot load OpenGL");
            return false;
        }

        glfwSetWindowUserPointer(m_nativeWindow, this);

        glfwSetWindowSizeCallback(m_nativeWindow, GlfwResizeCallback);
        glfwSetWindowCloseCallback(m_nativeWindow, GlfwCloseCallback);
        glfwSetCursorPosCallback(m_nativeWindow, GlfwMouseMovedCallback);
        glfwSetKeyCallback(m_nativeWindow, GlfwKeyPressedCallback);

        glClearColor(1, 1, 1, 1);

        return true;
    }

    void Window::beginLoop()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::endLoop()
    {
        glfwSwapBuffers(m_nativeWindow);
        glfwPollEvents();
    }

    void Window::close()
    {
        m_isClosed = true;
    }

    void Window::closeImmediately()
    {
        glfwTerminate();
    }

    void Window::tick(f32 deltaTime)
    {
        Camera& camera = Client::Get().getCamera();

        bool right    = isKeyPressed(KeyCode::D);
        bool forward  = isKeyPressed(KeyCode::W);
        bool up       = isKeyPressed(KeyCode::SPACE);
        bool left     = !right   && isKeyPressed(KeyCode::A);
        bool backward = !forward && isKeyPressed(KeyCode::S);
        bool down     = !up      && isKeyPressed(KeyCode::LEFT_SHIFT);

        if (right || left || forward || backward || up || down)
        {
            vec3 dir = glm::normalize(
                f32(right   - left    ) * camera.getRight()   +
                f32(up      - down    ) * camera.getUp()      +
                f32(forward - backward) * camera.getForward()
            );

            camera.move(dir * deltaTime * 10.0f);
            camera.updateViewMatrix();
        }
    }

    GLFWwindow* Window::getNativeWindow() const
    {
        return m_nativeWindow;
    }

    void Window::GlfwResizeCallback(GLFWwindow* nativeWindow, int width, int height)
    {
        Window& window = *(Window*)glfwGetWindowUserPointer(nativeWindow);

        window.m_height = height;
        window.m_width = width;

        glViewport(0, 0, width, height);

        window.onResize(width, height);
    }

    void Window::GlfwCloseCallback(GLFWwindow* nativeWindow)
    {
        Window& window = *(Window*)glfwGetWindowUserPointer(nativeWindow);
        window.m_isClosed = true;
    }

    void Window::GlfwMouseMovedCallback(GLFWwindow* nativeWindow, double x, double y)
    {
        Window& window = *(Window*)glfwGetWindowUserPointer(nativeWindow);

        if (window.m_lastMousePos.x == 0 && window.m_lastMousePos.y == 0)
            window.m_lastMousePos = window.m_mousePos = { f32(x), f32(y) };
        else
        {
            window.m_lastMousePos = window.m_mousePos;
            window.m_mousePos = { f32(x), f32(y) };
        }

        window.onMouseMoved(window.m_mousePos - window.m_lastMousePos);
    }

    void Window::GlfwKeyPressedCallback(GLFWwindow* nativeWindow, int key, int scancode, int action, int mods)
    {
        Window& window = *(Window*)glfwGetWindowUserPointer(nativeWindow);

        if (action == GLFW_PRESS)
            window.onKeyPressed((KeyCode)key, false);
        else if (action == GLFW_REPEAT)
            window.onKeyPressed((KeyCode)key, true);
        else
            window.onKeyReleased((KeyCode)key);
    }

    u32 Window::getWidth() const
    {
        return m_width;
    }

    u32 Window::getHeight() const
    {
        return m_height;
    }

    uvec2 Window::getPos() const
    {
        return m_pos;
    }

    f32 Window::getAspect() const
    {
        return 1.0f * m_width / m_height;
    }

    bool Window::isClosed() const
    {
        return m_isClosed;
    }

    bool Window::isKeyPressed(KeyCode key) const
    {
        return glfwGetKey(m_nativeWindow, (i32)key) == GLFW_PRESS;
    }

    void Window::toggleCursor()
    {
        if (m_enableCursor)
            disableCursor();
        else
            enableCursor();
    }

    void Window::enableCursor()
    {
        m_enableCursor = true;
        glfwSetInputMode(m_nativeWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Window::disableCursor()
    {
        m_enableCursor = false;
        glfwSetInputMode(m_nativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    bool Window::isCursorEnable() const
    {
        return m_enableCursor;
    }

    vec2 Window::getDeltaMousePos() const
    {
        return m_mousePos - m_lastMousePos;
    }

    void Window::onKeyPressed(KeyCode key, bool isRepeated)
    {
        switch (key)
        {

        // toggle cursor
        case KeyCode::F1:
            toggleCursor();
            break;

        // close window
        case KeyCode::ESCAPE:
            close();
            break;

        // reload shaders
        case KeyCode::R:
            Client::Get().getShaderManager().reloadShaders();
            break;
        }
    }

    void Window::onKeyReleased(KeyCode key)
    {
    }

    void Window::onMouseMoved(const vec2& delta)
    {
        if (!m_enableCursor)
        {
            Camera& camera = Client::Get().getCamera();

            camera.rotate(vec3(- delta.y, - delta.x, 0) * Client::Get().getDeltaTime() * 1.0f);
            camera.updateViewMatrix();
        }
    }

    void Window::onResize(u32 width, u32 height)
    {
        Camera& camera = Client::Get().getCamera();
        camera.setAspect(getAspect());
        camera.updateProjectionMatrix();
    }

    Window::~Window()
    {
        if (!m_isClosed)
            closeImmediately();

        CYBRION_CLIENT_TRACE("Main window is closed. Goodbye!");
    }
}