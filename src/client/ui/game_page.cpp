#include "client/ui/game_page.hpp"
#include "client/application.hpp"

namespace cybrion::ui
{
    void GamePage::onOpen()
    {
        Application::Get().startGame();
        Application::Get().disableCursor();

        m_blockMenu = {
            {"Nature",
             {
                 &Blocks::GRASS_BLOCK,
                 &Blocks::DIRT,
                 &Blocks::CLAY,
                 &Blocks::STONE,
                 &Blocks::COBBLESTONE,
                 &Blocks::SAND,
                 &Blocks::ACACIA_LOG,
                 &Blocks::BIRCH_LOG,
                 &Blocks::DARK_OAK_LOG,
                 &Blocks::JUNGLE_LOG,
                 &Blocks::MANGROVE_LOG,
                 &Blocks::OAK_LOG,
                 &Blocks::SPRUCE_LOG,
                 &Blocks::ACACIA_LEAF,
                 &Blocks::BIRCH_LEAF,
                 &Blocks::DARK_OAK_LEAF,
                 &Blocks::JUNGLE_LEAF,
                 &Blocks::MANGROVE_LEAF,
                 &Blocks::OAK_LEAF,
                 &Blocks::SPRUCE_LEAF,
             }},
            {"Plant",
             {&Blocks::GRASS,
              &Blocks::FERN,
              &Blocks::DANDELION,
              &Blocks::POPPY,
              &Blocks::BLUE_ORCHID,
              &Blocks::ALLIUM,
              &Blocks::AZURE,
              &Blocks::RED_TULIP,
              &Blocks::WHITE_TULIP,
              &Blocks::PINK_TULIP,
              &Blocks::OXEYE_DAISY,
              &Blocks::CORNFLOWER,
              &Blocks::LILY_OF_THE_VALLEY}},
            {"Building",
             {
                 // brick
                 &Blocks::STONE_BRICK,
                 &Blocks::MOSSY_STONE_BRICK,
                 &Blocks::CLAY_BRICK,
                 &Blocks::QUARTZ_BRICK,
                 // wool
                 &Blocks::YELLOW_WOOL,
                 &Blocks::WHITE_WOOL,
                 &Blocks::RED_WOOL,
                 &Blocks::PURPLE_WOOL,
                 &Blocks::PINK_WOOL,
                 &Blocks::ORANGE_WOOL,
                 &Blocks::MAGENTA_WOOL,
                 &Blocks::LIME_WOOL,
                 &Blocks::LIGHT_GRAY_WOOL,
                 &Blocks::LIGHT_BLUE_WOOL,
                 &Blocks::GREEN_WOOL,
                 &Blocks::GRAY_WOOL,
                 &Blocks::CYAN_WOOL,
                 &Blocks::BROWN_WOOL,
                 &Blocks::BLUE_WOOL,
                 &Blocks::BLACK_WOOL,
                 // concrete
                 &Blocks::YELLOW_CONCRETE,
                 &Blocks::WHITE_CONCRETE,
                 &Blocks::RED_CONCRETE,
                 &Blocks::PURPLE_CONCRETE,
                 &Blocks::PINK_CONCRETE,
                 &Blocks::ORANGE_CONCRETE,
                 &Blocks::MAGENTA_CONCRETE,
                 &Blocks::LIME_CONCRETE,
                 &Blocks::LIGHT_GRAY_CONCRETE,
                 &Blocks::LIGHT_BLUE_CONCRETE,
                 &Blocks::GREEN_CONCRETE,
                 &Blocks::GRAY_CONCRETE,
                 &Blocks::CYAN_CONCRETE,
                 &Blocks::BROWN_CONCRETE,
                 &Blocks::BLUE_CONCRETE,
                 &Blocks::BLACK_CONCRETE,
                 // fence
                 &Blocks::ACACIA_FENCE.set<"east">(1).set<"west">(1),
                 &Blocks::BIRCH_FENCE.set<"east">(1).set<"west">(1),
                 &Blocks::DARK_OAK_FENCE.set<"east">(1).set<"west">(1),
                 &Blocks::JUNGLE_FENCE.set<"east">(1).set<"west">(1),
                 &Blocks::MANGROVE_FENCE.set<"east">(1).set<"west">(1),
                 &Blocks::OAK_FENCE.set<"east">(1).set<"west">(1),
                 &Blocks::SPRUCE_FENCE.set<"east">(1).set<"west">(1),
                 // fence gate
                 &Blocks::ACACIA_FENCE_GATE.set<"face">(BlockHorizontalFace::NORTH),
                 &Blocks::BIRCH_FENCE_GATE.set<"face">(BlockHorizontalFace::NORTH),
                 &Blocks::DARK_OAK_FENCE_GATE.set<"face">(BlockHorizontalFace::NORTH),
                 &Blocks::JUNGLE_FENCE_GATE.set<"face">(BlockHorizontalFace::NORTH),
                 &Blocks::MANGROVE_FENCE_GATE.set<"face">(BlockHorizontalFace::NORTH),
                 &Blocks::OAK_FENCE_GATE.set<"face">(BlockHorizontalFace::NORTH),
                 &Blocks::SPRUCE_FENCE_GATE.set<"face">(BlockHorizontalFace::NORTH),
                 // mineral
                 &Blocks::COPPER_BLOCK,
                 &Blocks::IRON_BLOCK,
                 &Blocks::EMERALD_BLOCK,
                 &Blocks::LAPIS_BLOCK,
                 &Blocks::GOLD_BLOCK,
                 &Blocks::DIAMOND_BLOCK,
             }},
            {"Decoration",
             {
                 &Blocks::TABLECLOTH,
                 // chess
                 &Blocks::WHITE_KING,
                 &Blocks::WHITE_QUEEN,
                 &Blocks::WHITE_BISHOP,
                 &Blocks::WHITE_KNIGHT,
                 &Blocks::WHITE_ROOK,
                 &Blocks::WHITE_PAWN,
                 &Blocks::BLACK_KING,
                 &Blocks::BLACK_QUEEN,
                 &Blocks::BLACK_BISHOP,
                 &Blocks::BLACK_KNIGHT,
                 &Blocks::BLACK_ROOK,
                 &Blocks::BLACK_PAWN,
             }}};
        m_crosshairTex.load("ui/crosshair.png");
    }

