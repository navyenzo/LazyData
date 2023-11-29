#ifndef INCLUDE_MENU_PROPERTIES_HPP_
#define INCLUDE_MENU_PROPERTIES_HPP_



//-------------------------------------------------------------------
#include <string>
#include <iostream>

// ImGui
#include "imgui.h"
#include "imgui-SFML.h"

// SFML
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Properties for the Study Menu
//-------------------------------------------------------------------
struct MenuProperties
{
public:

	// Functions used to apply drawing styles
    void push_styles()
    {
        ImGui::PushStyleColor(ImGuiCol_Button, button_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hovered_color);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, backgroung_color);
    }

    void pop_styles()
    {
        ImGui::PopStyleColor(4);
    }

	// Button sizes for the different states
    sf::Vector2f button_size = sf::Vector2f(80,80);
    sf::Vector2f button_hovered_size = sf::Vector2f(80,80);

	// Button colors for the different states
	ImVec4 button_color = ImVec4(0.f, 0.f, 0.f, 0.f);
	ImVec4 button_active_color = ImVec4(0.5f, 0.5f, 0.f, 1.f);
	ImVec4 button_hovered_color = ImVec4(0.5f, 0.5f, 0.f, 1.f);

	// Menu background color
	ImVec4 backgroung_color = ImVec4(0.13, 0.08, 0.46, 1);

    // Menu title
    std::string title = "Menu";
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_MENU_PROPERTIES_HPP_