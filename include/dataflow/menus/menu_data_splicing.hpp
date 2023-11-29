#ifndef INCLUDE_MENU_DATA_SPLICING_HPP_
#define INCLUDE_MENU_DATA_SPLICING_HPP_



//-------------------------------------------------------------------
#include "menu_base.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This menu is to add "data splicing nodes" to a Study
//-------------------------------------------------------------------
class MenuDataSplicing : public NodeMenuBase<MenuDataSplicing>
{
public:

    MenuDataSplicing() : NodeMenuBase<MenuDataSplicing>()
    {
        this->properties_.title = "Data Splicing";
    }



    void initialize_button_hovered_states()
    {
        this->is_button_hovered_[get_node_type_name<ROINode>()] = false;
        this->is_button_hovered_[get_node_type_name<SelectorNode>()] = false;
    }



    // Function used in order to loasd all textures used
    // to draw the "add node" buttons in the menu
    void load_menu_resources()
    {
        // First get the path to the executable's resources sub-folder
        auto executable_parent_path = LazyMatrix::get_absolute_path_of_executable_parent_directory();
        std::string resources_path = executable_parent_path.string() + "/resources/images/";

        // Load the textures used to draw the "add node" buttons
        roi_texture_.loadFromFile(resources_path + std::string("roi.png"));
        selector_texture_.loadFromFile(resources_path + std::string("selector.png"));
    }



    // Function used to draw the Menu window
    void draw_menu()
    {
        if(ImGui::CollapsingHeader(this->properties_.title.c_str()))
        {
            ImGui::Separator();
            this->draw_button_to_add_node<ROINode>(roi_texture_);
            ImGui::Separator();
            this->draw_button_to_add_node<SelectorNode>(selector_texture_);
            ImGui::Separator();
        }

        if(ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            ImGui::BeginTooltip();
            ImGui::Text("%s", this->properties_.title.c_str());
            ImGui::EndTooltip();
        }
    }



private: // Private variables

    // All the button textures
    sf::Texture roi_texture_;
    sf::Texture selector_texture_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_MENU_DATA_SPLICING_HPP_