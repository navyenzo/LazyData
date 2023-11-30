#ifndef FILE_BROWSER_MANAGER_HPP_
#define FILE_BROWSER_MANAGER_HPP_



//-------------------------------------------------------------------
#include <imfilebrowser.h>
#include <string>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyApp 
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Manages a global file browser instance for ImGui.
 * 
 * This class uses a static inline member to ensure a single file browser instance
 * across the application. It can be used to open a file browser from any context,
 * and retrieve the selected file path.
 */
//-------------------------------------------------------------------
class FileBrowserManager
{
public:

    /**
     * Opens the file browser dialog with specified file type filters.
     * @param nodeId The ID of the node requesting to open the file browser.
     * @param file_types A vector of strings specifying the file extensions to be displayed in the file browser.
     *                   Each string in the vector should be a file extension (e.g., ".png", ".jpg").
     */
    static void open_file_browser(int nodeId, const std::vector<std::string>& file_types)
    {
        current_node_id = nodeId;
        auto& fileBrowser = get_file_browser();

        fileBrowser.SetTypeFilters(file_types);
        fileBrowser.Open();
    }


    /**
     * Renders the file browser dialog if it is open.
     */
    static void display()
    {
        get_file_browser().Display();
    }

    /**
     * Checks if a file has been selected and if the requesting node matches.
     * @param nodeId The ID of the node checking for the selected file.
     * @return The selected file path if the conditions are met, otherwise an empty string.
     */
    static std::string has_selected(int nodeId)
    {
        if (current_node_id == nodeId && get_file_browser().HasSelected())
        {
            std::string path = get_file_browser().GetSelected().string();
            get_file_browser().ClearSelected();
            return path;
        }
        return "";
    }

private:

    /**
     * @brief Returns a reference to the static file browser instance.
     * 
     * This function is private to ensure that the file browser instance is only
     * manipulated through the public interface of FileBrowserManager.
     * 
     * @return Reference to the static ImGui::FileBrowser instance.
     */
    static ImGui::FileBrowser& get_file_browser()
    {
        static ImGui::FileBrowser fileBrowser;
        return fileBrowser;
    }



    static inline int current_node_id = -1; ///< ID of the node that opened the file browser.
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyApp
//-------------------------------------------------------------------



#endif // FILE_BROWSER_MANAGER_HPP_
