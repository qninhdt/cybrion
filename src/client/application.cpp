#include "client/application.hpp"

#include "client/GL/mesh.hpp"

namespace cybrion
{
    Application* Application::s_application = nullptr;

    Application::Application() :
        m_width(1600),
        m_height(1200),
        m_mousePos(0, 0),
        m_lastMousePos(0, 0),
        m_title("Cybrion v1.0"),
        m_isClosed(false),
        m_enableCursor(true),
        m_window(nullptr),
        m_pos(0, 0),
        m_game(nullptr),
        m_rootPath(CYBRION_ROOT_PATH)
    {
        s_application = this;
    }

    bool Application::open()
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

        m_window = glfwCreateWindow((i32)m_width, (i32)m_height, m_title.c_str(), nullptr, nullptr);

        if (!m_window)
        {
            CYBRION_CLIENT_ERROR("Cannot create window");
            return false;
        }

        glfwMakeContextCurrent(m_window);

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

        glfwSetWindowUserPointer(m_window, this);

        glfwSetWindowSizeCallback(m_window, GlfwResizeCallback);
        glfwSetWindowCloseCallback(m_window, GlfwCloseCallback);
        glfwSetCursorPosCallback(m_window, GlfwMouseMovedCallback);
        glfwSetKeyCallback(m_window, GlfwKeyPressedCallback);

        glClearColor(1, 1, 1, 1);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        return true;
    }

    void Application::load()
    {
        // generate global Indexed Buffer Object
        GL::Mesh::GenerateGlobalIBO();

        // debug screen
        m_debugScreen.load();

        CYBRION_CLIENT_TRACE("Start loading resources ({})", getResourcePath(""));
        m_shaderManager.loadShaders();
    }

    void Application::run()
    {
        startGame();

        // main loop
        f32 deltaTime = 0;
        while (!isClosed())
        {
            // input
            // --------------------------------------------
            glfwPollEvents();

            // tick
            // --------------------------------------------
            m_frameProfiler.tick();
            deltaTime = m_frameProfiler.getDeltaTime();

            if (m_game)
                m_game->tick();

            // render
            // --------------------------------------------
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (m_game)
                m_game->render(deltaTime);

            m_debugScreen.render(deltaTime);

            // update render
            glfwSwapBuffers(m_window);
        }
    }

    void Application::close()
    {
        m_isClosed = true;
    }

    void Application::closeImmediately()
    {
        glfwTerminate();
    }

    void Application::startGame()
    {
        m_game = new ClientGame();
        m_game->load();
    }

    /*void Application::tick(f32 deltaTime)
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
    }*/

    GLFWwindow* Application::getWindow() const
    {
        return m_window;
    }

    ShaderManager& Application::getShaderManager()
    {
        return m_shaderManager;
    }

    string Application::getResourcePath(const string& path) const
    {
        return m_rootPath + "resources/" + path;
    }

    f32 Application::getFPS() const
    {
        return m_frameProfiler.getFPS();
    }

    f32 Application::getDeltaTime() const
    {
        return m_frameProfiler.getDeltaTime();
    }

    Application& Application::Get()
    {
        return *s_application;
    }

    void Application::GlfwResizeCallback(GLFWwindow* window, int width, int height)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);

        app.m_height = height;
        app.m_width = width;

        glViewport(0, 0, width, height);

        app.onResize(width, height);
    }

    void Application::GlfwCloseCallback(GLFWwindow* window)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);
        app.m_isClosed = true;
    }

    void Application::GlfwMouseMovedCallback(GLFWwindow* window, double x, double y)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);

        if (app.m_lastMousePos.x == 0 && app.m_lastMousePos.y == 0)
            app.m_lastMousePos = app.m_mousePos = { f32(x), f32(y) };
        else
        {
            app.m_lastMousePos = app.m_mousePos;
            app.m_mousePos = { f32(x), f32(y) };
        }

        app.onMouseMoved(app.m_mousePos - app.m_lastMousePos);
    }

    void Application::GlfwKeyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);

        if (action == GLFW_PRESS)
            app.onKeyPressed((KeyCode)key, false);
        else if (action == GLFW_REPEAT)
            app.onKeyPressed((KeyCode)key, true);
        else
            app.onKeyReleased((KeyCode)key);
    }

    u32 Application::getWidth() const
    {
        return m_width;
    }

    u32 Application::getHeight() const
    {
        return m_height;
    }

    uvec2 Application::getPos() const
    {
        return m_pos;
    }

    f32 Application::getAspect() const
    {
        return 1.0f * m_width / m_height;
    }

    bool Application::isClosed() const
    {
        return m_isClosed;
    }

    bool Application::isKeyPressed(KeyCode key) const
    {
        return glfwGetKey(m_window, (i32)key) == GLFW_PRESS;
    }

    void Application::toggleCursor()
    {
        if (m_enableCursor)
            disableCursor();
        else
            enableCursor();
    }

    void Application::enableCursor()
    {
        m_enableCursor = true;
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Application::disableCursor()
    {
        m_enableCursor = false;
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    bool Application::isCursorEnable() const
    {
        return m_enableCursor;
    }

    vec2 Application::getDeltaMousePos() const
    {
        return m_mousePos - m_lastMousePos;
    }

    void Application::onKeyPressed(KeyCode key, bool isRepeated)
    {
        if (!isRepeated)
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
                m_shaderManager.reloadShaders();
                break;

                // toggle wireframe
            case KeyCode::F2:
                ClientGame::Get().toggleWireframe();
                break;
            }
        }
    }

    void Application::onKeyReleased(KeyCode key)
    {
    }

    void Application::onMouseMoved(const vec2& delta)
    {
       /* if (!m_enableCursor)
        {
            Camera& camera = Client::Get().getCamera();

            camera.rotate(vec3(- delta.y, - delta.x, 0) * Client::Get().getDeltaTime() * 1.0f);
            auto r = camera.getRotation();

            if (r.x > pi / 2 - 0.001f && r.x < pi * 3 / 2 + 0.001f)
            {
                if (r.x - pi / 2 - 0.001f < pi * 3 / 2 + 0.001f - r.x)
                    r.x = pi / 2 - 0.001f;
                else
                    r.x = pi * 3 / 2 + 0.001f;
            }

            camera.setRotation(r);
            camera.updateViewMatrix();
        }*/
    }

    void Application::onResize(u32 width, u32 height)
    {
        /*Camera& camera = Client::Get().getCamera();
        camera.setAspect(getAspect());
        camera.updateProjectionMatrix();*/
    }

    Application::~Application()
    {
        if (!m_isClosed)
            closeImmediately();

        CYBRION_CLIENT_TRACE("Main window is closed. Goodbye!");
    }
}