#ifndef INCLUDE_MY_APPLICATION_HPP_
#define INCLUDE_MY_APPLICATION_HPP_



//-------------------------------------------------------------------
#include "app/base_imgui_app.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyApp 
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
class MyImGuiApplication : public LazyApp::BaseImGuiApp<MyImGuiApplication> 
{
public:
    MyImGuiApplication()
        : BaseImGuiApp(AppProperties()) // Initialize with default AppProperties
    {
        // Constructor content
    }

    bool init()
    {
        // Initialization code
        return true;
    }

    void first_loop_iteration()
    {
        // Code for first iteration
    }

    /**
     * @brief Checks and processes window events.
     * 
     * @param event The event that needs to be processed
     */
    void handle_events(const sf::Event& event)
    {
        return;
    }

    void loop_function(const sf::Time& delta_time)
    {
        sf::Event event;
        while (this->window_->pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                this->quit();
                return;
            }
        }

        ImGui::SFML::Update(*this->window_, delta_time);

        // ImGui drawing code here
        ImGui::Begin("Hello, ImGui!");
        ImGui::Button("Sample Button");
        ImGui::End();

        this->window_->clear();
        ImGui::SFML::Render(*this->window_);
        this->window_->display();
    }

    void kill()
    {
        // Cleanup code here
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyApp
//-------------------------------------------------------------------



#endif  // INCLUDE_MY_APPLICATION_HPP_