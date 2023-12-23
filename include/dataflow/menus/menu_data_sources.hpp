#ifndef INCLUDE_MENU_DATA_SOURCES_HPP_
#define INCLUDE_MENU_DATA_SOURCES_HPP_



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
// This menu is to add "data sources nodes" to a Study
//-------------------------------------------------------------------
class MenuDataSources : public NodeMenuBase<MenuDataSources>
{
public:

    MenuDataSources() : NodeMenuBase<MenuDataSources>()
    {
        this->properties_.title = "Data Sources";
    }



    void initialize_button_hovered_states()
    {
        this->is_button_hovered_[get_node_type_name<MatrixSourceNode>()] = false;
        this->is_button_hovered_[get_node_type_name<ImageLoaderNode>()] = false;
        this->is_button_hovered_[get_node_type_name<CsvLoaderNode>()] = false;
    }



    // Function used in order to loasd all textures used
    // to draw the "add node" buttons in the menu
    void load_menu_resources()
    {
        // First get the path to the executable's resources sub-folder
        auto executable_parent_path = LazyMatrix::get_absolute_path_of_executable_parent_directory();
        std::string resources_path = executable_parent_path.string() + "/resources/images/";

        // Load the textures used to draw the "add node" buttons
        matrix_source_texture_.loadFromFile(resources_path + std::string("matrix_source.png"));
        image_loader_texture_.loadFromFile(resources_path + std::string("load_image.png"));
        csv_loader_texture_.loadFromFile(resources_path + std::string("csv_file.png"));
    }



    // Function used to draw the Menu window
    void draw_menu()
    {
        if(ImGui::CollapsingHeader(this->properties_.title.c_str()))
        {
            ImGui::Separator();
            this->draw_button_to_add_node<MatrixSourceNode>(matrix_source_texture_);
            ImGui::Text("Matrix Generators");
            ImGui::Separator();
            this->draw_button_to_add_node<ImageLoaderNode>(image_loader_texture_);
            ImGui::Text("Load Image");
            ImGui::Separator();
            this->draw_button_to_add_node<CsvLoaderNode>(csv_loader_texture_);
            ImGui::Text("Load csv file");
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
    sf::Texture matrix_source_texture_;
    sf::Texture image_loader_texture_;
    sf::Texture csv_loader_texture_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_MENU_DATA_SOURCES_HPP_