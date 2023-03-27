#include "client/hud.hpp"
#include "client/application.hpp"
#include "client/graphic/block_renderer.hpp"

namespace cybrion
{
    void HUD::load()
    {
        m_blockMenu = {
            {
                "Nature",
                {
                    &Blocks::GRASS_BLOCK,
                    &Blocks::DIRT,
                    &Blocks::STONE,
                    &Blocks::COBBLESTONE,
                    &Blocks::SAND,
                    &Blocks::OAK_LOG,
                    &Blocks::OAK_LEAF
                }
            },
            {
                "Building",
                {
                    &Blocks::STONE_BRICK,
                    &Blocks::OAK_FENCE.set<"east">(1).set<"west">(1),
                    &Blocks::OAK_FENCE_GATE.set<"face">(BlockHorizontalFace::NORTH)
                }
            }
        };

        m_window = Application::Get().getWindow();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        m_io = &ImGui::GetIO();

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init("#version 430");

        m_crosshairTex.load("hud/crosshair.png");
    }

    void HUD::render(f32 deltaTime)
    {
        auto& app = Application::Get();
        u32 width = app.getWidth();
        u32 height = app.getHeight();

        auto& game = LocalGame::Get();
        auto& camera = game.getCamera();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // render crosshair
        u32 crosshairSize = 32;
        ImGui::SetNextWindowPos(ImVec2(width / 2 - crosshairSize / 2, height / 2 - crosshairSize / 2));
        ImGui::SetNextWindowSize(ImVec2(crosshairSize, crosshairSize));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        ImGui::Begin("Crosshair", nullptr, 
            ImGuiWindowFlags_NoDecoration | 
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoBackground
        );

        ImGui::Image((void*)(intptr_t)m_crosshairTex.getId(), ImVec2(crosshairSize, crosshairSize));

        ImGui::End();
        ImGui::PopStyleVar();

        // render debug info
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Debug", nullptr,
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse
        );

        ImGui::Text("FPS: %.0f", 1.0f/deltaTime);

        vec3 pos = camera.getPos();
        ImGui::Text("Pos: %.2f %.2f %.2f", pos.x, pos.y, pos.z);

        Block* block = game.getPlayer().getTargetBlock();
        if (block)
        {
            vec3 pos = game.getPlayer().getTargetPos();
            ImGui::Text("Target: %.0f %.0f %.0f", pos.x, pos.y, pos.z);
            ImGui::Text("Block: %s", block->getDisplayName().c_str());
            ImGui::Text("%s", block->toString().c_str());
        }

        Block* heldBlock = game.getPlayer().getHeldBlock();
        if (heldBlock)
        {
            ImGui::Text("Holding: %s", heldBlock->getDisplayName().c_str());
        }

        ImGui::End();

        // render toolbox
        ImGui::Begin("Toolbox");
        
        if (ImGui::Button("Reload shader"))
            ShaderManager::Get().reloadShaders();

        static bool _ = false;
        if (ImGui::Checkbox("Wireframe", &_))
            LocalGame::Get().toggleWireframe();

        ImGui::Checkbox("Chunk border", &game.m_showChunkBoder);
        ImGui::Checkbox("Entity border", &game.m_showEntityBorder);

        ImGui::Checkbox("Enable diffuse", &game.m_worldRenderer.m_enableDiffuse);
        ImGui::Checkbox("Enable AO", &game.m_worldRenderer.m_enableAO);

        ImGui::End();

        //renderInventory();
        renderBlockMenu();

        // render everything to window
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void HUD::renderInventory()
    {
        ImGui::SetNextWindowBgAlpha(0.3);
        ImGui::SetNextWindowPos(ImVec2(m_io->DisplaySize.x * 0.5f, m_io->DisplaySize.y - 50), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 1));
        
        ImGui::Begin("Inventory", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

        auto& player = LocalGame::Get().getPlayer();
        i32 currentSlot = player.getHeldSlot();
        i32 leftSlot = std::min(
            std::max(0, currentSlot - Player::DISPLAYED_INVENTORY_SIZE / 2),
            Player::INVENTORY_SIZE - Player::DISPLAYED_INVENTORY_SIZE
        );

        auto& inventory = player.getInventory();

        for (i32 idx = 0; idx < Player::DISPLAYED_INVENTORY_SIZE; ++idx)
        {
            i32 slot = leftSlot + idx;
            Block* block = inventory[slot];

            if (slot == currentSlot)
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.72f, 0.72f, 0.72f, 1));

