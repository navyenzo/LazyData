#ifndef INCLUDE_MATRIX_TABLE_UI_HPP_
#define INCLUDE_MATRIX_TABLE_UI_HPP_



//-------------------------------------------------------------------
#include <algorithm>
#include "constants_and_defaults.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define everything within the namespace DataFlow
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Function for drawing an ImGui table showing the values of a matrix.
 * 
 * This function creates a table in the ImGui context to display the contents
 * of a given matrix. It supports pagination for large matrices and editable
 * entries if enabled.
 * 
 * @param matrix_data The matrix data to be displayed in the table.
 * @param page_index The current page index for displaying the matrix.
 * @param table_size The size of the table to be drawn.
 * @param are_entries_editable Flag to determine if the matrix entries are editable.
 * @return true If any matrix entry was edited.
 * @return false If no matrix entries were edited.
 */
//-------------------------------------------------------------------
inline bool draw_matrix_table(MatrixType& matrix_data, int& page_index, const ImVec2& table_size, bool are_entries_editable)
{
    bool were_entries_edited = false; // Track if any entries were edited

    // Display matrix overall information
    ImGui::BeginGroup();
    {
        ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "Matrix size:");
        ImGui::SameLine();
        ImGui::Text("(%ix%i)", matrix_data.rows(), matrix_data.columns());

        ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "Matrix file size:");
        ImGui::SameLine();
        ImGui::Text("%u Bytes", matrix_data.get_mapped_file_size());

        ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "Location:");
        ImGui::SameLine();
        ImGui::Text(matrix_data.get_filename_of_memory_mapped_file().string().c_str());

        ImGui::Dummy(ImVec2(0, 15));
    }
    ImGui::EndGroup();

    // Draw the matrix table if it contains data
    if(matrix_data.size() > 0)
    {
        ImGui::BeginGroup();
        {
            const int number_of_visible_columns = std::min(matrix_data.columns(), uintptr_t(IMGUI_TABLE_MAX_COLUMNS - 3)) + 1;
            const int max_number_of_pages = matrix_data.columns() / number_of_visible_columns;

            // Page control if the table has multiple pages
            if(max_number_of_pages > 0)
            {
                ImGui::SliderInt("", &page_index, 0, max_number_of_pages);
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "page: %i of %i", page_index, max_number_of_pages);
            }
            else
            {
                page_index = 0; // Reset to first page if only one page
            }

            // Begin the table
            if(ImGui::BeginTable("Data", number_of_visible_columns + 1, ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, table_size))
            {
                ImGui::TableSetupScrollFreeze(1, 1); // Freeze the first row and column

                // Use the vertical clipper for large tables
                ImGuiListClipper clipper;
                clipper.Begin(matrix_data.rows() + 1);

                while(clipper.Step())
                {
                    for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
                    {
                        ImGui::TableNextRow();

                        for(int column = 0, actual_column = page_index * (number_of_visible_columns - 1);
                            column < number_of_visible_columns && actual_column < matrix_data.columns();
                            ++column, ++actual_column)
                        {
                            if(ImGui::TableNextColumn())
                            {
                                if(row == 0 && column == 0)
                                    ImGui::Text("");
                                else if(row == 0)
                                    ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "col: %i", actual_column);
                                else if(column == 0)
                                    ImGui::TextColored(ImVec4(0.0, 1.0, 1.0, 1.0), "row: %i", row - 1);
                                else if(are_entries_editable)
                                {
                                    ImGui::PushItemWidth(60);
                                    ImGui::PushID(row * matrix_data.columns() + actual_column);
                                    if(ImGui::InputDouble("",&matrix_data(row - 1, actual_column), 0.0, 0.0, "%lf", ImGuiInputTextFlags_EnterReturnsTrue))
                                        were_entries_edited = true;
                                    if(ImGui::IsItemHovered())
                                    {
                                        ImGui::BeginTooltip();
                                        ImGui::Text("%lf", matrix_data(row - 1, actual_column));
                                        ImGui::EndTooltip();
                                    }
                                    ImGui::PopID();
                                    ImGui::PopItemWidth();
                                }
                                else
                                    ImGui::Text("%lf", matrix_data(row - 1, actual_column));
                            }
                        }
                    }
                }

                clipper.End();
                ImGui::EndTable();
            }
        }

        ImGui::EndGroup();
    }

    return were_entries_edited;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * Draws an ImGui table for a CSVMatrix, allowing text display and editing with constraints.
 *
 * @param matrix_data Reference to the CSVMatrix.
 * @param page_index Current page index for displaying the matrix.
 * @param table_size Size of the ImGui table.
 * @param are_entries_editable Whether the matrix entries are editable.
 * @return true If any matrix entry was edited, false otherwise.
 */
