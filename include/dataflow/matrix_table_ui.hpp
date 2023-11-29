#ifndef INCLUDE_MATRIX_TABLE_UI_HPP_
#define INCLUDE_MATRIX_TABLE_UI_HPP_



//-------------------------------------------------------------------
#include <algorithm>

#include "constants_and_defaults.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This function draws an ImGui table showing the values of a
// LazyMatrix::MappedMatrix matrix
//-------------------------------------------------------------------
inline bool draw_matrix_table(MatrixType& matrix_data, int& page_index, const ImVec2& table_size, bool are_entries_editable)
{
    bool were_entries_edited = false;

    // Mapped matrix overall information
    ImGui::BeginGroup();
        ImGui::TextColored(ImVec4(1.0,1.0,0.0,1.0),"Matrix size:");
        ImGui::SameLine();
        ImGui::Text("(%ix%i)", matrix_data.rows(), matrix_data.columns());

        ImGui::TextColored(ImVec4(1.0,1.0,0.0,1.0),"Matrix file size:");
        ImGui::SameLine();
        ImGui::Text("%u Bytes", matrix_data.get_mapped_file_size());

        ImGui::TextColored(ImVec4(1.0,1.0,0.0,1.0),"Location:");
        ImGui::SameLine();
        ImGui::Text(matrix_data.get_filename_of_memory_mapped_file().string().c_str());

        ImGui::Dummy(ImVec2(0,15));
    ImGui::EndGroup();

    // Mapped matrix Table
    if(matrix_data.size() > 0)
    {
        ImGui::BeginGroup();

        int number_of_visible_columns = std::min(matrix_data.columns(), IMGUI_TABLE_MAX_COLUMNS - 3) + 1;

        // We use the idea of pages in order to show large tables
        // that have more than IMGUI_TABLE_MAX_COLUMNS columns

        int max_number_of_pages = matrix_data.columns() / number_of_visible_columns;

        if(max_number_of_pages > 0)
        {
            ImGui::SliderInt("", &page_index, 0, max_number_of_pages);
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0,1.0,0.0,1.0), "page: %i of %i", page_index, max_number_of_pages);
        }
        else
            page_index = 0;

        if(ImGui::BeginTable("Data",
                             number_of_visible_columns + 1,
                             ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                             table_size))
        {
            // Always show the row and column headers
            ImGui::TableSetupScrollFreeze(1,1);

            // Use the vertical clipper in order to
            // show large tables (many rows)
            ImGuiListClipper clipper;
            clipper.Begin(matrix_data.rows() + 1);

            while(clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
                {
                    ImGui::TableNextRow();
                    
                    for (int column = 0, actual_column = (number_of_visible_columns - 1) * page_index - 1;
                            column < (number_of_visible_columns + 1) && actual_column < matrix_data.columns();
                            ++column, ++actual_column)
                    {
                        if(ImGui::TableNextColumn())
                        {
                            if(row == 0 && column == 0)
                                ImGui::Text("");
                            else if(row == 0)
                                ImGui::TextColored(ImVec4(1.0,1.0,0.0,1.0), "col: %i", actual_column);
                            else if(column == 0)
                                ImGui::TextColored(ImVec4(0.0,1.0,1.0,1.0), "row: %i", row - 1);
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

        ImGui::EndGroup();
    }

    return were_entries_edited;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_MATRIX_TABLE_UI_HPP_