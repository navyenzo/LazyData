#ifndef INCLUDE_FILE_DIALOG_HPP
#define INCLUDE_FILE_DIALOG_HPP



//-------------------------------------------------------------------
#include <filesystem>
#include <string>
#include <vector>
#include <system_error>

#include <nfd.h>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyApp 
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class FileDialog
 * @brief Class to handle opening native file dialogs.
 *
 * Provides methods for opening save dialogs, open dialogs for multiple files,
 * and open dialogs for directories.
 */
//-------------------------------------------------------------------
class FileDialog 
{
public:

    /**
     * Opens a save file dialog.
     * 
     * @param filterList A string specifying the file type filter.
     * @param defaultPath The default path where the dialog will open.
     * @param ec Error code object to capture any errors.
     * @return The path to the file selected by the user.
     */
    static std::filesystem::path save_dialog(
        const std::string& filterList = "", 
        const std::filesystem::path& defaultPath = ""
    )
    {
        nfdchar_t* outPath = nullptr;
        nfdresult_t result = NFD_SaveDialog(filterList.c_str(), defaultPath.c_str(), &outPath);

        if (result == NFD_OKAY) 
        {
            std::filesystem::path filePath(outPath);
            free(outPath);
            return filePath;
        } 
        
        return {};
    }



    /**
     * Opens a dialog for selecting a single file.
     * 
     * @param filterList A string specifying the file type filter.
     * @param defaultPath The default path where the dialog will open.
     * @param ec Error code object to capture any errors.
     * @return The path to the file selected by the user.
     */
    static std::filesystem::path open_dialog_single(
        const std::string& filterList = "", 
        const std::filesystem::path& defaultPath = ""
    )
    {
        nfdchar_t* outPath = nullptr;
        nfdresult_t result = NFD_OpenDialog(filterList.c_str(), defaultPath.c_str(), &outPath);

        if (result == NFD_OKAY) 
        {
            std::filesystem::path filePath(outPath);
            free(outPath);
            return filePath;
        }

        return {};
    }



    /**
     * Opens a dialog for selecting a single image file.
     * 
     * @param defaultPath The default path where the dialog will open.
     * @param ec Error code object to capture any errors.
     * @return The path to the image file selected by the user.
     */
    static std::filesystem::path open_dialog_image(
        const std::filesystem::path& defaultPath = ""
    )
    {
        std::cout << "\ncazzzo\n\n\n" << std::endl;
        // Common image file formats
        const std::string filterList = "png,jpg,jpeg,bmp,tiff,tif,gif,svg";// Common image file formats
        std::cout << "\ncazzzo 2\n\n\n" << std::endl;

        return open_dialog_single(filterList, defaultPath);
    }



    /**
     * Opens a dialog for selecting multiple files.
     * 
     * @param filterList A string specifying the file type filter.
     * @param defaultPath The default path where the dialog will open.
     * @param ec Error code object to capture any errors.
     * @return A vector of paths to the files selected by the user.
     */
    static std::vector<std::filesystem::path> open_dialog_multiple(
        const std::string& filterList = "", 
        const std::filesystem::path& defaultPath = ""
    )
    {
        nfdpathset_t pathSet;
        nfdresult_t result = NFD_OpenDialogMultiple(filterList.c_str(), defaultPath.c_str(), &pathSet);

        std::vector<std::filesystem::path> filePaths;

        if (result == NFD_OKAY) 
        {
            size_t count = NFD_PathSet_GetCount(&pathSet);
            for (size_t i = 0; i < count; ++i) 
            {
                nfdchar_t* path = NFD_PathSet_GetPath(&pathSet, i);
                filePaths.emplace_back(path);
            }
            NFD_PathSet_Free(&pathSet);
        }

        return filePaths;
    }

    

    /**
     * Opens a dialog for selecting a directory.
     * 
     * @param defaultPath The default path where the dialog will open.
     * @param ec Error code object to capture any errors.
     * @return The path to the directory selected by the user.
     */
    static std::filesystem::path open_dialog_directory(
        const std::filesystem::path& defaultPath = ""
    )
    {
        nfdchar_t* outPath = nullptr;
        nfdresult_t result = NFD_PickFolder(defaultPath.c_str(), &outPath);

        if (result == NFD_OKAY) 
        {
            std::filesystem::path directoryPath(outPath);
            free(outPath);
            return directoryPath;
        }

        return {};
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyApp
//-------------------------------------------------------------------



#endif // INCLUDE_FILE_DIALOG_HPP