//-------------------------------------------------------------------
template<typename DataType>

inline bool draw_matrix_text_table(LazyMatrix::CSVMatrix<DataType>& matrix_data, 
                                   int& page_index, 
                                   const ImVec2& table_size, 
                                   bool are_entries_editable,
                                   bool has_column_headers,
                                   bool has_row_headers)
{
    bool were_entries_edited = false;

    if(matrix_data.rows() > 0 && matrix_data.columns() > 0)
    {
        ImGui::BeginGroup();
        {
            const int number_of_visible_columns = std::min(matrix_data.columns(), uintptr_t(IMGUI_TABLE_MAX_COLUMNS - 3)) + 1;
            const int max_number_of_pages = matrix_data.columns() / number_of_visible_columns;

            if(max_number_of_pages > 0)
            {
                ImGui::SliderInt("Page", &page_index, 0, max_number_of_pages);
                ImGui::SameLine();
                ImGui::Text("Page: %i of %i", page_index, max_number_of_pages);
            }
            else
            {
                page_index = 0;
            }

            if(ImGui::BeginTable("Matrix Data", number_of_visible_columns + 1, ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, table_size))
            {
                // Adjust scroll freeze based on header presence
                if (has_column_headers && has_row_headers) {
                    ImGui::TableSetupScrollFreeze(1, 1); // Freeze first row and first column
                } else if (has_column_headers) {
                    ImGui::TableSetupScrollFreeze(0, 1); // Freeze first row only
                } else if (has_row_headers) {
                    ImGui::TableSetupScrollFreeze(1, 0); // Freeze first column only
                } else {
                    ImGui::TableSetupScrollFreeze(0, 0); // Do not freeze any rows or columns
                }

                ImGuiListClipper clipper;
                clipper.Begin(matrix_data.rows() + 1);

                while(clipper.Step())
                {
                    for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
                    {
                        ImGui::TableNextRow();

                        for(int column = 0, actual_column = page_index * (number_of_visible_columns - 1);
                            column < number_of_visible_columns && actual_column < matrix_data.columns();
                            ++column, ++actual_column)
                        {
                            if(ImGui::TableNextColumn())
                            {
                                if(row == 0 && column == 0 && (has_column_headers || has_row_headers)) 
                                {
                                    ImGui::Text(""); // Top-left corner cell
                                }
                                else if(row == 0 && has_column_headers) 
                                {
                                    ImGui::Text("%s", matrix_data.get_column_header(actual_column).c_str());
                                }
                                else if(column == 0 && has_row_headers) 
                                {
                                    ImGui::Text("%s", matrix_data.get_row_header(row - 1).c_str());
                                }
                                else
                                {
                                    int cell_row = row - (has_column_headers ? 1 : 0);
                                    int cell_column = actual_column - (has_row_headers ? 1 : 0);

                                    // Cell editing
                                    std::string_view currentContent = matrix_data.string_at(cell_row, cell_column);
                                    std::string buffer(currentContent.data(), currentContent.size());
                                    buffer.resize(currentContent.size() + 1, '\0'); // Resize buffer for null-termination
                                    
                                    ImGui::PushItemWidth(60);
                                    ImGui::PushID(row * matrix_data.columns() + actual_column);
                                    
                                    if(are_entries_editable && ImGui::InputText("##edit", buffer.data(), buffer.size(), ImGuiInputTextFlags_EnterReturnsTrue))
                                    {
                                        std::string newContent(buffer);
                                        if(newContent.length() <= currentContent.length())
                                        {
                                            // Update cell if new content is not larger than current
                                            // matrix_data.set_cell_value_from_string(cell_row, cell_column, newContent);
                                            were_entries_edited = true;
                                        }
                                        else
                                        {
                                            // Display warning: Input too long
                                        }
                                    }
                                    else
                                    {
                                        ImGui::Text("%.*s", static_cast<int>(currentContent.size()), currentContent.data());
                                    }
                                    
                                    ImGui::PopID();
                                    ImGui::PopItemWidth();
                                }
                            }
                        }
                    }
                }

                clipper.End();
                ImGui::EndTable();
            }
        }
        ImGui::EndGroup();
    }

    return were_entries_edited;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_MATRIX_TABLE_UI_HPP_
