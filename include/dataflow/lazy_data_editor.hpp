#ifndef INCLUDE_LAZY_DATA_EDITOR_HPP_
#define INCLUDE_LAZY_DATA_EDITOR_HPP_



//-------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <map>

#include <app/base_imgui_app.hpp>
#include <app/font_loader.hpp>
#include <app/animated_drawer.hpp>

#include "data_flow.hpp"

//#include "node_menus_window.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// The Lazy Analysis Data-Flow Node Editor
//-------------------------------------------------------------------
//
// -- This class defines the main User Interface to our Lazy Analysis
//    application
//
//    -- It is made of the following components:
//
//       1.  A "Main Menu Bar"
//       2.  A "Node Menu"
//       3.  A "Study Tab Bar"
//
//    -- Studies are the equivalent of an Analysis Worksheet
//
//       -- The application allows the user to have multiple
//          Studies open at the same time
//
//       -- A Study allows the user to construct a "Computational Graph"
//          by dragging-and-dropping Nodes and wiring them together
//
//       -- In each Study a user can add "Nodes", and the nodes
//          can be divided into the following two categories:
//
//          -- "DATA SOURCE" Nodes
//             -- These nodes are used to either "load" or "generate"
//                matrix-like data
//
//          -- "ANALYSIS/COMPUTATIONAL" Nodes
//             -- These nodes are used to "manipulate" input data or to
//                "carry out computations" on input data
//
//          -- Nodes can have 0, 1, or multiple "Input" Pins and/or
//             "Output" Pins
//             -- Pins can be wired together by the user using the
//                mouse
//             -- An "Output" Pin can be connected to multiple "Input" Pins
//             -- An "Input" Pin can be connected to ONLY ONE "Output" Pin
//
//          -- WHen the user connects two pins (input/output) together
//             the Node whose Input Pin got connected starts carrying
//             out its data computations/manipulations
//
//             -- NOTE:  The current implementation of the Nodes carry
//                       out the computation/manipulation in the same
//                       thread as the User Interface
//
//             -- TODO:  The next step is to allow Nodes to carry out
//                       computations in parallel and to also add an
//                       "enable"/"disable" live computations so that
//                       a user can "pause" computations while building
//                       the computation node-graph
//
//-------------------------------------------------------------------
class LazyDataEditor : public LazyApp::BaseNodeEditorApp<LazyDataEditor>
{
public:

	// Constructor
	explicit LazyDataEditor(const LazyApp::AppProperties& app_properties = LazyApp::AppProperties())
	: LazyApp::BaseNodeEditorApp<LazyDataEditor>(app_properties)
	{
	}



	// Function used to initialize the app
	bool init();

	// Function called during the first iteration of the main_loop
	void first_loop_iteration();

    /**
     * @brief Checks and processes window events.
     * 
     * @param event The event that needs to be processed
     */
    void handle_events(const sf::Event& event);

	// Function called by the app once every loop
	void loop_function(const sf::Time& delta_time);

	// Function used to quit the application
	void kill();



    // Function used to create an empty study to
    // the environment
    void add_study();

    // Function used to draw/handle the Main Menu
    void draw_main_menu();

    // Draw the Studies
    void draw_studies();

    // Function used to draw the Node Menus
    void draw_node_menus();

    // Function used to draw the "Expanded Node Contents"
    void draw_expanded_node_contents_window();
    


private:

    // This deque holds all the open studies
    // -- It uses an std::deque instead of an std::vector
    //    because this way it prevents a study to be copy
    //    constructed which is not allowed
    //    -- The copy construction would happen with
    //       std::vector when it grows and re-allocates
    //       more memory
    std::deque<DataFlow::Study> studies_;

    // The node menus used to add nodes to the
    // study that is currently selected
    DataFlow::MenuDataSources menu_data_sources_;
    DataFlow::MenuDataMatrixOperations menu_data_matrix_operations_;
    DataFlow::MenuDataSplicing menu_data_splicing_;
    DataFlow::MenuDataAugmenting menu_data_augmenting_;
    DataFlow::MenuDataVisualization menu_data_visualization_;

    // Expandable animated drawer holding
    // the Node Menus
    LazyApp::AnimatedDrawer node_menu_drawer_;
    float expand_collapse_button_width_ = 40;

    // Expandable animated drawer that contains
    // the "In-Focus Node's Contents"
    LazyApp::AnimatedDrawer expanded_node_drawer_;


    sf::Vector2f main_menu_bar_size_ = sf::Vector2f(0,0);

    sf::Vector2f minimum_window_size_ = sf::Vector2f(1080, 608);

