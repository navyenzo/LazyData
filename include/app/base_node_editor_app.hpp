#ifndef INCLUDE_BASE_NODE_EDITOR_APP_HPP_
#define INCLUDE_BASE_NODE_EDITOR_APP_HPP_



//-------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <map>

//#include <imgui_canvas.h>

#include "base_imgui_app.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace LazyApp
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename AppType>
class BaseNodeEditorApp : public BaseImGuiApp<BaseNodeEditorApp<AppType> >
{
public:

    /**
     * @brief Initializes the application, setting up the main window.
     * 
     * @return true If initialization is successful.
     * @return false If initialization fails.
     */
	bool init()
    {
        // First we initialize the ImNodes context
        node_editor_context_ = ImNodes::CreateContext();

        // Then initialize implot
        ImPlot::CreateContext();

        // Load fonts used throughout the application
        // TODO: Fonts still not working correctly, right
        //       now i can only load one font, will need
        //       to fix this
        font_loader_.load_font_from_resource_font_folder("proggy_vector_regular", "proggyfonts/ProggyVector/ProggyVector Regular.ttf", 18);

        return this->underlying().init();
    }

	/**
     * @brief Function called during the first iteration of the main_loop
     */
	void first_loop_iteration()
    {
        this->underlying().first_loop_iteration();
    }

    /**
     * @brief Checks and processes window events.
     * 
     * @param event The event that needs to be processed
     */
    void handle_events(const sf::Event& event)
    {
        this->underlying().handle_events(event);
    }

	/**
     * @brief The core loop function of the application, called every iteration.
     * 
     * @param delta_time The time elapsed since the last loop iteration.
     */
	void loop_function(const sf::Time& delta_time)
    {
        this->underlying().loop_function(delta_time);
    }

	/**
     * @brief Cleans up resources before the application quits.
     */
	void kill()
    {
        this->underlying().kill();

        // Here we need to destroy the ImPlot and
        // ImNodes contexts, we do this only once
        // right before the Node Editor user interface
        // is closed
        ImPlot::DestroyContext();
        ImNodes::DestroyContext(node_editor_context_);
        node_editor_context_ = nullptr;
    }
    


private:

    /**
     * @brief Construct a new Base Sfml App object. 
     * 
     * This constructor is private to enforce the use of CRTP pattern, 
     * meaning that this base class should only be instantiated through 
     * derived classes. The derived class is declared as a friend to 
     * allow it to access this private constructor.
     * 
     * @param app_properties Application properties.
     */
    explicit BaseNodeEditorApp<AppType>(const AppProperties& app_properties = AppProperties())
    {
        this->set_app_properties(app_properties);
    }

    friend AppType;



    AppType& underlying()
    {
        return static_cast<AppType&>(*this);
    }

    const AppType& underlying() const
    {
        return static_cast<const AppType&>(*this);
    }



    ImNodesContext* node_editor_context_ = nullptr;
    
    LazyApp::FontLoader font_loader_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyApp
//-------------------------------------------------------------------



#endif  // INCLUDE_BASE_NODE_EDITOR_APP_HPP_