    void GamePage::onRender()
    {
        auto &game = LocalGame::Get();
        auto &app = Application::Get();

        if (!game.m_showUI)
            return;

        // render HUD
        // render crosshair
        u32 crosshairSize = 32;
        ImGui::SetNextWindowPos(ImVec2(m_io.DisplaySize.x / 2 - crosshairSize / 2, m_io.DisplaySize.y / 2 - crosshairSize / 2));
        ImGui::SetNextWindowSize(ImVec2(crosshairSize, crosshairSize));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        ImGui::Begin("Crosshair", nullptr,
                     ImGuiWindowFlags_NoDecoration |
                         ImGuiWindowFlags_NoBringToFrontOnFocus |
                         ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoBackground);

        ImGui::Image((void *)(intptr_t)m_crosshairTex.getId(), ImVec2(crosshairSize, crosshairSize));

        ImGui::End();
        ImGui::PopStyleVar();

        if (!game.m_showDebugUI)
        {
            // render debug info
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::Begin("Debug", nullptr,
                         ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoCollapse);

            ImGui::Text("FPS: %.0f", app.getFPS());

            vec3 pos = game.getCamera().getPos();
            ImGui::Text("Pos: %.2f %.2f %.2f", pos.x, pos.y, pos.z);

            Block *block = game.getPlayer().getTargetBlock();
            if (block)
            {
                vec3 pos = game.getPlayer().getTargetPos();
                ImGui::Text("Target: %.0f %.0f %.0f", pos.x, pos.y, pos.z);
                ImGui::Text("Block: %s", block->getDisplayName().c_str());
                ImGui::Text("%s", block->toString().c_str());
            }

            Block *heldBlock = game.getPlayer().getHeldBlock();
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
        }

        if (showBlockMenu)
            BlockMenuWidget();
        else
            InventoryWidget();
    }

