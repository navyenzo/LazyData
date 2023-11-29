#ifndef INCLUDE_MENU_BASE_HPP_
#define INCLUDE_MENU_BASE_HPP_



//-------------------------------------------------------------------
#include <unordered_map>

#include "menu_properties.hpp"
#include "../study.hpp"

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
// Base Study Menu that we can derive from to define specific
// Study Menus
//-------------------------------------------------------------------
template<typename MenuType>

class NodeMenuBase
{
public:

    // Function used in order to loasd all textures used
    // to draw the "add node" buttons in the menu
    void load_resources()
    {
        underlying().load_menu_resources();
    }



    // Setters/Getters
    void set_study(Study* study)
    {
        linked_study_ = study;
    }

    Study* get_study()
    {
        return linked_study_;
    }

    void set_properties(const MenuProperties& properties)
    {
        properties_ = properties;
    }

    const MenuProperties& get_properties()const
    {
        return properties_;
    }



    // Function used to check whether any node is being dragged
    bool is_any_node_being_dragged_and_dropped()const
    {
        for(const auto& button : is_button_active_)
        {
            if(button.second)
                return true;
        }

        return false;
    }



    // Function used to draw the Menu window
    void draw()
    {
        properties_.push_styles();
        underlying().draw_menu();
        properties_.pop_styles();
    }



    // This function is called by the main "draw" function
    // to draw each "add node" button
    template<typename NodeType>
    void draw_button_to_add_node(const sf::Texture& button_texture)
    {
        // This string is used in the "Drag-and-Drop" mechanism
        std::string node_type_name = get_node_type_name<NodeType>();

        // Determine the size of the button depending
        // on whether the button is being hovered
        sf::Vector2f button_size = properties_.button_size;
        if(is_button_hovered_[node_type_name])
            button_size = properties_.button_hovered_size;
        
        // Get the available region so we can draw the
        // button in the middle
        sf::Vector2f available_region = ImGui::GetContentRegionAvail();

        // Place the cursor in the middle before drawing
        ImGui::SetCursorPos(sf::Vector2f(available_region.x/2.0 - button_size.x/2.0, ImGui::GetCursorPosY()));

        if(ImGui::ImageButton(button_texture, button_size))
        {
            if(linked_study_)
            {
                linked_study_->add_node<NodeType>();
            }
        }

        // Enable the ability to drag and drop menu buttons into
        // a study in order to add a node to the study
        if(ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload(node_type_name.c_str(), node_type_name.c_str(), node_type_name.size(), ImGuiCond_Once);
            ImGui::Image(button_texture, properties_.button_size);
            ImGui::EndDragDropSource();
        }

        // If the button is hovered, we show the button's tooltip
        if(ImGui::IsItemHovered())
        {
            is_button_hovered_[node_type_name] = true;
            show_button_tooltip((std::string("Add a ") + node_type_name).c_str());
        }
        else
            is_button_hovered_[node_type_name] = false;

        // We use the button's active state in order to know
        // that the user is dragging a node so we prevent
        // the node menu from collapsing
        if(ImGui::IsItemActive())
            is_button_active_[node_type_name] = true;
        else
            is_button_active_[node_type_name] = false;
    }



    // This function draws the "Add Study" button
    // which is displayed/drawn whenever all the
    // studies have been closed by the user
    bool draw_add_study()
    {
        properties_.push_styles();

        // Get the available region so we can draw the
        // button in the middle
        sf::Vector2f available_region = ImGui::GetContentRegionAvail();

        ImGui::SetCursorPos(sf::Vector2f(available_region.x/2.0 - 64.0, available_region.y/2.0 - 64.0));

        bool was_button_pressed = false;

        if(ImGui::ImageButton(add_study_texture_, sf::Vector2f(128,128)))
        {
            was_button_pressed = true;
        }
        
        if (ImGui::IsItemHovered())
            show_button_tooltip("Add a New Study");

        properties_.pop_styles();

        return was_button_pressed;
    }



private: // Private functions

    // The constructor is defined as private and the
    // derived class is defined as friend in order to
    //  prevent the derived menu from specifying the
    // wrong template argument
    NodeMenuBase<MenuType>()
    {
        underlying().initialize_button_hovered_states();

        // We need to load the "Add Study" button texture
        auto executable_parent_path = LazyMatrix::get_absolute_path_of_executable_parent_directory();
        std::string resources_path = executable_parent_path.string() + "/resources/images/";
        add_study_texture_.loadFromFile(resources_path + std::string("add_study.png"));
    }

    friend MenuType;

    // Function used to facilitate invoking the
    // derived functions
    MenuType& underlying()
    {
        return static_cast<MenuType&>(*this);
    }

    const MenuType& underlying()const
    {
        return static_cast<const MenuType&>(*this);
    }

    // This function is used to display the tooltip
    // of the button that is currently hovered
    void show_button_tooltip(const std::string& tooltip_message)
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::BeginTooltip();
        ImGui::Text("%s", tooltip_message.c_str());
        ImGui::EndTooltip();
    }



protected: // Protected variables

    int id_ = LazyApp::UniqueID::generate_uuid_hash();

    // This pointer is the current Study that this
    // Study menu will add nodes to, it is dynamically
    // set by the application as the user switches
    // between studies
    Study* linked_study_ = nullptr;

    // These maps hold the "hovered" and "active" states
    // of each node button
    std::unordered_map<std::string, bool> is_button_hovered_;
    std::unordered_map<std::string, bool> is_button_active_;

    // Boolean used to check whether a node button is being
    // currently dragged and dropped
    bool is_button_being_dragged_and_dropped_ = false;

    // Menu properties (sizes and colors and styles)
    MenuProperties properties_;

    // The "Add Study" texture
    sf::Texture add_study_texture_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_MENU_BASE_HPP_