            ImGui::SetNextWindowBgAlpha(0.2);
            ImGui::BeginChildFrame(idx + 240404, ImVec2(ITEM_FRAME_SIZE, ITEM_FRAME_SIZE), ImGuiWindowFlags_NoScrollbar);

            if (block)
                renderBlock(*block);

            ImGui::EndChildFrame();
            ImGui::SameLine();

            if (slot == currentSlot)
                ImGui::PopStyleColor();
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(2);
        ImGui::End();

        Block* heldBlock = inventory[currentSlot];

        if (heldBlock)
        {
            ImGui::SetNextWindowPos(ImVec2(m_io->DisplaySize.x * 0.5f, m_io->DisplaySize.y - 110), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowBgAlpha(0.4);
            ImGui::Begin("Inventory Header", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
            
            ImGui::Text("%s", heldBlock->getDisplayName().c_str());

            ImGui::End();
        }

        ImGui::PopStyleVar();
    }

    void HUD::renderBlockMenu()
    {
        // render black background
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.7));
        ImGui::Begin("Block Menu Background", NULL,
            ImGuiWindowFlags_NoTitleBar | 
            ImGuiWindowFlags_NoScrollbar | 
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoResize
        );

        ImGui::End();
        ImGui::PopStyleColor();

        // render menu
        ImGui::SetNextWindowBgAlpha(0.3);
        ImGui::SetNextWindowPos(ImVec2(m_io->DisplaySize.x * 0.5f, 50), ImGuiCond_Always, ImVec2(0.5f, 0.0f));

        ImGui::Begin("Block Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

        ImGui::BeginTabBar("Block Menu Tab");

        static string currentTab = std::get<0>(m_blockMenu[0]);
        static i32 selectedSlot = -1;

        for (auto [name, blocks] : m_blockMenu)
            if (ImGui::TabItemButton(name.c_str()))
                currentTab = name;

        for (auto [name, blocks] : m_blockMenu)
        {
            if (name == currentTab)
            {
                i32 ncols = Player::DISPLAYED_INVENTORY_SIZE/2;
                i32 nrows = std::ceil(1.0f * blocks.size() / ncols);

                i32 idx = 0;

                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3);
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 1));

                for (i32 i = 0; i < nrows; ++i)
                {
                    for (i32 j = 0; j < ncols; ++j)
                    {
                        ImGui::SetNextWindowBgAlpha(0.2);
                        ImGui::BeginChildFrame(idx + 140404, ImVec2(ITEM_FRAME_SIZE, ITEM_FRAME_SIZE), ImGuiWindowFlags_NoScrollbar);

                        if (idx < blocks.size())
                            renderBlock(*blocks[idx]);

                        ImGui::EndChildFrame();
                        ImGui::SameLine();

                        idx += 1;
                    }
                    ImGui::NewLine();
                }

                ImGui::PopStyleVar(3);
                ImGui::PopStyleColor(2);
            }
        }

        ImGui::EndTabBar();    
        ImGui::End();
    }

    bool HUD::renderBlock(const Block& block)
    {
        if (m_itemFrameMap.find(block.getId()) == m_itemFrameMap.end())
        {
            m_itemFrameMap[block.getId()] = std::make_shared<GL::Framebuffer>();
            m_itemFrameMap[block.getId()]->alloc(ITEM_FRAME_SIZE, ITEM_FRAME_SIZE);
        }

        auto itemFrame = m_itemFrameMap[block.getId()];

        itemFrame->bind();
        glViewport(0, 0, ITEM_FRAME_SIZE, ITEM_FRAME_SIZE);

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto mesh = LocalGame::Get().getBlockRenderer(block.getId()).getMesh();
        auto shader = ShaderManager::Get().getShader<BasicBlockShader>("basic_block");

        mesh->setRot({ -pi / 6, -pi / 6, 0 });
        mesh->updateModelMat();

        shader.use();
        shader.setUniform<"MVP">(mesh->getModelMat());
        BlockLoader::Get().bindTextureArray();

        mesh->drawTriangles();

        itemFrame->unbind();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0.1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0.2));

        bool flag = ImGui::ImageButton(
            (ImTextureID)itemFrame->getTexture().getId(),
            ImVec2(ITEM_FRAME_SIZE, ITEM_FRAME_SIZE),
            ImVec2(0, 1), ImVec2(1, 0)
        );

        ImGui::PopStyleColor(3);

        return flag;
    }

    HUD::~HUD()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}