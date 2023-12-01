#ifndef INCLUDE_TOGGLE_BUTTON_HPP_
#define INCLUDE_TOGGLE_BUTTON_HPP_



//-------------------------------------------------------------------
#include <SFML/Graphics/Texture.hpp>
#include "imgui.h"
#include <utils/unique_id.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyApp
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief A toggle button class for ImGui using SFML textures.
 *
 * This class creates a toggle button in ImGui using provided SFML textures for the on and off states.
 * The button changes appearance based on its current state and visually responds to user interactions.
 */
//-------------------------------------------------------------------
class ToggleButton
{
public:

    /**
     * @brief Construct a new ToggleButton object
     * 
     * @param on_texture_path Path to the texture to display when the button is 'on'.
     * @param off_texture_path Path to the texture to display when the button is 'off'.
     * @param size Size of the button.
     */
    ToggleButton(const std::string& on_texture_path,
                 const std::string& off_texture_path,
                 const ImVec2& size)
    {
        on_texture_.loadFromFile(on_texture_path);
        off_texture_.loadFromFile(off_texture_path);
        size_ = size;
    }



    /**
     * @brief Draws the toggle button with the linked image and text.
     * 
     * This function creates a unified interactive area that includes both the image
     * and the accompanying text. The entire area is clickable and changes the state
     * of the button.
     * 
     * @param external_state A pointer to an external boolean variable to be updated by the button state.
     * @param text The text to display next to the button.
     * @return true if the button is in the 'on' state, false otherwise.
     */
    bool draw(bool* external_state, const std::string& text)
    {
        ImGui::PushID(id_);

        // Calculate the size of the text
        ImVec2 text_size = ImGui::CalcTextSize(text.c_str());

        // Scale down the size for the image to make it smaller than the button
        ImVec2 scaled_image_size = ImVec2(size_.x * 0.9f, size_.y * 0.9f); // Example: 90% of the original size

        // Calculate the total button size using the original image size, not the scaled size
        ImVec2 total_button_size = ImVec2(size_.x + text_size.x, size_.y);

        // Calculate the horizontal offset to center the scaled image within the translucent button
        float image_offset_x = (total_button_size.x - scaled_image_size.x - text_size.x) * 0.5f;
        // Calculate the vertical offset to center the image within the button height
        float image_offset_y = (total_button_size.y - scaled_image_size.y) * 0.5f;

        // Store the initial cursor position
        ImVec2 start_pos = ImGui::GetCursorPos();

        // Set the position for the scaled image
        ImGui::SetCursorPos(ImVec2(start_pos.x + image_offset_x, start_pos.y + image_offset_y));

        const sf::Texture& texture_to_use = is_on_ ? on_texture_ : off_texture_;
        ImGui::Image(reinterpret_cast<void*>(texture_to_use.getNativeHandle()), scaled_image_size);

        // Draw the text next to the scaled image
        ImGui::SameLine();
        // Ensure the text is vertically aligned with the image
        float text_vertical_offset = (scaled_image_size.y - text_size.y) * 0.5f;
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + text_vertical_offset);
        ImGui::Text("%s", text.c_str());

        // Reset the cursor position to the start of the button area
        ImGui::SetCursorPos(start_pos);

        // Set the style for a translucent button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // Fully transparent background
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.1)); // Slightly visible on hover
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.2)); // More visible when active
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f); // Rounded corners with a radius of 10.0f

        // Draw the invisible button over the area of the scaled image and text
        if (ImGui::Button("##toggle", total_button_size))
        {   
            is_on_ = !is_on_;
            if (external_state)
            {
                *external_state = is_on_;
            }
        }

        // Reset the style to default
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

        // Change cursor to hand when hovering over the button
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        ImGui::PopID();

        return is_on_;
    }



private:

    int id_ = UniqueID::generate_uuid_hash();
    sf::Texture on_texture_;
    sf::Texture off_texture_;
    ImVec2 size_;
    bool is_on_ = true;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyApp
//-------------------------------------------------------------------



#endif // INCLUDE_TOGGLE_BUTTON_HPP_