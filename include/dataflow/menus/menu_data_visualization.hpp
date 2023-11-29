#ifndef INCLUDE_MENU_DATA_VISUALIZATION_HPP_
#define INCLUDE_MENU_DATA_VISUALIZATION_HPP_



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
// This menu is to add "data visualization nodes" to a Study
//-------------------------------------------------------------------
class MenuDataVisualization : public NodeMenuBase<MenuDataVisualization>
{
public:

    MenuDataVisualization() : NodeMenuBase<MenuDataVisualization>()
    {
        this->properties_.title = "Data\nVisualization";
    }



    void initialize_button_hovered_states()
    {
        this->is_button_hovered_[get_node_type_name<TableNode>()] = false;
        this->is_button_hovered_[get_node_type_name<PlotNode>()] = false;
        this->is_button_hovered_[get_node_type_name<HeatMapNode>()] = false;
    }



    // Function used in order to loasd all textures used
    // to draw the "add node" buttons in the menu
    void load_menu_resources()
    {
        // First get the path to the executable's resources sub-folder
        auto executable_parent_path = LazyMatrix::get_absolute_path_of_executable_parent_directory();
        std::string resources_path = executable_parent_path.string() + "/resources/images/";

        // Load the textures used to draw the "add node" buttons
        table_texture_.loadFromFile(resources_path + std::string("table.png"));
        plot_texture_.loadFromFile(resources_path + std::string("plot.png"));
        heat_map_texture_.loadFromFile(resources_path + std::string("heat_map.png"));
    }



    // Function used to draw the Menu window
    void draw_menu()
    {
        if(ImGui::CollapsingHeader(this->properties_.title.c_str()))
        {
            ImGui::Separator();
            this->draw_button_to_add_node<TableNode>(table_texture_);
            ImGui::Separator();
            this->draw_button_to_add_node<PlotNode>(plot_texture_);
            ImGui::Separator();
            this->draw_button_to_add_node<HeatMapNode>(heat_map_texture_);
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
    sf::Texture table_texture_;
    sf::Texture plot_texture_;
    sf::Texture heat_map_texture_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_MENU_DATA_VISUALIZATION_HPP_