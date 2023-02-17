#include "client/application.hpp"
#include "core/pool.hpp"
#include "client/GL/mesh.hpp"

namespace cybrion
{
    Application* Application::s_application = nullptr;

    void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length,
        const GLchar* msg, const void* data)
    {
        string _source;
        string _type;
        string _severity;

        switch (source) {
        case GL_DEBUG_SOURCE_API:
            _source = "API";
            break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = "WINDOW SYSTEM";
            break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = "SHADER COMPILER";
            break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = "THIRD PARTY";
            break;

        case GL_DEBUG_SOURCE_APPLICATION:
            _source = "APPLICATION";
            break;

        case GL_DEBUG_SOURCE_OTHER:
            _source = "UNKNOWN";
            break;

        default:
            _source = "UNKNOWN";
            break;
        }

        switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            _type = "ERROR";
            break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = "DEPRECATED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = "UDEFINED BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_PORTABILITY:
            _type = "PORTABILITY";
            break;

        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = "PERFORMANCE";
            break;

        case GL_DEBUG_TYPE_OTHER:
            _type = "OTHER";
            break;

        case GL_DEBUG_TYPE_MARKER:
            _type = "MARKER";
            break;

        default:
            _type = "UNKNOWN";
            break;
        }

        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            _severity = "HIGH";
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            _severity = "MEDIUM";
            break;

        case GL_DEBUG_SEVERITY_LOW:
            _severity = "LOW";
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            _severity = "NOTIFICATION";
            break;

        default:
            _severity = "UNKNOWN";
            break;
        }

        printf("%d: %s of %s severity, raised from %s: %s\n",
            id, _type.c_str(), _severity.c_str(), _source.c_str(), msg);
    }

    Application::Application() :
        m_width(1500),
        m_height(800),
        m_mousePos(0, 0),
        m_lastMousePos(0, 0),
        m_title("Cybrion v1.0"),
        m_isClosed(false),
        m_enableCursor(true),
        m_window(nullptr),
        m_pos(0, 0),
        m_game(nullptr),
        m_rootPath(CYBRION_ROOT_PATH),
        m_playingGame(false)
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

        //glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        
        glDebugMessageCallback(GLDebugMessageCallback, NULL);

        return true;
    }

    void Application::load()
    {
        // generate global Indexed Buffer Object
        GL::Mesh::GenerateGlobalIBO();

        // load debug screen
        m_hud.load();

        CYBRION_CLIENT_TRACE("Start loading resources ({})", getResourcePath(""));
        m_shaderManager.loadShaders();
    }

    void Application::run()
    {
        Stopwatch stopwatch;
        Stopwatch fpsStopwatch;

        startGame();

        auto& input = LocalGame::Get().getPlayer().getInput();

        // main loop
        f32 deltaTime = 0;
        stopwatch.reset();
        fpsStopwatch.reset();

        while (!isClosed())
        {
            // input
            // --------------------------------------------
            glfwPollEvents();

            Camera& camera = LocalGame::Get().getCamera();

            if (!m_enableCursor)
            {
                if (isRightMouseDown())
                {
                    input.rightClick = true;
                }

                if (isLeftMouseDown())
                {
                    input.leftClick = true;
                }
            }

            bool right = isKeyPressed(KeyCode::D);
            bool forward = isKeyPressed(KeyCode::W);
            bool up = isKeyPressed(KeyCode::SPACE);
            bool left = !right && isKeyPressed(KeyCode::A);
            bool backward = !forward && isKeyPressed(KeyCode::S);
            bool down = !up && isKeyPressed(KeyCode::LEFT_SHIFT);

            if (right || left || forward || backward || up || down)
            {
                vec3 dir = glm::normalize(
                    f32(right - left) * camera.getRight() +
                    f32(up - down) * camera.getUp() +
                    f32(forward - backward) * camera.getForward()
                );

                input.isMoving = true;
                input.moveDir = dir;
            }
            else
            {
                input.isMoving = false;
            }

            // game tick
            // --------------------------------------------
            if (m_game)
            {
                while (stopwatch.getDeltaTime() >= GAME_TICK)
                {
                    m_game->tick();
                    stopwatch.reduceDeltaTime(GAME_TICK);
                }
            }

            // tick
            // --------------------------------------------
            m_frameProfiler.tick();

            if (fpsStopwatch.getDeltaTime() >= 250000)
            {
                deltaTime = m_frameProfiler.getDeltaTime();
                fpsStopwatch.reduceDeltaTime(250000);
            }

            // render
            // --------------------------------------------
            f32 lerpFactor = 1.0f * stopwatch.getDeltaTime() / GAME_TICK;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (m_game)
                m_game->render(lerpFactor);

            m_hud.render(deltaTime);

          
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
        m_game = new LocalGame();
        m_game->load();
        std::cout << Blocks::COBBLESTONE.getDisplayName() << '\n';

        m_playingGame = true;
    }

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

        if (app.isPlayingGame())
            app.m_game->onWindowResized(width, height);
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

        if (app.isPlayingGame())
            app.m_game->onMouseMoved(app.m_mousePos - app.m_lastMousePos);
    }

    void Application::GlfwKeyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Application& app = *(Application*)glfwGetWindowUserPointer(window);

        if (action == GLFW_PRESS)
        {
            if (app.isPlayingGame())
                app.m_game->onKeyPressed((KeyCode)key, false);
        }
        else if (action == GLFW_REPEAT)
        {
            if (app.isPlayingGame())
                app.m_game->onKeyPressed((KeyCode)key, true);
        }
        else
        {
            if (app.isPlayingGame())
                app.m_game->onKeyReleased((KeyCode)key);
        }
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

    bool Application::isPlayingGame() const
    {
        return m_playingGame;
    }

    bool Application::isKeyPressed(KeyCode key) const
    {
        return glfwGetKey(m_window, (i32)key) == GLFW_PRESS;
    }

    bool Application::isRightMouseDown() const
    {
        return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    }

    bool Application::isLeftMouseDown() const
    {
        return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
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

    Application::~Application()
    {
        CYBRION_CLIENT_TRACE("Wait until game stopped");

        GetPool().pause();
        GetPool().wait_for_tasks();

        if (!m_isClosed)
            closeImmediately();

        CYBRION_CLIENT_TRACE("Main window is closed. Goodbye!");
    }
}