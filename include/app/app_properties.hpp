#ifndef INCLUDE_APP_PROPERTIES_HPP_
#define INCLUDE_APP_PROPERTIES_HPP_



//-------------------------------------------------------------------
#include <string>
#include <iostream>

// LazyMatrix library
#include <lazy_matrix.hpp>

// SFML
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

// ImGui
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_internal.h>

// imnodes library (for node editor)
#include <imnodes.h>

// imgui plotting library implot
#include <implot.h>

// Native File Dialog library
#include <utils/native_file_dialog.hpp>

// Unique id generator
#include <utils/unique_id.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define everything within the namespace LazyApp
//-------------------------------------------------------------------
namespace LazyApp
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Structure holding properties for an SFML-based application.
 * 
 * This structure encapsulates various settings and properties
 * used for configuring an SFML application window, including
 * appearance, behavior, and OpenGL context settings.
 */
//-------------------------------------------------------------------
struct AppProperties
{
    std::string title = "My App"; ///< Title of the application window.
    sf::Color background_color = sf::Color(0,0,0,255); ///< Background color of the window.

    // Opengl specific settings
    sf::ContextSettings context_settings = sf::ContextSettings(24, 8, 4, 4, 6, sf::ContextSettings::Default, false); ///< OpenGL context settings.

    sf::VideoMode video_mode = sf::VideoMode::getDesktopMode(); ///< Video mode (width, height, bit_per_pixel).

    sf::Uint32 style = sf::Style::Default; ///< Window style flags (e.g., title bar, resizable).

    int frames_per_second_limit = 0; ///< Frame rate limit. A value <= 0 means no limit.

    /**
     * @brief Determines whether to synchronize the window update rate with the monitor's refresh rate.
     * 
     * Note: This supersedes the frames_per_second_limit setting. If this is set to true,
     * then frames_per_second_limit is ignored.
     */
    bool should_vertical_sync_be_enabled = true;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyApp
//-------------------------------------------------------------------



#endif  // INCLUDE_APP_PROPERTIES_HPP_
