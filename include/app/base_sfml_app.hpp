#ifndef INCLUDE_BASE_SFML_APP_HPP_
#define INCLUDE_BASE_SFML_APP_HPP_



//-------------------------------------------------------------------
#include <atomic>
#include "app_properties.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyApp
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Base SFML App framework based on CRTP pattern.
 * 
 * This template class provides a foundation for building applications
 * using SFML. It handles the main application loop, window management,
 * and basic application lifecycle.
 * 
 * @tparam AppType The derived application type.
 */
//-------------------------------------------------------------------
template<typename AppType>
class BaseSfmlApp
{
public:
    
    /**
     * @brief Sets the application properties.
     * 
     * @param app_properties The desired application properties.
     */
    void set_app_properties(const AppProperties& app_properties)
    {
        app_properties_ = app_properties;
    }



    /**
     * @brief Gets the current application properties.
     * 
     * @return const AppProperties& The current application properties.
     */
    const AppProperties& get_app_properties() const
    {
        return app_properties_;
    }



    /**
     * @brief Signals the application to quit.
     */
    void quit()
    {
        this->should_app_quit_ = true;
    }



    /**
     * @brief Initializes the application, setting up the main window.
     * 
     * @return true If initialization is successful.
     * @return false If initialization fails.
     */
    bool init()
    {
        window_ = new sf::RenderWindow(app_properties_.video_mode,
                                       app_properties_.title,
                                       app_properties_.style);

        if(!window_->isOpen())
        {
            delete window_;
            window_ = nullptr;

            return false;
        }

        if(app_properties_.should_vertical_sync_be_enabled)
        {
            window_->setVerticalSyncEnabled(true);
        }
        else
        {
            window_->setVerticalSyncEnabled(false);
            window_->setFramerateLimit(app_properties_.frames_per_second_limit);
        }
        
        return underlying().init();
    }



    /**
     * @brief Executes operations required during the first loop iteration.
     */
    void first_loop_iteration()
    {
        underlying().first_loop_iteration();
    }



    /**
     * @brief Checks and processes window events.
     */
    void handle_events()
    {
        sf::Event event;
    
        while(this->window_->pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(*this->window_, event);

            if(event.type == sf::Event::Closed)
            {
                this->quit();
            }
            else
            {
                underlying().handle_events(event);
            }
        }
    }



    /**
     * @brief The core loop function of the application, called every iteration.
     * 
     * @param delta_time The time elapsed since the last loop iteration.
     */
    void loop_function(const sf::Time& delta_time)
    {
        underlying().loop_function(delta_time);
    }



    /**
     * @brief Cleans up resources before the application quits.
     */
    void kill()
    {
        underlying().kill();

        if(window_)
        {
            if(window_->isOpen())
                window_->close();
                
            delete window_;
            window_ = nullptr;
        }
    }



    /**
     * @brief Main application loop. Runs the core loop until a quit signal is received.
     * 
     * @return int The application's exit code.
     */
    int main_loop()
    {
        if(!this->init())
            return 1;

        this->first_loop_iteration();

        delta_clock_.restart();

        while(!should_app_quit_)
        {
            this->handle_events();

            this->loop_function(delta_clock_.restart());
        }

        this->kill();
        
        return 0;
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
    explicit BaseSfmlApp<AppType>(const AppProperties& app_properties = AppProperties())
    {
        set_app_properties(app_properties);
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



protected:

    std::atomic<bool> should_app_quit_ = ATOMIC_VAR_INIT(false);
    AppProperties app_properties_;
    sf::RenderWindow* window_ = nullptr;
    sf::Clock delta_clock_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyApp
//-------------------------------------------------------------------



#endif  // INCLUDE_BASE_SFML_APP_HPP_
