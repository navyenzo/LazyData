#ifndef INCLUDE_IMGUI_HELPERS_HPP_
#define INCLUDE_IMGUI_HELPERS_HPP_



//-------------------------------------------------------------------
#include "imgui.h"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyApp
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief A collection of helper functions for ImGui.
 *
 * ImGuiHelpers provides additional functionalities that can be used in ImGui interfaces.
 * It includes methods like creating a vertical separator, which can be useful in organizing
 * and structuring the ImGui layout more effectively.
 */
//-------------------------------------------------------------------
class ImGuiHelpers
{
public:

    /**
     * @brief Creates a visible vertical separator in ImGui with dynamic height.
     * 
     * This static method draws a vertical line as a separator based on the height of the current window or node in ImGui.
     */
    static void vertical_separator()
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return;

        // Calculate the height based on the height of the ImGui window
        float separatorHeight = window->Size.y - window->WindowPadding.y * 2;

        ImVec2 startPos = ImGui::GetCursorScreenPos() + ImVec2(ImGui::GetStyle().ItemSpacing.x/2, 0);
        ImVec2 endPos = startPos + ImVec2(0.0f, separatorHeight);

        // Draw the line
        window->DrawList->AddLine(startPos, endPos, ImGui::GetColorU32(ImGuiCol_Separator));

        // Adjust cursor position
        ImGui::Dummy(ImVec2(ImGui::GetStyle().ItemSpacing.x, 0.0f));
    }



    /**
     * @brief Creates a dynamic vertical separator in ImGui.
     * 
     * This function draws a vertical separator line that dynamically adjusts to the height of the ImGui window.
     * It should be used in layouts where the height of the content can vary.
     * 
     * @param startPos The starting position (top-left corner) of the separator.
     */
    static void vertical_separator_dynamic(const ImVec2& startPos)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return;

        float currentWindowHeight = ImGui::GetWindowHeight();
        float separatorHeight = currentWindowHeight - (startPos.y - window->Pos.y);

        ImGuiContext& g = *GImGui;
        ImVec2 p = startPos;
        ImGui::SetCursorScreenPos(ImVec2(p.x, p.y));
        ImGui::Dummy(ImVec2(1.0f, separatorHeight));
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyApp
//-------------------------------------------------------------------



#endif // INCLUDE_IMGUI_HELPERS_HPP_