    void GamePage::onClose()
    {
        Application::Get().exitGame();
        Application::Get().currentGame = "";
        Application::Get().enableCursor();
    }

    void GamePage::InventoryWidget()
    {
        ImGui::SetNextWindowBgAlpha(0.3);
        ImGui::SetNextWindowPos(ImVec2(m_io.DisplaySize.x * 0.5f, m_io.DisplaySize.y - 50), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 1));

        ImGui::Begin("Inventory", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

        auto &player = LocalGame::Get().getPlayer();
        i32 currentSlot = player.getHeldSlot();
        i32 leftSlot = std::min(
            std::max(0, currentSlot - Player::DISPLAYED_INVENTORY_SIZE / 2),
            Player::INVENTORY_SIZE - Player::DISPLAYED_INVENTORY_SIZE);

        auto &inventory = player.getInventory();

        for (i32 idx = 0; idx < Player::DISPLAYED_INVENTORY_SIZE; ++idx)
        {
            i32 slot = leftSlot + idx;
            Block *block = inventory[slot];

            if (slot == currentSlot)
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.72f, 0.72f, 0.72f, 1));

            ImGui::SetNextWindowBgAlpha(0.2);
            ImGui::BeginChildFrame(idx + 240404, ImVec2(ITEM_FRAME_SIZE, ITEM_FRAME_SIZE), ImGuiWindowFlags_NoScrollbar);

            if (block)
                BlockButton(block);

            ImGui::EndChildFrame();
            ImGui::SameLine();

            if (slot == currentSlot)
                ImGui::PopStyleColor();
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(2);
        ImGui::End();

        Block *heldBlock = inventory[currentSlot];

        if (heldBlock)
        {
            ImGui::SetNextWindowPos(ImVec2(m_io.DisplaySize.x * 0.5f, m_io.DisplaySize.y - 110), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowBgAlpha(0.4);
            ImGui::Begin("Inventory Header", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

            ImGui::Text("%s", heldBlock->getDisplayName().c_str());

            ImGui::End();
        }

        ImGui::PopStyleVar();
    }

    void GamePage::BlockMenuWidget()
    {
        auto &player = LocalGame::Get().getPlayer();
        auto &inventory = player.getInventory();

        // render black background
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.7));
        ImGui::Begin("Block Menu Background", NULL,
                     ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoScrollbar |
                         ImGuiWindowFlags_NoBringToFrontOnFocus |
                         ImGuiWindowFlags_NoResize);

        ImGui::End();
        ImGui::PopStyleColor();

        // render menu
        static string currentTab = std::get<0>(m_blockMenu[0]);
        static i32 currentSlot = 0;

