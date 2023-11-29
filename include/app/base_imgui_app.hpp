#ifndef INCLUDE_BASE_IMGUI_APP_HPP_
#define INCLUDE_BASE_IMGUI_APP_HPP_



//-------------------------------------------------------------------
#include "base_sfml_app.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyApp
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Base class for ImGui applications using the CRTP pattern.
 * 
 * This class extends the BaseSfmlApp to integrate ImGui, providing
 * a framework for SFML applications that utilize ImGui for their GUI.
 * It manages ImGui-specific initializations and cleanup.
 * 
 * @tparam AppType The derived application type.
 */
//-------------------------------------------------------------------
template<typename AppType>
class BaseImGuiApp : public BaseSfmlApp<BaseImGuiApp<AppType> >
{
public:

    /**
     * @brief Initializes ImGui within the SFML application.
     * 
     * @return true If initialization is successful.
     * @return false If initialization fails.
     */
    bool init()
    {
        ImGui::SFML::Init(*this->window_);
        return this->underlying().init();
    }

    /**
     * @brief Executes operations required during the first loop iteration.
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
     * @brief Cleans up ImGui resources before the application quits.
     */
    void kill()
    {
        this->underlying().kill();
        
        if (this->window_)
        {
            ImGui::SFML::Shutdown(*this->window_);
        }
    }



private:

    /**
     * @brief Construct a new Base ImGui App object.
     * 
     * Private constructor to enforce the CRTP pattern, ensuring instantiation 
     * only through derived classes. The derived class is declared as a friend 
     * to access this private constructor.
     * 
     * @param app_properties Application properties.
     */
    explicit BaseImGuiApp<AppType>(const AppProperties& app_properties = AppProperties())
        : BaseSfmlApp<BaseImGuiApp<AppType>>(app_properties)
    {
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
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyApp
//-------------------------------------------------------------------



#endif  // INCLUDE_BASE_IMGUI_APP_HPP_
