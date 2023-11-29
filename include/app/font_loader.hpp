#ifndef INCLUDE_FONT_LOADER_HPP_
#define INCLUDE_FONT_LOADER_HPP_



//-------------------------------------------------------------------
#include <fstream>
#include <vector>

#include "base_imgui_app.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyApp
//-------------------------------------------------------------------
namespace LazyApp
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
const std::string DEFAULT_FONT_RESOURCE_FOLDER = LazyMatrix::get_absolute_path_of_executable_parent_directory().string() + std::string("/resources/fonts/");
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @struct FontInformation
 * @brief Stores information about a font.
 */
//-------------------------------------------------------------------
struct FontInformation
{
    std::string font_name;
    std::string font_absolute_filename;
    float pixel_size = 12;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class FontLoader
 * @brief Facilitates loading and applying fonts used throughout the application.
 */
//-------------------------------------------------------------------
class FontLoader
{
public:

	// Constructor
	FontLoader()
	{
	}



    /**
     * Loads a font from the application's default resource/font sub-folder.
     * @param font_name Name used to refer to the font (e.g., "Roboto-Regular").
     * @param font_filename Filename of the font (e.g., "roboto/Roboto-Regular.ttf").
     * @param pixel_size Size of the font.
     * @param font_resource_folder Folder where the font is located.
     */
    void  load_font_from_resource_font_folder(const std::string font_name,
                                              const std::string& font_filename,
                                              float pixel_size,
                                              const std::string font_resource_folder = DEFAULT_FONT_RESOURCE_FOLDER)
    {
        // First we construct the absolute path to the font's filename
        std::string font_absolute_filename = font_resource_folder + font_filename;

        ImGuiIO& io = ImGui::GetIO();

        // We need to clear all fonts even if only the default
        // font was loaded previously
        io.Fonts->Clear();

        // We then reload all previously loaded fonts
        // including the Default font
        
        //io.Fonts->AddFontDefault(); // Default font

        for(const auto& font_info : loaded_fonts_)
        {
            io.Fonts->AddFontFromFileTTF(font_info.font_absolute_filename.c_str(), font_info.pixel_size);
        }

        // We now load the newly specified font
        io.Fonts->AddFontFromFileTTF(font_absolute_filename.c_str(), pixel_size);

        // We then store information about the
        // newly loaded font
        FontInformation info_about_newly_loaded_font = {font_name, font_absolute_filename, pixel_size};
        loaded_fonts_.push_back(info_about_newly_loaded_font);

        ImGui::SFML::UpdateFontTexture();
    }



    /**
     * Pushes a font based on its name.
     * @param font_name Name of the font to push.
     */
    void push_font(const std::string& font_name)const
    {
        // If we don't find the specified font name
        // in the fonts that have been loaded, then
        // we apply the default font

        ImGuiIO& io = ImGui::GetIO();

        for(int i = 0; i < loaded_fonts_.size(); ++i)
        {
            if(loaded_fonts_[i].font_name == font_name)
            {
                 ImGui::PushFont(io.Fonts->Fonts[1]);
                return;
            }
        }
    }

    /**
     * Pops the last pushed font.
     */
    void pop_font()const
    {
        ImGui::PopFont();
    }
    


private:

    // vector holding the information to every font
    // loaded by the application
    std::vector<FontInformation> loaded_fonts_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyApp
//-------------------------------------------------------------------



#endif  // INCLUDE_FONT_LOADER_HPP_