    int docking_id_ = LazyApp::UniqueID::generate_uuid_hash();
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to initialize the app
//-------------------------------------------------------------------
inline bool LazyDataEditor::init()
{
    // Load fonts used throughout the application    
    // TODO: Fonts still not working correctly, right
    //       now i can only load one font, will need
    //       to fix this
    font_loader_.load_font_from_resource_font_folder("proggy_vector_regular", "proggyfonts/ProggyVector/ProggyVector Regular.ttf", 18);

    node_menu_drawer_.set_min_width(0);
    node_menu_drawer_.set_max_width(180);
    node_menu_drawer_.set_opening_speed(700);
    node_menu_drawer_.set_closing_speed(700);
    node_menu_drawer_.start_opening_drawer();

    expanded_node_drawer_.set_min_width(0);
    expanded_node_drawer_.set_max_width(450);
    expanded_node_drawer_.set_opening_speed(1000);
    expanded_node_drawer_.set_closing_speed(1000);
    expanded_node_drawer_.start_closing_drawer();

    return true;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function called during the first iteration of the main_loop
//-------------------------------------------------------------------
inline void LazyDataEditor::first_loop_iteration()
{
    menu_data_sources_.load_resources();
    menu_data_matrix_operations_.load_resources();
    menu_data_splicing_.load_resources();
    menu_data_augmenting_.load_resources();
    menu_data_visualization_.load_resources();
    add_study();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void LazyDataEditor::handle_events(const sf::Event& event)
{
    return;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function called by the app once every loop
//-------------------------------------------------------------------
inline void LazyDataEditor::loop_function(const sf::Time& delta_time)
{
    ImGui::SFML::Update(*this->window_, delta_time);

    // Draw the main menu bar
    draw_main_menu();

    // Draw the Nodes Menus
    draw_node_menus();

    // Draw the expandable Node Contents Window
    draw_expanded_node_contents_window();

    // Draw the studies
    draw_studies();

    // Clear the current window with the background color
    this->window_->clear(this->app_properties_.background_color);
    
    // This function actually draws all the ImGui
    // related things
    ImGui::SFML::Render(*this->window_);

    // This function draws the SFML window
    this->window_->display();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to quit the application
//-------------------------------------------------------------------
inline void LazyDataEditor::kill()
{
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to create an empty study to the environment
//-------------------------------------------------------------------
inline void LazyDataEditor::add_study()
{
    // First we construct the study in-place and
    // then we set it as the active study for
    // the Node menu so that nodes are actually
    // added to the newly created study

    studies_.emplace_back();

    menu_data_sources_.set_study(&studies_.back());
    menu_data_matrix_operations_.set_study(&studies_.back());
    menu_data_splicing_.set_study(&studies_.back());
    menu_data_augmenting_.set_study(&studies_.back());
    menu_data_visualization_.set_study(&studies_.back());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to draw/handle the Main Menu
//-------------------------------------------------------------------
inline void LazyDataEditor::draw_main_menu()
{
    if (ImGui::BeginMainMenuBar())
    {
        main_menu_bar_size_ = ImGui::GetWindowSize();

        if (ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("New Study", "Ctrl+N"))
                add_study();

            if(ImGui::MenuItem("Exit", "ALT+F4"))
                this->should_app_quit_ = true;

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if(ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if(ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if(ImGui::MenuItem("Cut", "CTRL+X")) {}
            if(ImGui::MenuItem("Copy", "CTRL+C")) {}
            if(ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Draw the Studies
//-------------------------------------------------------------------
inline void LazyDataEditor::draw_studies()
{
    ImGuiIO& io = ImGui::GetIO();

    auto study_window_position = sf::Vector2f(node_menu_drawer_.get_current_width() + expand_collapse_button_width_, main_menu_bar_size_.y);
    auto study_window_size = sf::Vector2f(io.DisplaySize.x - node_menu_drawer_.get_current_width() - expand_collapse_button_width_ - expand_collapse_button_width_ - expanded_node_drawer_.get_current_width(), io.DisplaySize.y - main_menu_bar_size_.y);

    ImGui::SetNextWindowPos(study_window_position);
    ImGui::SetNextWindowSize(study_window_size);

    if(ImGui::Begin("Studies", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
    {
            if(studies_.empty())
        {
            // In this case, there are no open studies
            // and therefore we show the user the "Add Study"
            // button
            if(menu_data_sources_.draw_add_study())
                add_study();
        }
        else if(ImGui::BeginTabBar("Studies", ImGuiTabBarFlags_Reorderable))
        {
            // In this case we do have Studies and
            // therefore we draw them here
            // -- The studies are drawn as individual Tabs
            //    inside a Tab Bar
            
            ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 10);
            
            for(auto iter = studies_.begin(); iter != studies_.end(); ++iter)
            {
                if(!iter->draw())
                {
                    if(menu_data_sources_.get_study() == &(*iter))
                        menu_data_sources_.set_study(nullptr);

                    if(menu_data_matrix_operations_.get_study() == &(*iter))
                        menu_data_matrix_operations_.set_study(nullptr);

                    if(menu_data_splicing_.get_study() == &(*iter))
                        menu_data_splicing_.set_study(nullptr);

                    if(menu_data_augmenting_.get_study() == &(*iter))
                        menu_data_augmenting_.set_study(nullptr);

                    if(menu_data_visualization_.get_study() == &(*iter))
                        menu_data_visualization_.set_study(nullptr);

                    studies_.erase(iter--);
                }
                else if(iter->get_is_study_active())
                {
                    menu_data_sources_.set_study(&(*iter));
                    menu_data_matrix_operations_.set_study(&(*iter));
                    menu_data_splicing_.set_study(&(*iter));
                    menu_data_augmenting_.set_study(&(*iter));
                    menu_data_visualization_.set_study(&(*iter));
                }
            }

            ImGui::PopStyleVar(1);
        
            ImGui::EndTabBar();
        }
    }
    
    ImGui::End();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to draw the Node Menus
//-------------------------------------------------------------------
inline void LazyDataEditor::draw_node_menus()
{
    ImGuiIO& io = ImGui::GetIO();

    if(node_menu_drawer_.is_drawer_partially_open())
    {
        ImGui::SetNextWindowPos(sf::Vector2f(0,main_menu_bar_size_.y));
        ImGui::SetNextWindowSize(sf::Vector2f(node_menu_drawer_.get_current_width(), io.DisplaySize.y - main_menu_bar_size_.y));

        if(ImGui::Begin("Nodes Menus", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
        {
            menu_data_sources_.draw();
            menu_data_matrix_operations_.draw();
            menu_data_splicing_.draw();
            menu_data_augmenting_.draw();
            menu_data_visualization_.draw();
        }
        
        ImGui::End();
    }

    // Draw the expand/collapse button

    ImGui::SetNextWindowPos(sf::Vector2f(node_menu_drawer_.get_current_width(),main_menu_bar_size_.y));
    ImGui::SetNextWindowSize(sf::Vector2f(expand_collapse_button_width_, io.DisplaySize.y - main_menu_bar_size_.y));

    if(ImGui::Begin("Node Menu Expand Button Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
    {
        std::string expand_button_text = ">\n>\n>\n\nM\ne\nn\nu\n\n>\n>\n>";
        if(node_menu_drawer_.is_drawer_opening())
            expand_button_text = "<\n<\n<\n\nM\ne\nn\nu\n\n<\n<\n<";

        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f,0.5f));

        if(ImGui::Button(expand_button_text.c_str(), ImVec2(expand_collapse_button_width_ - 15,io.DisplaySize.y - main_menu_bar_size_.y - 20)))
        {
            if(node_menu_drawer_.is_drawer_opening())
                node_menu_drawer_.start_closing_drawer();
            else
                node_menu_drawer_.start_opening_drawer();
        }

        if(ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        ImGui::PopStyleVar(1);
    }

    ImGui::End();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to draw the "Expanded Node Contents"
//-------------------------------------------------------------------
inline void LazyDataEditor::draw_expanded_node_contents_window()
{
    ImGuiIO& io = ImGui::GetIO();

    if(expanded_node_drawer_.is_drawer_partially_open())
    {
        ImGui::SetNextWindowPos(sf::Vector2f(io.DisplaySize.x - expanded_node_drawer_.get_current_width(), main_menu_bar_size_.y));
        ImGui::SetNextWindowSize(sf::Vector2f(expanded_node_drawer_.get_current_width(), io.DisplaySize.y - main_menu_bar_size_.y));
        
        if(ImGui::Begin("Expanded In-Focus Node", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
        {
        }
        
        ImGui::End();
    }


    // Draw the expand/collapse button

    ImGui::SetNextWindowPos(sf::Vector2f(io.DisplaySize.x - expanded_node_drawer_.get_current_width() - expand_collapse_button_width_, main_menu_bar_size_.y));
    ImGui::SetNextWindowSize(sf::Vector2f(expand_collapse_button_width_, io.DisplaySize.y - main_menu_bar_size_.y));

    if(ImGui::Begin("In-Focus Node Expand Button Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
    {
        std::string expand_button_text = "<\n<\n<";
        if(expanded_node_drawer_.is_drawer_opening())
            expand_button_text = ">\n>\n>";

        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f,0.5f));

        if(ImGui::Button(expand_button_text.c_str(), ImVec2(expand_collapse_button_width_ - 15, io.DisplaySize.y - main_menu_bar_size_.y - 20)))
        {
            if(expanded_node_drawer_.is_drawer_opening())
                expanded_node_drawer_.start_closing_drawer();
            else
                expanded_node_drawer_.start_opening_drawer();
        }

        ImGui::PopStyleVar(1);
    }

    ImGui::End();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_LAZY_DATA_EDITOR_HPP_