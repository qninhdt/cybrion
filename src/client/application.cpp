#include "client/application.hpp"
#include "core/pool.hpp"
#include "client/GL/mesh.hpp"

#include "client/ui/home_page.hpp"
#include "client/ui/game_page.hpp"
#include "client/ui/world_list_page.hpp"

#include "client/ui/controls.hpp"

namespace cybrion
{
    Application *Application::s_application = nullptr;

    SDL_HitTestResult HitTestCallback(SDL_Window *Window, const SDL_Point *Area, void *Data)
    {
        int Width, Height;
        SDL_GetWindowSize(Window, &Width, &Height);
        f32 MOUSE_GRAB_PADDING = 8;

        if (Area->y < MOUSE_GRAB_PADDING)
        {
            if (Area->x < MOUSE_GRAB_PADDING)
            {
                return SDL_HITTEST_RESIZE_TOPLEFT;
            }
            else if (Area->x > Width - MOUSE_GRAB_PADDING)
            {
                return SDL_HITTEST_RESIZE_TOPRIGHT;
            }
            else
            {
                return SDL_HITTEST_RESIZE_TOP;
            }
        }
        else if (Area->y > Height - MOUSE_GRAB_PADDING)
        {
            if (Area->x < MOUSE_GRAB_PADDING)
            {
                return SDL_HITTEST_RESIZE_BOTTOMLEFT;
            }
            else if (Area->x > Width - MOUSE_GRAB_PADDING)
            {
                return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
            }
            else
            {
                return SDL_HITTEST_RESIZE_BOTTOM;
            }
        }
        else if (Area->x < MOUSE_GRAB_PADDING)
        {
            return SDL_HITTEST_RESIZE_LEFT;
        }
        else if (Area->x > Width - MOUSE_GRAB_PADDING)
        {
            return SDL_HITTEST_RESIZE_RIGHT;
        }
        else if (Area->y < 24 && Area->x < Width - 24 * 5)
        {
            return SDL_HITTEST_DRAGGABLE;
        }

        return SDL_HITTEST_NORMAL;
    }

    Application::Application(const string &rootPath) : m_width(1200),
                                                       m_height(600),
                                                       m_mousePos(0, 0),
                                                       m_lastMousePos(0, 0),
                                                       m_title("Cybrion v1.0"),
                                                       m_isClosed(false),
                                                       m_enableCursor(true),
                                                       m_window(nullptr),
                                                       m_context(nullptr),
                                                       m_pos(0, 0),
                                                       m_fps(0),
                                                       m_game(nullptr),
                                                       m_soundEngine(nullptr),
                                                       m_rootPath(rootPath),
                                                       m_playingGame(false),
                                                       m_currentPage("")
    {
        s_application = this;
    }

    bool Application::open()
    {
        // init SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            CYBRION_CLIENT_ERROR("Cannot initialize SDL");
            return false;
        }

        SDL_GL_LoadLibrary(nullptr);

        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        m_window = SDL_CreateWindow(
            m_title.c_str(),
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            m_width, m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS);

        if (!m_window)
        {
            CYBRION_CLIENT_ERROR("Cannot create window");
            return false;
        }

        m_context = SDL_GL_CreateContext(m_window);

        if (!m_context)
        {
            CYBRION_CLIENT_ERROR("Cannot create OpenGL context");
            return false;
        }

        SDL_SetWindowResizable(m_window, SDL_TRUE);
        SDL_SetWindowHitTest(m_window, HitTestCallback, 0);

        // load OpenGL
        if (gladLoadGLLoader(SDL_GL_GetProcAddress))
        {
            char *name = (char *)glGetString(GL_RENDERER);
            char *version = (char *)glGetString(GL_VERSION);
            CYBRION_CLIENT_INFO("Loaded OpenGL {}", version);
            CYBRION_CLIENT_TRACE("Graphic card: [{}]", name);
        }
        else
        {
            CYBRION_CLIENT_ERROR("Cannot load OpenGL");
            return false;
        }

        glClearColor(1, 1, 1, 1);
        glEnable(GL_DEPTH_TEST);

