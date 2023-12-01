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

        // Draw the image button
        const sf::Texture& texture_to_use = is_on_ ? on_texture_ : off_texture_;
        if (ImGui::ImageButton(reinterpret_cast<void*>(texture_to_use.getNativeHandle()), size_))
        {
            is_on_ = !is_on_;
            if (external_state)
            {
                *external_state = is_on_;
            }
        }

        // Change cursor to hand when hovering over the button
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        // Adjust this offset to reduce the gap between the image and text
        ImGui::SameLine(0,0);

        // Move the cursor up a bit to align the text with the button
        //ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (size_.y / 2 - ImGui::GetTextLineHeight() / 2));        

        // Draw the text as a button
        if (ImGui::Button(text.c_str(), ImVec2(0, size_.y*1.01f)))
        {
            is_on_ = !is_on_;
            if (external_state)
            {
                *external_state = is_on_;
            }
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