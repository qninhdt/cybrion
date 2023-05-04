#include "client/ui/controls.hpp"

namespace cybrion::ui
{
    bool HoverableImageButton(const GL::Texture &img1, const GL::Texture &img2, i32 width, i32 height, bool &state)
    {
        ImTextureID texId = (ImTextureID)(intptr_t)(state ? img2 : img1).getId();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

        bool clicked = ImGui::ImageButton(texId, ImVec2(width, height));
        state = ImGui::IsItemHovered();

        ImGui::PopStyleColor(3);

        return clicked;
    }
}