        m_soundEngine = irrklang::createIrrKlangDevice();
        m_soundEngine->setSoundVolume(1.0f);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::GetIO().Fonts->AddFontDefault();
        ImGui::GetIO().IniFilename = nullptr;
        m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(getResourcePath("font.ttf").c_str(), 14);

        ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
        ImGui_ImplOpenGL3_Init("#version 430");

        std::filesystem::create_directory(getSavePath(""));

        m_pages["home"] = std::make_shared<ui::HomePage>();
        m_pages["game"] = std::make_shared<ui::GamePage>();
        m_pages["world_list"] = std::make_shared<ui::WorldListPage>();

        return true;
    }

    void Application::load()
    {
        // generate global Indexed Buffer Object
        GL::Mesh::GenerateGlobalIBO();

        CYBRION_CLIENT_TRACE("Start loading resources ({})", getResourcePath(""));
        m_shaderManager.loadShaders();

        m_iconTexture.load("ui/icon.png");

        m_maximizeTexture.load("ui/maximize_button.png");
        m_restoreTexture.load("ui/restore_button.png");
        m_minimizeTexture.load("ui/minimize_button.png");
        m_closeTexture.load("ui/close_button.png");
    }

    void Application::run()
    {
        Stopwatch fpsStopwatch;
        Stopwatch stopwatch;

        // main loop
        //f32 deltaTime = 0;
        fpsStopwatch.reset();
        stopwatch.reset();

        SDL_Event event;
        bool mouseFlag = true;

        goToPage("home");

        //currentGame = "2";
        //goToPage("game");

        while (!isClosed())
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_WINDOWEVENT)
                {
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        resizeCallback(event.window.data1, event.window.data2);
                    }
                    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                    {
                        close();
                    }
                }
                if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
                {
                    keyPressedCallback(event.key.keysym.scancode, (SDL_EventType)event.type);

                    if (event.key.keysym.scancode == SDL_SCANCODE_F11 && event.type == SDL_KEYDOWN)
                    {
                        bool isFullscreen = (SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN;
                        if (!isFullscreen) {
                            SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        }
                        else {
                            SDL_SetWindowFullscreen(m_window, 0);
                        }
                    }
                }
                if (event.type == SDL_MOUSEWHEEL)
                {
                    scrollCallback(event.wheel.preciseX, event.wheel.preciseY);
                }
                if (isPlayingGame() && !LocalGame::Get().isPaused())
                {
                    auto &input = LocalGame::Get().getPlayer().getInput();

                    if (event.type == SDL_MOUSEBUTTONDOWN)
                    {
                        if (!m_enableCursor)
                        {
                            if (event.button.button == SDL_BUTTON_RIGHT)
                                input.rightClick = true;

                            if (event.button.button == SDL_BUTTON_LEFT)
                                input.leftClick = true;
                        }
                    }
                    if (event.type == SDL_MOUSEBUTTONUP)
                    {
                        if (!m_enableCursor)
                        {
                            if (event.button.button == SDL_BUTTON_RIGHT)
                                input.rightClick = false;

                            if (event.button.button == SDL_BUTTON_LEFT)
                                input.leftClick = false;
                        }
                    }
                }

                ImGui_ImplSDL2_ProcessEvent(&event);
            }

            if (isPlayingGame() && !m_enableCursor && !LocalGame::Get().isPaused())
            {
                SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);

                ivec2 delta = m_mousePos - ivec2(m_width / 2, m_height / 2);

                if (delta != ivec2(0, 0))
                {
                    m_game->onMouseMoved(delta);
                    SDL_WarpMouseInWindow(m_window, m_width / 2, m_height / 2);
                }
            }

            // input
            // --------------------------------------------

            Camera &camera = LocalGame::Get().getCamera();

            bool right = isKeyPressed(SDL_SCANCODE_D);
            bool forward = isKeyPressed(SDL_SCANCODE_W);
            bool up = isKeyPressed(SDL_SCANCODE_SPACE);
            bool left = !right && isKeyPressed(SDL_SCANCODE_A);
            bool backward = !forward && isKeyPressed(SDL_SCANCODE_S);
            bool down = !up && isKeyPressed(SDL_SCANCODE_LSHIFT);

            if (isPlayingGame() && !LocalGame::Get().isPaused())
            {
                auto &input = LocalGame::Get().getPlayer().getInput();

                if (right || left || forward || backward || up || down)
                {
                    vec3 dir = glm::normalize(
                        f32(right - left) * camera.getRight() +
                        f32(up - down) * camera.getUp() +
                        f32(forward - backward) * camera.getForward());

                    input.isMoving = true;
                    input.moveDir = dir;
                }
                else
                {
                    input.isMoving = false;
                }

                input.ctrl = isKeyPressed(SDL_SCANCODE_LCTRL);
                input.shift = isKeyPressed(SDL_SCANCODE_LSHIFT);
            }

            // tick
            // --------------------------------------------
            m_frameProfiler.tick();

            if (fpsStopwatch.getDeltaTime() >= 250000)
            {
                //deltaTime = m_frameProfiler.getDeltaTime();
                fpsStopwatch.reduceDeltaTime(250000);
                m_fps = m_frameProfiler.getFPS();
            }

            if (isPlayingGame())
            {
                while (stopwatch.getDeltaTime() >= GAME_TICK)
                {
                    m_game->tick();
                    stopwatch.reduceDeltaTime(GAME_TICK);
                }
            }

            // render
            // --------------------------------------------

            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (!isCursorEnable())
                ImGui::SetMouseCursor(ImGuiMouseCursor_None);

            if (isPlayingGame())
            {
                f32 lerpFactor = 1.0f * stopwatch.getDeltaTime() / GAME_TICK;
                m_game->render(lerpFactor);
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            ImGui::PushFont(m_font);
            renderTitleBar();
            m_pages[m_currentPage]->onRender();
            ImGui::PopFont();

            ImGui::Render();
            glViewport(0, 0, m_width, m_height);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // update render
            SDL_GL_SwapWindow(m_window);
        }
    }

    void Application::close()
    {
        m_isClosed = true;
    }

    void Application::closeImmediately()
    {
        SDL_GL_DeleteContext(m_context);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    void Application::startGame()
    {
        m_game = new LocalGame(getSavePath(currentGame));
        m_game->load();

        m_playingGame = true;
    }

    void Application::exitGame()
    {
        CYBRION_CLIENT_TRACE("Saving world");
        m_game->stop();
        CYBRION_CLIENT_TRACE("Finish saving world");

        delete m_game;
        m_game = nullptr;

        m_playingGame = false;
    }

    ref<ui::Page> Application::getCurrentPage()
    {
        return m_pages[m_currentPage];
    }

    void Application::goToPage(const string &name)
    {
        if (m_currentPage != "")
            m_pages[m_currentPage]->onClose();
        m_currentPage = name;
        m_pages[m_currentPage]->onOpen();
    }

    SDL_Window *Application::getWindow() const
    {
        return m_window;
    }

    void *Application::getContext() const
    {
        return m_context;
    }

    ShaderManager &Application::getShaderManager()
    {
        return m_shaderManager;
    }

    string Application::getResourcePath(const string &path) const
    {
        return m_rootPath + "/resources/" + path;
    }

    string Application::getSavePath(const string &path) const
    {
        return m_rootPath + "/saves/" + path;
    }

    f32 Application::getFPS() const
    {
        return m_fps;
    }

    f32 Application::getDeltaTime() const
    {
        return m_frameProfiler.getDeltaTime();
    }

    Application &Application::Get()
    {
        return *s_application;
    }

    void Application::resizeCallback(int width, int height)
    {
        m_height = height;
        m_width = width;

        glViewport(0, 0, width, height);

        if (isPlayingGame())
            m_game->onWindowResized(width, height);
    }

    void Application::closeCallback()
    {
        m_isClosed = true;
    }

    void Application::keyPressedCallback(SDL_Scancode key, SDL_EventType type)
    {
        if (isPlayingGame())
        {
            if (type == SDL_KEYDOWN)
                m_game->onKeyPressed(key, false);
            else
                m_game->onKeyReleased(key);
        }
    }

    void Application::scrollCallback(f64 xoffset, f64 yoffset)
    {
        if (isPlayingGame())
            m_game->onMouseScrolled(yoffset);
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

    bool Application::isKeyPressed(SDL_Scancode key) const
    {
        return SDL_GetKeyboardState(nullptr)[key];
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
        SDL_ShowCursor(SDL_ENABLE);
    }

    void Application::disableCursor()
    {
        m_enableCursor = false;
        SDL_ShowCursor(SDL_DISABLE);
    }

    void Application::playSound(const string &name)
    {
        string soundPath = getResourcePath("sounds/" + name + ".ogg");
        m_soundEngine->play2D(soundPath.c_str());
    }

    bool Application::isCursorEnable() const
    {
        return m_enableCursor;
    }

    void Application::renderTitleBar()
    {
        bool isMaximized = (SDL_GetWindowFlags(m_window) & SDL_WINDOW_MAXIMIZED) != 0;
        bool isFullscreen = (SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN;

        if (isFullscreen) return;

        const i32 TITLE_BAR_SIZE = 24;
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, TITLE_BAR_SIZE));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.2));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0));
        ImGui::Begin("Title Bar", NULL, ImGuiWindowFlags_NoDecoration);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0.2));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0.4));

        ImGui::SameLine(TITLE_BAR_SIZE / 2);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        ImGui::BeginChildFrame(12345, ImVec2(150, 0));
        ImGui::PopStyleColor(1);
        ImGui::PopStyleVar(1);

        ImGui::Image((ImTextureID)(intptr_t)m_iconTexture.getId(), ImVec2(16, 16));

        ImGui::SameLine();
        ImGui::Text("Cybrion");

        ImGui::EndChildFrame();

        ImGui::SameLine(125);
        ImGui::PushID(90);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.1f));
        ImGui::Button(std::to_string((i32)getFPS()).c_str(), ImVec2(0, TITLE_BAR_SIZE));
        ImGui::PopStyleColor(1);
        ImGui::PopID();

        ImGui::SameLine(ImGui::GetWindowWidth() - TITLE_BAR_SIZE * 2);
        if (ImGui::ImageButton((ImTextureID)(intptr_t)m_closeTexture.getId(), ImVec2(TITLE_BAR_SIZE, TITLE_BAR_SIZE)))
        {
            close();
        }

        ImGui::SameLine(ImGui::GetWindowWidth() - TITLE_BAR_SIZE * 3 - 12);
        if (isMaximized)
        {
            if (ImGui::ImageButton((ImTextureID)(intptr_t)m_restoreTexture.getId(), ImVec2(TITLE_BAR_SIZE, TITLE_BAR_SIZE)))
            {
                SDL_RestoreWindow(m_window);
            }
        }
        else
        {
            if (ImGui::ImageButton((ImTextureID)(intptr_t)m_maximizeTexture.getId(), ImVec2(TITLE_BAR_SIZE, TITLE_BAR_SIZE)))
            {
                SDL_MaximizeWindow(m_window);
            }
        }

        ImGui::SameLine(ImGui::GetWindowWidth() - TITLE_BAR_SIZE * 4 - 12 * 2);
        if (ImGui::ImageButton((ImTextureID)(intptr_t)m_minimizeTexture.getId(), ImVec2(TITLE_BAR_SIZE, TITLE_BAR_SIZE)))
        {
            SDL_MinimizeWindow(m_window);
        }

        ImGui::PopStyleVar(4);
        ImGui::PopStyleColor(4);
        ImGui::End();
    }

    Application::~Application()
    {
        CYBRION_CLIENT_TRACE("Wait until game stopped");

        GetPool().pause();
        GetPool().wait_for_tasks();

        if (m_game)
        {
            m_pages[m_currentPage]->onClose();
        }

        if (!m_isClosed)
            closeImmediately();

        CYBRION_CLIENT_TRACE("Main window is closed. Goodbye!");
    }
}