        ImGui::SetNextWindowBgAlpha(0.3);
        ImGui::SetNextWindowPos(ImVec2(m_io.DisplaySize.x * 0.5f, 50), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
        ImGui::Begin("Block Menu Header", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

        for (auto &[name, blocks] : m_blockMenu)
        {
            if (ImGui::Button(name.c_str()))
            {
                currentTab = name;
            }
            ImGui::SameLine();
        }

        ImGui::End();

        ImGui::SetNextWindowBgAlpha(0.3);
        ImGui::SetNextWindowPos(ImVec2(m_io.DisplaySize.x * 0.5f, 100), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
        ImGui::SetNextWindowSizeConstraints(ImVec2(-1, m_io.DisplaySize.y * 0.5), ImVec2(-1, m_io.DisplaySize.y * 0.5), nullptr);
        ImGui::Begin("Block Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 1));

        for (auto [name, blocks] : m_blockMenu)
        {
            if (name == currentTab)
            {
                i32 ncols = 6;
                i32 nrows = std::ceil(1.0f * blocks.size() / ncols);

                i32 idx = 0;

                for (i32 i = 0; i < nrows; ++i)
                {
                    for (i32 j = 0; j < ncols; ++j)
                    {
                        ImGui::SetNextWindowBgAlpha(0.2);
                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                        ImGui::BeginChildFrame(idx + 140404, ImVec2(ITEM_FRAME_SIZE, ITEM_FRAME_SIZE), ImGuiWindowFlags_NoScrollbar);

                        if (idx < blocks.size())
                        {
                            if (BlockButton(blocks[idx]))
                            {
                                inventory[currentSlot] = blocks[idx];
                                currentSlot = (currentSlot + 1) % Player::INVENTORY_SIZE;
                            }

                            if (ImGui::IsItemHovered())
                                ImGui::SetTooltip("%s", blocks[idx]->getDisplayName().c_str());
                        }

                        ImGui::EndChildFrame();
                        ImGui::SameLine();

                        idx += 1;

                        ImGui::PopStyleVar();
                    }
                    ImGui::NewLine();
                }
            }
        }

        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(2);

        ImGui::End();

        i32 ncols = Player::DISPLAYED_INVENTORY_SIZE;
        i32 nrows = Player::INVENTORY_SIZE / ncols;

        ImGui::SetNextWindowBgAlpha(0.3);
        ImGui::SetNextWindowPos(ImVec2(m_io.DisplaySize.x * 0.5f, m_io.DisplaySize.y - 20), ImGuiCond_Always, ImVec2(0.5f, 1));
        ImGui::Begin("Inventory In Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

        i32 idx = 0;

        for (i32 i = 0; i < nrows; ++i)
        {
            for (i32 j = 0; j < ncols; ++j)
            {
                ImGui::SetNextWindowBgAlpha(0.2);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3);

                if (idx == currentSlot)
                    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.72f, 0.72f, 0.72f, 1));

                ImGui::BeginChildFrame(idx + 140404, ImVec2(ITEM_FRAME_SIZE, ITEM_FRAME_SIZE), ImGuiWindowFlags_NoScrollbar);

                if (idx == currentSlot)
                    ImGui::PopStyleColor();

                if (inventory[idx])
                {
                    if (BlockButton(inventory[idx]))
                        currentSlot = idx;

                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("%s", inventory[idx]->getDisplayName().c_str());
                }
                else
                {
                    ImGui::PushID(1000 + idx);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0.1));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0.2));

                    if (ImGui::Button("", ImVec2(ITEM_FRAME_SIZE, ITEM_FRAME_SIZE)))
                        currentSlot = idx;

                    ImGui::PopStyleColor(3);
                    ImGui::PopID();
                }

                ImGui::EndChildFrame();
                ImGui::SameLine();
                ImGui::PopStyleVar(2);

                idx += 1;
            }
            ImGui::NewLine();
        }

        ImGui::End();
    }

    bool GamePage::BlockButton(Block *block)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0.1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0.2));

        if (m_itemFrameMap.find(block->getId()) == m_itemFrameMap.end())
        {
            m_itemFrameMap[block->getId()] = std::make_shared<GL::Framebuffer>();
            m_itemFrameMap[block->getId()]->alloc(ITEM_FRAME_SIZE, ITEM_FRAME_SIZE);
        }

        auto itemFrame = m_itemFrameMap[block->getId()];

        itemFrame->bind();
        glViewport(0, 0, ITEM_FRAME_SIZE, ITEM_FRAME_SIZE);

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto mesh = LocalGame::Get().getBlockRenderer(block->getId()).getMesh();
        auto shader = ShaderManager::Get().getShader<BasicBlockShader>("basic_block");

        mesh->setRot({-pi / 6, -pi / 6, 0});
        mesh->updateModelMat();

        shader.use();
        shader.setUniform<"MVP">(mesh->getModelMat());
        shader.setUniform<"use_light">(u32(block->getShape() == BlockShape::CUBE));
        BlockLoader::Get().bindTextureArray();

        mesh->drawTriangles();

        itemFrame->unbind();

        bool flag = ImGui::ImageButton(
            (ImTextureID)(intptr_t)itemFrame->getTexture().getId(),
            ImVec2(ITEM_FRAME_SIZE, ITEM_FRAME_SIZE),
            ImVec2(0, 1), ImVec2(1, 0));

        ImGui::PopStyleColor(3);

        return flag;
    }
}